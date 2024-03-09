#ifndef __HYPER_LOG_LOG_HPP__

#define __HYPER_LOG_LOG_HPP__

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

enum class HLLSetResult {
    SUCCESS,
    NOUPDATE,
    PROMOTIONREQUIRED,
    INVALIDFORMAT,
    ERROR // 其他错误
};

struct HLLHeader {
    HLLHeader()
        : _verification_code{'H', 'Y', 'L', 'L'}, _data_type(0), _cache(0) {
        std::fill(std::begin(_future_use), std::end(_future_use), 0);
    }

    char _verification_code[4];
    uint8_t _data_type;
    uint8_t _future_use[3];
    uint64_t _cache;
};

class HyperLogLog {
  public:
    static constexpr int Precision = 14;
    static constexpr int QBits = 64 - Precision;
    static constexpr int Registers = 1 << Precision;
    static constexpr int PMask = Registers - 1;
    static constexpr int BitCount = 6;
    static constexpr int RegisterMax = (1 << BitCount) - 1;
    static constexpr size_t HeaderSize = sizeof(HLLHeader);
    static constexpr size_t DenseSize =
        (HeaderSize + ((Registers * BitCount + 7) / 8));
    static constexpr int DenseEncoding = 0;
    static constexpr int SparseEncoding = 1;
    static constexpr int MaxEncoding = 1;
    //-------------------------------------
    // 稀疏型相关变量
    static constexpr uint8_t XZeroBit = 0x40;
    static constexpr uint8_t ValBit = 0x80;
    static constexpr int ValMaxValue = 32;
    static constexpr int ZeroMaxLen = 64;
    static constexpr int XZeroMaxLen = 16384;
    //-------------------------------------
    static constexpr double AlphaInf = 0.721347520444481703680; // 误差校正常数
    static const char *InvalidHllError;

  public:
    HyperLogLog() : _registers() {}

    void Init() {
        // 默认使用稀疏表示
        _head._data_type = SparseEncoding;

        // 初始化稀疏表示
        // 计算需要多少个XZERO操作来表示所有的寄存器
        int number_of_xzero_ops = (Registers + (XZeroMaxLen - 1)) / XZeroMaxLen;
        // 初始化寄存器的容量
        _registers.resize(number_of_xzero_ops * 2, 0);
        auto it = _registers.begin();
        int aux = Registers;

        // 使用XZERO操作填充寄存器
        while (aux > 0) {
            int xzero = std::min(aux, XZeroMaxLen);
            // 设置XZERO操作，需要把这个逻辑转换成如何在C++中设置
            *it++ = ZeroBit | ((xzero - 1) >> 8);
            *it++ = (xzero - 1) & 0xFF;
            aux -= xzero;
        }
    }

    int Add(const std::string &str) {
        switch (_head._data_type) {
        case DenseEncoding:
            return AddToDense(str);
        case SparseEncoding:
            return AddToSparse(str);
        default:
            return -1;
        }
    }

  private:
    //-------------------------
    // 哈希算法相关函数
    // 判断系统是否为小端字节序
    bool IsLittleEndian() {
        uint32_t num = 1;
        return *(reinterpret_cast<uint8_t *>(&num)) == 1;
    }

    // 安全加载函数，以避免对齐问题
    uint64_t SafeLoad(const uint8_t *data) {
        uint64_t val;
        memcpy(&val, data, sizeof(uint64_t));
        return val;
    }

    // MurmurHash64A 实现，包含字节序和对齐处理
    uint64_t MurmurHash64A(const void *key, int len, uint32_t seed) {
        const uint64_t m = 0xc6a4a7935bd1e995ull; // 哈希函数的乘数
        const int r = 47;                         // 右移的位数
        uint64_t h = seed ^ (len * m); // 使用种子和长度初始化哈希值

        const uint8_t *data =
            reinterpret_cast<const uint8_t *>(key); // 将键转换为字节序列
        const int nblocks = len / 8; // 计算完整8字节块的数量

        // 处理每个8字节块
        for (int i = 0; i < nblocks; i++) {
            uint64_t k = SafeLoad(data); // 安全加载8字节数据
            data += 8;

            if (!IsLittleEndian()) {
                k = __builtin_bswap64(k); // 如果是大端系统，交换字节序
            }

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        // 处理剩余字节
        uint64_t tail = 0;
        switch (len & 7) {
        case 7:
            tail ^= uint64_t(data[6]) << 48;
        case 6:
            tail ^= uint64_t(data[5]) << 40;
        case 5:
            tail ^= uint64_t(data[4]) << 32;
        case 4:
            tail ^= uint64_t(data[3]) << 24;
        case 3:
            tail ^= uint64_t(data[2]) << 16;
        case 2:
            tail ^= uint64_t(data[1]) << 8;
        case 1:
            tail ^= uint64_t(data[0]);
            tail *= m;
            tail ^= tail >> r;
            tail *= m;
            h ^= tail;
        }

        // 最终处理
        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }
    //-------------------------

    uint8_t CalculateZeroBitRunAndIndex(const std::string &element,
                                        long *regp) {
        const std::uint64_t HLL_P_MASK = (1ULL << Precision) - 1;

        std::uint64_t hash =
            MurmurHash64A(reinterpret_cast<unsigned char *>(&element[0]),
                          element.size(), seed);
        // MurmurHash64A(element.data(), element.size(), 0xadc83b19ULL);
        std::uint64_t index = hash & PMask;
        hash >>= Precision;
        hash |= (1ULL << QBits);

        std::uint64_t bit = 1;
        int count = 1; // 初始化成1, 防止死循环
        while ((hash & bit) == 0) {
            count++;
            bit <<= 1;
        }
        *regp = static_cast<long>(index);
        return count;
    }

    int AddToDense(const std::string &str) {}

    int AddToSparse(const std::string &str) {}
    //----------------------------
    // 稀疏型的相关函数
    // 设置 Val 操作码
    void SetSparseVal(std::vector<uint8_t>::iterator it, uint8_t val, int len) {
        // 把val设置进对应的位置，同时设置 val 标识
        *it = ((val)-1) << 2 | ((len - 1)) | ValBit;
    }

    void SetSparseZero(std::vector<uint8_t>::iterator it, int len) {
        *it = len - 1;
    }

    void SetSparseXZero(std::vector<uint8_t>::iterator it, int len) {
        len--;
        // 设置第一个字节并设置 xzero 标识
        *it = len >> 8 | XZeroBit;
        // 设置第二个字节
        *(it + 1) = len & 0xff;
    }

    //----------------------------

  private:
    HLLHeader _head;
    std::vector<uint8_t> _registers;
};
#endif // !__HYPER_LOG_LOG_HPP__

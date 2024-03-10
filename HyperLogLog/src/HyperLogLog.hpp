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
    ERROR  // 其他错误
};

struct HLLHeader {
    HLLHeader()
        : _verification_code{'H', 'Y', 'L', 'L'}, _data_type(0), _cache{0} {
        std::fill(std::begin(_future_use), std::end(_future_use), 0);
    }

    char _verification_code[4];
    uint8_t _data_type;
    uint8_t _future_use[3];
    uint64_t _cache[8];
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
    static constexpr size_t DenseSize = (Registers * BitCount + 7) / 8;
    static constexpr int DenseEncoding = 0;
    static constexpr int SparseEncoding = 1;
    static constexpr int MaxEncoding = 1;
    //-------------------------------------
    // 稀疏型相关变量
    static constexpr int OK = 0;
    static constexpr int ERR = -1;
    static constexpr uint8_t XZeroOpcodeBit = 0x40;  // 01000000
    static constexpr uint8_t ValueOpcodeBit = 0x80;  // 10000000
    static constexpr int ValMaxValue = 32;
    static constexpr int ValMaxLen = 4;
    static constexpr int ZeroMaxLen = 64;
    // static constexpr int XZeroMaxLen = 16384;
    int XZeroMaxLen = 16384;
    // 配置转换密集型的预设值，后续改成可配
    static constexpr size_t MaxBytesForSparseRepresentation = 3000;
    //-------------------------------------
    static constexpr double AlphaInf = 0.721347520444481703680;  // 误差校正常数
    // static const char *InvalidHllError;

   public:
    HyperLogLog() { Init(); }

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
            *it++ = XZeroOpcodeBit | ((xzero - 1) >> 8);
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
    // 标记缓存的基数为失效状态
    inline void MarkCacheAsInvalid() { _head._cache[7] |= (1 << 7); }

    // 检查缓存的基数是否有效
    inline bool IsCacheValid() const {
        return (_head._cache[7] & (1 << 7)) == 0;
    }
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
        // const uint64_t m = 0xc6a4a7935bd1e995ull;  // 哈希函数的乘数
        const uint64_t m = 0xc6a4a7935bd1e995;
        const int r = 47;  // 使用种子和长度初始化哈希值
        uint64_t h = seed ^ (len * m);  // 使用种子和长度初始化哈希值

        const uint8_t *data =
            reinterpret_cast<const uint8_t *>(key);  // 将键转换为字节序列
        const int nblocks = len / 8;  // 计算完整8字节块的数量

        // 处理每个8字节块
        for (int i = 0; i < nblocks; i++) {
            uint64_t k = SafeLoad(data);  // 安全加载8字节数据
            data += 8;

            if (!IsLittleEndian()) {
                k = __builtin_bswap64(k);  // 如果是大端系统，交换字节序
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
    uint64_t MurmurHash64B(const void *key, int len, uint32_t seed) {
        const uint64_t m = 0xc6a4a7935bd1e995;
        const int r = 47;
        uint64_t h = seed ^ (len * m);
        const uint8_t *data = (const uint8_t *)key;
        const uint8_t *end = data + (len - (len & 7));

        while (data != end) {
            uint64_t k;

#if (BYTE_ORDER == LITTLE_ENDIAN)
#ifdef USE_ALIGNED_ACCESS
            memcpy(&k, data, sizeof(uint64_t));
#else
            k = *((uint64_t *)data);
#endif
#else
            k = (uint64_t)data[0];
            k |= (uint64_t)data[1] << 8;
            k |= (uint64_t)data[2] << 16;
            k |= (uint64_t)data[3] << 24;
            k |= (uint64_t)data[4] << 32;
            k |= (uint64_t)data[5] << 40;
            k |= (uint64_t)data[6] << 48;
            k |= (uint64_t)data[7] << 56;
#endif

            k *= m;
            k ^= k >> r;
            k *= m;
            h ^= k;
            h *= m;
            data += 8;
        }

        switch (len & 7) {
            case 7:
                h ^= (uint64_t)data[6] << 48; /* fall-thru */
            case 6:
                h ^= (uint64_t)data[5] << 40; /* fall-thru */
            case 5:
                h ^= (uint64_t)data[4] << 32; /* fall-thru */
            case 4:
                h ^= (uint64_t)data[3] << 24; /* fall-thru */
            case 3:
                h ^= (uint64_t)data[2] << 16; /* fall-thru */
            case 2:
                h ^= (uint64_t)data[1] << 8; /* fall-thru */
            case 1:
                h ^= (uint64_t)data[0];
                h *= m; /* fall-thru */
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;
        return h;
    }
    //-------------------------

    // 稀疏型，检查容量是否足够
    void EnsureCapacity() {
        size_t current_size = _registers.size();
        size_t available = _registers.capacity() - current_size;
        // 如果当前大小小于最大稀疏表示大小，并且可用空间少于3个字节
        if (current_size < MaxBytesForSparseRepresentation && available < 3) {
            size_t new_size = current_size + 3;
            // 贪心策略：如果新的大小小于300，则加倍；否则增加300字节，以避免频繁扩容
            new_size += std::min(new_size, static_cast<size_t>(300));

            // 如果计算出的新大小超过了最大稀疏表示大小，则调整为最大值
            if (new_size > MaxBytesForSparseRepresentation) {
                new_size = MaxBytesForSparseRepresentation;
            }

            _registers.reserve(new_size);
        }
    }

    uint8_t CalculateZeroBitRunAndIndex(const std::string &element,
                                        long *regp) {
        const std::uint64_t HLL_P_MASK = (1ULL << Precision) - 1;

        // std::uint64_t hash = MurmurHash64A(
        //     reinterpret_cast<const unsigned char *>(element.c_str()),
        //     element.size(), 0xadc83b19ULL);
        std::uint64_t hash = MurmurHash64B(
            reinterpret_cast<const unsigned char *>(element.c_str()),
            element.size(), 0xadc83b19ULL);
        // MurmurHash64A(element.data(), element.size(), 0xadc83b19ULL);
        std::uint64_t index = hash & PMask;
        hash >>= Precision;
        hash |=
            (1ULL
             << QBits);  // 确保哈希值的QBits+1位是“1”，这样在寻找连续的零时循环一定会终止

        std::uint64_t bit =
            1;  // 使用bit（从1开始）检查每一位，直到找到一个为“1”的位
        int count = 1;  // 包括了所有连续的零以及跟随的一个“1”
        while ((hash & bit) == 0) {
            count++;
            bit <<= 1;
        }
        *regp = static_cast<long>(index);
        return count;
    }

    int AddToDense(const std::string &str) { return 0; }

    int AddToSparse(const std::string &str) {
        long index = 0;
        uint8_t count = CalculateZeroBitRunAndIndex(str, &index);

        HLLSetResult result = SetSparseRegister(index, count);

        switch (result) {
            case HLLSetResult::SUCCESS:
                // 处理成功的情况
                break;
            case HLLSetResult::NOUPDATE:
                // 处理没有更新的情况
                break;
            case HLLSetResult::PROMOTIONREQUIRED:
                // 处理需要提升类型的情况
                return ConvertHllFromSparseToDense();
            case HLLSetResult::INVALIDFORMAT:
                // 处理无效格式的情况
                break;
            case HLLSetResult::ERROR:
                // 处理错误的情况
                break;
            default:
                // 处理其他可能的未知情况
                break;
        }

        return 0;
    }

    int ConvertHllFromSparseToDense() {
        if (_head._data_type == DenseEncoding) {
            // 已经是密集型就不需要转换
            return OK;
        }

        std::vector<uint8_t> dense(DenseSize, 0);
        int index = 0;
        int run_len = 0;
        int reg_val = 0;

        auto cur = _registers.begin();
        while (cur < _registers.end()) {
            if (IsZeroOpcode(cur)) {
                run_len = GetZeroLength(cur);
                index += run_len;
                cur++;
            } else if (IsXZeroOpcode(cur)) {
                run_len = GetXZeroLength(cur);
                index += run_len;
                cur += 2;
            } else {
                run_len = GetValueLength(cur);
                reg_val = GetValueFromOpcode(cur);

                // 如果大于16384说明被损坏了
                if (run_len + index > Registers) {
                    break;
                }

                while (run_len--) {
                    SetRegisterValue(dense.begin(), index, reg_val);
                    index++;
                }

                cur++;
            }
        }

        // 转换失败
        if (index != Registers) {
            return ERR;  // 稀疏形被损坏
        }

        _registers = std::move((dense));

        // 设置缓存无效
        MarkCacheAsInvalid();
        _head._data_type = DenseEncoding;

        return OK;
    }

    HLLSetResult SetSparseRegister(long index, uint8_t count) {
        uint8_t current_count;  // 当前寄存器的计数值
        std::vector<uint8_t>
            *sparse_representation;  // 指向稀疏表示的vector的指针
        // vector结束位置的迭代器
        std::vector<uint8_t>::iterator end_iterator = _registers.end();
        // 当前处理位置的迭代器
        std::vector<uint8_t>::iterator current_iterator = _registers.begin();
        std::vector<uint8_t>::iterator previous_opcode =
            _registers.end();  // 指向下一个操作码的迭代器
        std::vector<uint8_t>::iterator next_opcode =
            _registers.end();  // 指向下一个操作码的迭代器
        int cur_index = 0;
        int next_index = -1;
        int prev_index = -1;
        int end_index = -1;

        long first_index = 0;  //  当前操作码涵盖的第一个寄存器的索引
        long span_length = 0;         // 当前操作码涵盖的寄存器数量
        bool is_zero_opcode = false;  // 当前是否处理的是ZERO操作码
        bool is_xzero_opcode = false;  // 当前是否处理的是XZERO操作码
        bool is_value_opcode = false;  // 当前是否处理的是VAL操作码
        long run_length = 0;           // 当前操作码的运行长度

        // 如果 count 大于 valMax的话表示无法存储下了
        // 需要替换成密集型存储
        if (count > ValMaxValue) {
            return HLLSetResult::PROMOTIONREQUIRED;
        }

        // 检查空间是否足够
        EnsureCapacity();

        // 更新迭代器
        end_iterator = _registers.end();
        previous_opcode = end_iterator;
        next_opcode = end_iterator;

        end_index = _registers.size() + 1;

        while (cur_index < end_index) {
            long opcode_length = 1;

            if (IsZeroOpcode(_registers.begin() + cur_index)) {
                span_length = GetZeroLength(_registers.begin() + cur_index);
            } else if (IsValueOpcode(_registers.begin() + cur_index)) {
                span_length = GetValueLength(_registers.begin() + cur_index);
            } else {
                span_length = GetXZeroLength(_registers.begin() + cur_index);
                opcode_length = 2;
            }

            // 如果 index 计算有误那么将终止，如果没问题是不可能符合这个条件的
            if (index <= first_index + span_length - 1) {
                break;
            }
            // previous_opcode = current_iterator;
            // current_iterator += opcode_length;
            prev_index = cur_index;
            cur_index += opcode_length;
            first_index += span_length;
        }

        // span一定不可能为0
        if (!span_length || cur_index >= end_index) {
            return HLLSetResult::INVALIDFORMAT;
        }

        next_index = IsXZeroOpcode(_registers.begin() + cur_index)
                         ? cur_index + 2
                         : cur_index + 1;

        if (next_index >= end_index) {
            next_index = end_index;
        }

        if (IsZeroOpcode(_registers.begin() + cur_index)) {
            is_zero_opcode = true;
            run_length = GetZeroLength(_registers.begin() + cur_index);
        } else if (IsXZeroOpcode(_registers.begin() + cur_index)) {
            is_xzero_opcode = true;
            run_length = GetXZeroLength(_registers.begin() + cur_index);
        } else {
            is_value_opcode = true;
            run_length = GetValueLength(_registers.begin() + cur_index);
        }

        // Step 2: 循环后的处理逻辑
        // 处理不同的情况以原地更新数据结构而不是从头开始生成
        // 如果是VAL操作码且已经设置了大于等于我们的计数值的值，则不需要更新。
        if (is_value_opcode) {
            current_count = GetValueFromOpcode(_registers.begin() + cur_index);

            // 不需要更新
            if (current_count >= count) {
                return HLLSetResult::SUCCESS;
            }

            // 需要更新
            if (run_length == 1) {
                SetSparseVal(_registers.begin() + cur_index, count, 1);

                MergeAdjacentValues(prev_index);
                return HLLSetResult::NOUPDATE;
            }
        }

        // 另一个易于处理的情况是长度为1的ZERO操作码。
        // 我们可以直接用我们的值和长度1替换它为VAL操作码。
        if (is_zero_opcode && run_length == 1) {
            SetSparseVal(_registers.begin() + cur_index, count, 1);

            MergeAdjacentValues(prev_index);
            return HLLSetResult::NOUPDATE;
        }

        std::vector<uint8_t> new_sequence(5);
        auto sequence_iterator = new_sequence.begin();

        int last = first_index + span_length - 1;
        long length = 0;

        if (is_zero_opcode || is_xzero_opcode) {
            if (index != first_index) {
                length = index - first_index;
                if (length > ZeroMaxLen) {
                    SetSparseXZero(sequence_iterator, length);
                    sequence_iterator += 2;
                } else {
                    SetSparseZero(sequence_iterator, length);
                    sequence_iterator++;
                }
            }

            SetSparseVal(sequence_iterator, count, 1);
            sequence_iterator++;

            if (index != last) {
                length = last - index;
                if (length > ZeroMaxLen) {
                    SetSparseXZero(sequence_iterator, length);
                    sequence_iterator += 2;
                } else {
                    SetSparseZero(sequence_iterator, length);
                    sequence_iterator++;
                }
            }
        } else {
            int cur_val = GetValueFromOpcode(_registers.begin() + cur_index);

            if (index != first_index) {
                length = index - first_index;
                SetSparseVal(sequence_iterator, cur_val, length);
                sequence_iterator++;
            }

            SetSparseVal(sequence_iterator, count, 1);
            sequence_iterator++;
            if (index != last) {
                length = last - index;
                SetSparseVal(sequence_iterator, cur_val, length);
                sequence_iterator++;
            }
        }

        // 第三步：用新序列替换旧序列。
        size_t seq_len = std::distance(new_sequence.begin(), sequence_iterator);
        size_t old_len = is_xzero_opcode ? 2 : 1;
        int delta_len = static_cast<int>(seq_len) - static_cast<int>(old_len);

        // 如果长度变化后超出稀疏表示的最大字节数限制，则执行升级逻辑
        if (delta_len > 0 &&
            _registers.size() + delta_len > MaxBytesForSparseRepresentation) {
            return HLLSetResult::PROMOTIONREQUIRED;
        }

        // _registers.resize(_registers.size() + delta_len);
        // 移动旧序列后的数据以为新序列腾出空间
        // if (delta_len && next_opcode != end_iterator) {
        //     std::move_backward(next_opcode, end_iterator,
        //                        next_opcode + delta_len);
        // }

        // std::copy(new_sequence.begin(), new_sequence.begin() + delta_len,
        //           current_iterator);

        auto it_tmp = _registers.begin() + cur_index;
        for (int i = 0; i < seq_len; i++) {
            it_tmp = _registers.insert(it_tmp, new_sequence[i]);
            it_tmp++;
        }

        end_iterator = _registers.end();

        MergeAdjacentValues(prev_index);

        return HLLSetResult::NOUPDATE;
    }
    //----------------------------
    // 稀疏型的相关函数
    // 设置 Val 操作码
    inline void SetSparseVal(std::vector<uint8_t>::iterator iter, uint8_t val,
                             int len) {
        // 把val设置进对应的位置，同时设置 val 标识
        *iter = ((val)-1) << 2 | ((len - 1)) | ValueOpcodeBit;
    }

    inline void SetSparseZero(std::vector<uint8_t>::iterator iter, int len) {
        *iter = len - 1;
    }

    void MergeAdjacentValues(int cur_index) {
        auto cur_it = cur_index != -1 ? _registers.begin() + cur_index
                                      : _registers.begin();
        int scan_len = 5;

        while (cur_it < _registers.end() && scan_len--) {
            if (IsXZeroOpcode(cur_it)) {
                cur_it += 2;
                continue;
            } else if (IsZeroOpcode(cur_it)) {
                cur_it++;
                continue;
            }

            // 检查是否有两个相邻的val
            auto next_it = cur_it + 1;
            if (next_it < _registers.end() && IsValueOpcode(next_it)) {
                int val_cur = GetValueFromOpcode(cur_it);
                int val_next = GetValueFromOpcode(next_it);

                // 如果相等说明需要合并成为一个
                if (val_cur == val_next) {
                    int len =
                        GetValueLength(cur_it) + GetValueLength(cur_it + 1);

                    if (len <= ValMaxLen) {
                        SetSparseVal(next_it + 1, val_cur, len);
                        cur_it = _registers.erase(
                            cur_it);  // 删除当前操作码，并向左合并
                                      // 合并后重新评估合并可能性，而不是简单地移动到下一个操作码
                        continue;
                    }
                }
            }

            cur_it++;
        }

        // 设置缓存无效
        MarkCacheAsInvalid();
    }

    inline void SetSparseXZero(std::vector<uint8_t>::iterator iter, int len) {
        len--;
        // 设置第一个字节并设置 xzero 标识
        *iter = len >> 8 | XZeroOpcodeBit;
        // 设置第二个字节
        *(iter + 1) = len & 0xff;
    }

    // 检查操作码是否表示一个零序列
    inline bool IsZeroOpcode(std::vector<uint8_t>::const_iterator iter) {
        return ((*iter) & 0xc0) == 0;  // 检查最高两位是否为00，表示一个零序列
    }

    // 检查操作码是否表示一个扩展的零序列
    inline bool IsXZeroOpcode(std::vector<uint8_t>::const_iterator iter) {
        return ((*iter) & 0xc0) ==
               XZeroOpcodeBit;  // 检查是否为扩展零序列的操作码
    }

    // 检查操作码是否表示一个 val
    inline bool IsValueOpcode(std::vector<uint8_t>::const_iterator iter) {
        return ((*iter) & ValueOpcodeBit) !=
               0;  // 检查最高位是否为1，表示一个值操作码
    }

    // ZERO操作码中获取零序列的长度
    inline long GetZeroLength(std::vector<uint8_t>::const_iterator iter) {
        return ((*iter) & 0x3f) +
               1;  // 获取操作码的低6位，左移8位后与下一个字节组合，再加1，表示扩展零序列的长度
    }

    // 从XZERO操作码中获取扩展零序列的长度
    inline long GetXZeroLength(std::vector<uint8_t>::const_iterator iter) {
        return ((((*iter) & 0x3f) << 8) | *(std::next(iter))) +
               1;  // 获取操作码的低6位，左移8位后与下一个字节组合，再加1，表示扩展零序列的长度
    }

    // 从VAL操作码中获取值
    inline long GetValueFromOpcode(std::vector<uint8_t>::const_iterator iter) {
        return (((*iter) >> 2) & 0x1f) +
               1;  // 获取操作码的第3到第7位并加1，表示值
    }

    // 从val操作码中获取长度
    inline long GetValueLength(std::vector<uint8_t>::const_iterator iter) {
        return ((*iter) & 0x3) + 1;  // 获取操作码的低2位并加1，表示值的序列长度
    }

    //----------------------------

    // 密集型相关函数
    //------------------------
    // 获取指定寄存器的值
    unsigned long GetRegisterValue(std::vector<uint8_t>::const_iterator iter,
                                   unsigned long register_index) {
        auto tmp_iter = iter;
        std::advance(tmp_iter,
                     register_index * BitCount / 8);  // 移动到正确的字节位置
        unsigned long byte_offset = register_index * BitCount % 8;
        unsigned long next_byte_offset = 8 - byte_offset;

        unsigned long value = *tmp_iter;
        value |= static_cast<unsigned long>(*(std::next(tmp_iter, 1)))
                 << 8;  // 获取下一个字节的值
        value = (value >> byte_offset) & RegisterMax;  // 提取正确的寄存器值

        return value;
    }

    // 设置指定寄存器的值
    void SetRegisterValue(std::vector<uint8_t>::iterator iter,
                          unsigned long register_index, unsigned long value) {
        auto tmp_iter = iter;
        std::advance(tmp_iter,
                     register_index * BitCount / 8);  // 移动到正确的字节位置
        unsigned long byte_offset = register_index * BitCount % 8;
        unsigned long next_byte_offset = 8 - byte_offset;

        // 清除当前字节和下一个字节中对应的旧寄存器值
        *tmp_iter &= ~(RegisterMax << byte_offset);
        *(std::next(tmp_iter, 1)) &= ~(RegisterMax >> next_byte_offset);

        // 设置新的寄存器值
        *tmp_iter |= (value & RegisterMax) << byte_offset;
        *(std::next(tmp_iter, 1)) |= (value & RegisterMax) >> next_byte_offset;
    }
    //------------------------

   private:
    HLLHeader _head;
    std::vector<uint8_t> _registers;
};
#endif  // !__HYPER_LOG_LOG_HPP__

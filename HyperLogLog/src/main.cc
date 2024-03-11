/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2024-03-10 22:38:55
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2024-03-11 17:59:31
 * @FilePath: /HyperLogLog/src/main.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "HyperLogLog.hpp"

int main() {
    HyperLogLog hll;

    std::string str = "a";

    for (int i = 1; i < 58000000; i++) {
        hll.Add(str + std::to_string(i));
    }
    // hll.Add("0");
    // hll.Add("1");
    // hll.Add("2");
    // hll.Add("3");
    hll.Add(str + std::to_string(280));

    int count = hll.count();

    std::cout << "Count: " << count << std::endl;

    return 0;
}

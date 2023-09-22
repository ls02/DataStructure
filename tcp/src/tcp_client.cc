/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-22 17:09:52
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-22 17:10:56
 * @FilePath: /DataStructure/tcp/src/tcp_client.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "tcp_client.hpp"
#include <memory>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << argv[0] << " ip "
                  << " port" << std::endl;
        exit(1);
    }

    std::shared_ptr<TcpClient> client_ptr = std::make_shared<TcpClient>(argv[1], atoi(argv[2]));
    client_ptr->Start();

    return 0;
}
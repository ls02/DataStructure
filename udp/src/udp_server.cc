/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-12 16:54:54
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-19 21:38:49
 * @FilePath: /DataStructure/udp/src/udp_server.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "udp_server.hpp"
#include <memory>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;

        return 1;
    }

    int port = atoi(argv[1]);

    std::shared_ptr<UdpServer> ptr_server = std::make_shared<UdpServer>(port);
    ptr_server->init();
    ptr_server->Start();

    return 0;
}
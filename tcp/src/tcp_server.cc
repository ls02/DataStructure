/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-22 16:03:21
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-22 17:27:43
 * @FilePath: /DataStructure/tcp/src/tcp_server.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "tcp_server.hpp"
#include <signal.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << argv[0] << " "
                  << "port" << std::endl;
        exit(1);
    }

    signal(SIGCHLD, SIG_IGN);

    std::shared_ptr<TcpServer>
        ptr_server = std::make_shared<TcpServer>(atoi(argv[1]));

    ptr_server->Init();
    ptr_server->Start();

    return 0;
}
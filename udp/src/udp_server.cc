/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-12 16:54:54
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-12 22:34:57
 * @FilePath: /DataStructure/udp/src/udp_server.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "socket.hpp"
#include <iostream>

// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         std::cout << "name port" << std::endl;

//         return 0;
//     }

//     int16_t port = atoi(argv[1]);
//     socke local;
//     // int socket_fd = local.Socket();
//     int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
//     local.Bind(socket_fd, port);

//     char buff[1024];
//     while (true)
//     {
//         struct sockaddr_in peer;
//         socklen_t len = sizeof(peer);

//         ssize_t cnt = recvfrom(socket_fd, buff, sizeof(buff) - 1, 0, (sockaddr *)&peer, &len);
//         if (cnt > 0)
//         {
//             buff[cnt] = 0;
//             std::cout << "client# " << buff << std::endl;
//         }

//         std::string echo_hello = "hello";

//         // sendto(socket_fd, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&peer, len);
//         sendto(socket_fd, echo_hello.c_str(), echo_hello.size(), 0, (struct sockaddr *)&peer, len);
//     }

//     return 0;
// }

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "name port" << std::endl;

        return 1;
    }

    // 构建套接字
    int socke_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socke_fd < 0)
    {
        std::cout << "Get socke_fd fail" << std::endl;

        return 2;
    }
    // 绑定套接字

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(atoi(argv[1]));

    if (bind(socke_fd, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        std::cout << "bind fail" << std::endl;

        return 3;
    }

    // 绑定成功执行业务代码
    // 1. 先接收客户端发来的消息
    // 2. 把客户端的消息发回去
    // 3. 继续循环业务
    while (true)
    {
        char buff[1024];
        struct sockaddr_in peer;
        socklen_t peer_len = sizeof(peer);

        int cnt = recvfrom(socke_fd, buff, sizeof(buff), 0, (struct sockaddr *)&peer, &peer_len);
        if (cnt > 0)
        {
            buff[cnt] = 0;
            std::string str(buff);
            std::cout << "client# " << buff << std::endl;
            // 同时业务发回去

            sendto(socke_fd, str.c_str(), str.size(), 0, (struct sockaddr *)&peer, peer_len);
        }
    }

    return 0;
}
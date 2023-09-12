/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-12 17:49:22
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-12 22:43:49
 * @FilePath: /DataStructure/udp/src/udp_clinet.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "socket.hpp"
#include <cstring>

// int main(int argc, char *argv[])
// {
//     if (argc != 3)
//     {
//         std::cout << "name ip port" << std::endl;

//         return 1;
//     }

//     socke local;
//     // int sock_fd = local.Socket();
//     int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

//     struct sockaddr_in server;
//     server.sin_family = AF_INET;
//     server.sin_port = htons(atoi(argv[2]));
//     server.sin_addr.s_addr = inet_addr(argv[1]);

//     while (true)
//     {
//         std::cout << "clinet# ";
//         char line[1024];
//         fgets(line, sizeof(line), stdin);

//         sendto(sock_fd, line, strlen(line), 0, (struct sockaddr *)&server, sizeof(server));

//         // 接收服务器的消息
//         struct sockaddr_in peer;
//         socklen_t len = sizeof(peer);

//         char buff[1024];
//         ssize_t cnt = recvfrom(sock_fd, buff, sizeof(buff), 0, (struct sockaddr *)&peer, &len);
//         if (cnt > 0)
//         {
//             buff[cnt] = 0;
//             std::cout << buff << std::endl;
//         }
//     }

//     return 0;
// }

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "name ip port" << std::endl;

        return 1;
    }

    // 获取套接字
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        std::cout << "Get socket fd fail" << std::endl;

        return 2;
    }

    // clinet 不需要绑定端口
    // 执行业务逻辑代码，但是你需要把自己的IP Port 填充进去
    // 告诉计算机你的信息要发给谁

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    // 把本地的发送给对方，再接收对方发来的数据
    while (true)
    {
        char line[1024];
        std::cout << "client#  ";
        fgets(line, sizeof(line), stdin);
        line[strlen(line) - 1] = 0;

        sendto(socket_fd, line, strlen(line), 0, (struct sockaddr *)&server, sizeof(server));

        //  接收对方发来的消息
        char buff[1024];
        struct sockaddr_in peer;
        socklen_t peer_len = sizeof(peer);
        int cnt = recvfrom(socket_fd, buff, sizeof(buff), 0, (struct sockaddr *)&peer, &peer_len);
        if (cnt > 0)
        {
            buff[cnt] = 0;

            std::cout << "server# " << buff << std::endl;
        }
    }

    return 0;
}
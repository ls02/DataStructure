/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-12 16:54:54
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-18 23:03:19
 * @FilePath: /DataStructure/udp/src/udp_server.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "socket.hpp"
#include <iostream>
#include <unordered_map>
#include <string>

int main(int argc, char *argv[])
{
    std::unordered_map<std::string, struct sockaddr_in> m;
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
            int16_t port = ntohs(peer.sin_port);
            std::string messg_str(inet_ntoa(peer.sin_addr));
            std::string addrs_str = messg_str;
            addrs_str += ":";
            addrs_str += std::to_string(port);

            if (!m.count(addrs_str))
            {
                m.insert({addrs_str, peer});
            }

            buff[cnt] = 0;

            std::string str(buff);
            std::cout << "[ " << messg_str << ":" << port << " ]"
                      << "client# " << buff << std::endl;

            // 同时业务发回去
            // sendto(socke_fd, str.c_str(), str.size(), 0, (struct sockaddr *)&peer, peer_len);
            auto it = m.begin();
            while (it != m.end())
            {
                struct sockaddr_in peer_tmp = it->second;
                // socklen_t peer_len_tmp = sizeof(peer_tmp);
                // peer_tmp = it->second;

                // sendto(socke_fd, str.c_str(), str.size(), 0, (struct sockaddr *)&peer_tmp, peer_len_tmp);
                sendto(socke_fd, str.c_str(), str.size(), 0, (struct sockaddr *)&peer_tmp, sizeof(peer_tmp));
            }
        }
    }

    return 0;
}
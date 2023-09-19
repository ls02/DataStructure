/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-19 19:37:32
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-19 22:46:04
 * @FilePath: /DataStructure/udp/src/udp_server.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

class UdpServer
{
public:
    UdpServer(int port = PORT) : _port(port), _sock_fd(-1)
    {
    }

    bool init()
    {
        _sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sock_fd < 0)
        {
            std::cerr << "socket fail" << std::endl;

            return false;
        }

        std::cout << "socket create success, sock_fd: " << _sock_fd << std::endl;

        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(_sock_fd, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind fail " << std::endl;

            return false;
        }

        std::cout << "bind success" << std::endl;

        return true;
    }

    void Start()
    {
        char buff[1024];
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t peer_len = sizeof(peer);
            ssize_t size = recvfrom(_sock_fd, buff, sizeof(buff) - 1, 0, (struct sockaddr *)&peer, &peer_len);
            if (size > 0)
            {
                buff[size] = 0;
                // 1.提取ip
                char ip[1024];
                snprintf(ip, sizeof(ip) - 1, "%s:%u", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
                std::string addr(ip);
                std::string messg(addr);
                messg += "# ";
                messg += buff;

                // 检查这个ip是否存在
                if (!_user.count(addr))
                {
                    // _user.insert(std::make_pair(addr, peer));
                    _user.insert({addr, peer});
                }

                // 给所有人发送消息
                for (auto &iter : _user)
                {
                    sendto(_sock_fd, messg.c_str(), messg.size(), 0, (struct sockaddr *)&iter.second, sizeof(iter.second));
                }

                std::cout << messg << std::endl;
            }
        }
    }

private:
    int _port;
    int _sock_fd;
    std::unordered_map<std::string, struct sockaddr_in> _user;
};
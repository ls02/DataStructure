/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-12 14:39:04
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-18 21:54:39
 * @FilePath: /DataStructure/udp/src/socket.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <iostream>
#include <string>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class socke
{
public:
    int Socket()
    {
        return socket(AF_INET, SOCK_DGRAM, 0);
    }

    void Bind(int fd, int16_t port)
    {
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(fd, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind fail" << std::endl;
        }
    }
};

#endif
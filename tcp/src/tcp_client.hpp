/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-22 16:06:54
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-22 17:17:28
 * @FilePath: /DataStructure/tcp/src/tcp_client.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 *
 */
#ifndef __TCP_CLIENT_HPP__
#define __TCP_CLIENT_HPP__

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.hpp"

class TcpClient
{
public:
    TcpClient(std::string ip, int16_t port) : _ip(ip), _port(port)
    {
    }

    int Socket()
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            logMessage(FATAL, "socket fail, %d:%s", errno, strerror(errno));

            return -1;
        }

        logMessage(NORMAL, "Socket Success");

        return sock;
    }

    bool Connect(int sock)
    {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(_port);
        server.sin_addr.s_addr = inet_addr(_ip.c_str());

        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            logMessage(FATAL, "connect fail, %d:%s", errno, strerror(errno));

            return false;
        }

        logMessage(NORMAL, "coonect success");

        return true;
    }

    void Start()
    {
        int sock = 0;
        std::string line;
        bool flags = false;
        while (true)
        {
            if (!flags)
            {
                sock = Socket();
                if (sock < 0)
                {
                    exit(1);
                }

                if (!Connect(sock))
                {
                    exit(2);
                }

                flags = true;
            }

            std::cout << "请输入# ";
            std::getline(std::cin, line);
            if (line == "quit")
            {
                break;
            }

            // 发送数据
            ssize_t ret = write(sock, line.c_str(), line.size());
            if (ret > 0)
            {
                char buff[1024]{0};

                ssize_t ret = read(sock, buff, sizeof(buff) - 1);
                if (ret > 0)
                {
                    buff[ret] = 0;
                    std::cout << "server# " << buff << std::endl;
                }
                else if (ret == 0)
                {
                    flags = false;
                    close(sock);
                }
            }
            else
            {
                flags = false;
                close(sock);
            }
        }
    }

private:
    std::string _ip;
    int16_t _port;
};

#endif
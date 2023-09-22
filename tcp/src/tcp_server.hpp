/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-22 12:42:50
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-22 17:32:53
 * @FilePath: /DataStructure/tcp/src/tcp_server.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include "log.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <memory>
#include <unistd.h>
#include <stdlib.h>

static int gbacklog = 5;

class TcpServer
{
public:
    TcpServer(uint16_t port, std::string ip = "0.0.0.0") : _ip(ip), _port(port), _listen_sock(-1)
    {
    }

private:
    void Socket()
    {
        _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listen_sock < 0)
        {
            logMessage(FATAL, "create socket error, %d:%s", errno, strerror(errno));
            exit(2);
        }

        logMessage(NORMAL, "create socket success");
    }

    void Bind()
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_port = htons(_port);
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
        // inet_pton(AF_INET, _ip.c_str(), &local.sin_addr);

        if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind error, %d:%s", errno, strerror(errno));
            exit(3);
        }
    }

    void Listen()
    {
        if (listen(_listen_sock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
        }
    }

public:
    void Init()
    {
        Socket();
        Bind();
        Listen();
    }

    // 套接字完成，这里完成发送数据和接收数据
    void Start()
    {
        // 死循环的接收和发送，因为是service所以不需要退出
        while (true)
        {
            // 任务： 接受数据同时把数据发送回去
            // 1. 先接受数据

            char buff[1024]{0};
            struct sockaddr_in peer;
            socklen_t peer_len = sizeof(peer);

            int sock = accept(_listen_sock, (struct sockaddr *)&peer, &peer_len);
            if (sock < 0)
            {
                logMessage(FATAL, "accept fail, %d:%s", errno, strerror(errno));

                continue;
            }

            pid_t pid = fork();
            if (pid < 0)
            {
                logMessage(FATAL, "fork fail, %d:%s", errno, strerror(errno));
                exit(3);
            }
            else if (pid == 0)
            {
                // 子进程来完成这些操作
                // 接收到数据发送回去
                // 发送回去需要构建自己的“网络套接字”
                std::string addr = inet_ntoa(peer.sin_addr);
                addr += ":";
                addr += std::to_string(ntohs(peer.sin_port));

                logMessage(NORMAL, "link success, servicesock: %d | %s |\n",
                           sock, addr.c_str());

                while (true)
                {
                    // 接收对端的数据
                    int ret = read(sock, buff, sizeof(buff));
                    if (ret > 0)
                    {
                        // 把数据取出来
                        buff[ret] = 0;
                        // 取出对端ip，端口放入新的 str 内
                        std::cout << "client# " << buff << std::endl;

                        std::string mesg_str("[ " + addr + "] # ");
                        ret = write(sock, mesg_str.c_str(), mesg_str.size());
                        if (ret < 0)
                        {
                            logMessage(WARNING, "write fail: %d:%s", errno, strerror(errno));
                            close(sock);
                        }
                        else if (ret == 0)
                        {
                            logMessage(NORMAL, "peer close, quit ...");
                            close(sock);
                        }
                        else
                        {
                            logMessage(NORMAL, "write success!");
                        }
                    }
                }
            }
        }
    }

    ~TcpServer()
    {
        close(_listen_sock);
    }

private:
    std::string _ip;
    int _listen_sock;
    uint16_t _port;
};

#endif
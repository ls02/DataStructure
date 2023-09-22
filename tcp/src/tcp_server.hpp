/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-22 12:42:50
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-22 13:21:03
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

static int gbacklog = 5;

class TcpServer
{
public:
    TcpServer(uint16_t port, std::string ip = "0.0.0.0") : _ip(ip), _port(port), _listen_sock(-1), _fd(-1)
    {
    }

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
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
            exit(3);
        }
    }

    void Listen()
    {
        _fd = listen(_listen_sock, gbacklog);
        if (_fd < 0)
        {
        }
    }

    void Init()
    {
    }

    void Start()
    {
    }

private:
    std::string _ip;
    int _listen_sock;
    uint16_t _port;
    int _fd;
};

#endif
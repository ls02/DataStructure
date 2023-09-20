#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <unistd.h>
#include "Thread.hpp"

struct ClientData
{
    int _sock_fd;
    int _server_port;
    std::string _server_ip;

    ClientData(std::string ip, int port) : _server_ip(ip), _server_port(port), _sock_fd(-1)
    {
    }

    ~ClientData()
    {
        close(_sock_fd);
    }
};

class UdpClient
{
public:
    UdpClient(std::string ip, int port) : _data(ip, port)
    {
    }

    bool initClient()
    {
        _data._sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_data._sock_fd < 0)
        {
            std::cerr << "socket fail" << std::endl;

            return false;
        }

        return true;
    }

    static void *ClientSend(void *args)
    {
        ClientData data = *(ClientData *)((Thread::ThreadData *)args)->_args;
        struct sockaddr_in server;
        std::string message;
        memset(&server, 0, sizeof(server));

        server.sin_family = AF_INET;
        server.sin_port = htons(data._server_port);
        server.sin_addr.s_addr = inet_addr(data._server_ip.c_str());

        while (true)
        {
            std::cerr << "请输入信息：";
            std::getline(std::cin, message);
            if (message == "quit")
            {
                break;
            }

            if (sendto(data._sock_fd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server)) == -1)
            {
                std::cerr << "code: " << errno << " error: " << strerror(errno) << std::endl;
            }
        }
    }

    static void *ClientRecver(void *args)
    {
        ClientData data = *(ClientData *)((Thread::ThreadData *)args)->_args;
        char buff[1024];
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t peer_len = sizeof(peer);

            ssize_t size = recvfrom(data._sock_fd, buff, sizeof(buff) - 1, 0, (struct sockaddr *)&peer, &peer_len);
            if (size > 0)
            {
                buff[size] = 0;

                std::cout << buff << std::endl;
            }
        }
    }

    void Start()
    {
        // 让两个线程去读取和写入
        // std::unique_ptr<Thread::Thread> sender = std::make_unique<Thread::Thread>(1, ClientSend, (void *)&_data);
        // std::unique_ptr<Thread::Thread> recver = std::make_unique<Thread::Thread>(2, ClientRecver, (void *)&_data);
        std::unique_ptr<Thread::Thread> sender = std::unique_ptr<Thread::Thread>(new Thread::Thread(1, ClientSend, (void *)&_data));
        std::unique_ptr<Thread::Thread> recver = std::unique_ptr<Thread::Thread>(new Thread::Thread(2, ClientRecver, (void *)&_data));

        sender->Start();
        recver->Start();

        sender->Join();
        recver->Join();
    }

private:
    ClientData _data;
};

#include "udp_clinet.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << "server_ip server_port" << std::endl;

        return 1;
    }

    std::shared_ptr<UdpClient> client_ptr = std::make_shared<UdpClient>(argv[1], atoi(argv[2]));

    client_ptr->initClient();
    client_ptr->Start();

    return 0;
}
CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

add_compile_options(-std=c++11 -lpthread -g)


ADD_EXECUTABLE(client_udp udp_clinet.cc)
ADD_EXECUTABLE(server_udp udp_server.cc)

target_link_libraries (
    server_udp 
    pthread
)

target_link_libraries (
    client_udp
    pthread
)
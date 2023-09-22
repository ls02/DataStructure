CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

add_compile_options(-std=c++11 -lpthread -g)

ADD_EXECUTABLE(client_tcp tcp_client.cc)
ADD_EXECUTABLE(server_tcp tcp_server.cc)

target_link_libraries (
    client_tcp
    pthread
)

target_link_libraries (
    server_tcp
    pthread
)
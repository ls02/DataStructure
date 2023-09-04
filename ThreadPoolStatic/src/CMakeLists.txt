CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

add_compile_options(-std=c++11 -lpthread -g)

SET(List main.cc)

ADD_EXECUTABLE(run ${List})

target_link_Libraries(
    run 
    pthread
)
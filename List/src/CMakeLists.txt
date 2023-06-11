CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

add_compile_options(-std=c++11 -g)

SET(LIST test1.cc)

ADD_EXECUTABLE(run_list ${LIST})
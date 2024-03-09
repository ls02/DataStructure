cmake_minimum_required(VERSION 3.20)

add_compile_options(-std=c++11 -g -O0)

set(list main.cc)

add_executable(run ${list})

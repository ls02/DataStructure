CMAKE_MINIMUM_REQUIRED(VERSION 3.26)
PROJECT(HEAP CXX)

add_compile_options(-std=c++11 -g)

SET(SRC_LIST test.cc)

MESSAGE(STATUS "This is BINARY dir " ${HEAP_BINARY_DIR})

MESSAGE(STATUS "This is SOURCE dir " ${HEAP_SOURCE_DIR})

ADD_EXECUTABLE(myproc ${SRC_LIST})

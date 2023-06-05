CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

add_compile_options(-std=c++11 -g)

ADD_EXECUTABLE(un_string unit_string.cc)

target_link_libraries(
    un_string 
    gtest 
    #gmock
    )

include(GoogleTest)

gtest_discover_tests(un_string)


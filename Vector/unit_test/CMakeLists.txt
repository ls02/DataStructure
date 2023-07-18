CMAKE_MINIMUM_REQUIRED(VERSION 3.27)

add_compile_options(-std=c++11 -g)

ADD_EXECUTABLE(ut unit_vector.cc)

target_link_libraries (
    ut
    gtest 
)

include(GoogleTest)

gtest_discover_tests(ut)
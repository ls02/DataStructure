/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-15 17:57:36
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-17 16:20:55
 * @FilePath: /Vector/src/test.cc
 * @Description:
 *
 * Copyright (c) 2023 by ls02, All Rights Reserved.
 */
#include <iostream>
#include "MyVector.hpp"

int main()
{
    ls::vector<int> v1;

    v1.push_back(1);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std::endl;
    v1.push_back(2);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std::endl;
    v1.push_back(3);
    v1.push_back(4);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std::endl;
    v1.push_back(5);
    v1.push_back(6);
    v1.push_back(7);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std::endl;
    v1.push_back(8);
    std::cout << "capacity: " << v1.capacity() << " size: " << v1.size() << std::endl;

    v1.Printf();

    v1.pop_back();
    v1.pop_back();
    v1.pop_back();
    v1.pop_back();
    v1.pop_back();

    std::cout << v1 << std::endl;

    ls::vector<int> v2;
    v2.push_back(8);
    v2.push_back(7);
    v2.push_back(6);
    v2.push_back(5);

    std::cout << v2 << std::endl;

    std::cout << "===========" << std::endl;

    v1.swap(v2);

    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;

    ls::vector<int> v3;

    v3 = v1;

    std::cout << v3 << std::endl;

    ls::vector<int> v4(v3.begin(), v3.end());

    std::cout << v4 << std::endl;

    return 0;
}
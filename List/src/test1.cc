/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-24 10:26:10
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-24 10:26:10
 * @FilePath: /List/src/test1.cc
 * @Description: 
 * 
 * Copyright (c) 2023 by ls02, All Rights Reserved. 
 */
#include <iostream>
#include "List.hpp"

void Test1()
{
    ls::list<int> l1;
    l1.push_back(1);
    l1.push_back(2);

    l1.Print();
}

void Test2()
{
    ls::list<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);

    ls::list<int>::iterator it = l1.begin();

    while (it != l1.end())
    {
        std::cout << *it << " ";
        ++it;
    }

    printf("\n");
}

int main()
{
    // Test1();
    Test2();

    return 0;
}
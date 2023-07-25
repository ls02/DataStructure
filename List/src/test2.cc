/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-24 09:52:45
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-25 14:20:14
 * @FilePath: /DataStructure/List/src/test2.cc
 * @Description:
 *
 * Copyright (c) 2023 by ls02, All Rights Reserved.
 */
#include <iostream>
#include "List.hpp"

void Func(const ls::list<int> &l)
{
    ls::list<int>::const_iterator it = l.begin();
    while (it != l.end())
    {
        std::cout << *it << std::endl;

        it++;
    }
}

void Test1()
{
    ls::list<int> l;

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);
    l.push_back(5);
    l.push_back(6);
    l.push_back(7);
    l.push_back(8);
    l.push_back(9);

    l.print();

    const ls::list<int> l2(l);
    ls::list<int> l3;

    l3 = l;
    l2.print();

    Func(l2);

    l.erase(l.begin());

    l.print();

    ls::list<int>::iterator it = l.begin();
    while (it!= l.end())
    {
        if (*it == 5)
        {
            break;
        }

        it++;
    }

    std::cout << *l.erase(l.begin(), it) << std::endl;

    l.print();

    std::cout << "====================" << std::endl;

    std::cout << "l1:";
    l.print();

    std::cout << "l3: ";
    l3.print();
    std::cout << "====================" << std::endl;
    l.swap(l3);
    std::cout << "l1:";
    l.print();

    std::cout << "l3: ";
    l3.print();

    ls::list<int> l4(l);
        ls::list<int>::iterator it1 = l4.begin();

    std::cout << "l4: ";

    ls::list<int>::reverse_iterator rit = l4.rbegin();

    while (rit != l4.rend())
    {
        std::cout << *rit << " ";
        ++rit;
    }

    printf("\n");

}

int main()
{
    Test1();

    return 0;
}
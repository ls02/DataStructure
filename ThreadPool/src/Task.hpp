/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-03 16:27:46
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-03 16:55:51
 * @FilePath: /DataStructure/ThreadPool/src/Task.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <iostream>
#include <functional>
#include <string>

typedef std::function<int(int, int)> func_t;

class Task
{
public:
    Task() {}
    Task(int x, int y, func_t func) : _x(x), _y(y), _func(func)
    {
    }

    void operator()(const std::string &name)
    {
        std::cout << "线程 " << name << " 处理完成， 结果是： " << _x << " + " << _y << " = " << _func(_x, _y) << std::endl;
    }

private:
    int _x;
    int _y;
    func_t _func;
};

#endif
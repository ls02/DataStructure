/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-04 10:57:10
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-04 11:18:25
 * @FilePath: /DataStructure/ThreadPoolStatic/src/Task.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <iostream>
#include <functional>
#include <string>

namespace Task
{
typedef std::function<int(int, int)> func_t;

class Task
{
    public:
    Task()
    {}

    Task(int x, int y, func_t func):_x(x), _y(y), _func(func)
    {}

    void operator()(std::string name)
    {
        std::cout << name << " 任务处理结果： " <<  _x << " + " << _y << " = " << _func(_x, _y) << std::endl;
    }

    private:
    int _x;
    int _y;
    func_t _func;
};

}

#endif
/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-01 15:41:43
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-03 15:49:00
 * @FilePath: /DataStructure/ThreadPool/src/Thread.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __THREAD_HPP__
#define __THREAD_HPP__
#include <iostream>
#include <pthread.h>
#include <string>

typedef void *(*fun_t)(void *);

class ThreadData
{
public:
    void *_args;
    std::string _name;
};

class Thread
{
public:
    Thread(int num, fun_t callback, void *args) : _func(callback)
    {
        char buff[64];
        snprintf(buff, sizeof(buff), "Thread-%d", num);
        _name = buff;

        _data._name = _name;
        _data._args = args;
    }

    void Start()
    {
        pthread_create(&_id, nullptr, _func, (void *)&_data);
    }

    void Join()
    {
        pthread_join(_id, nullptr);
    }

    std::string GetName()
    {
        return _name;
    }

    ~Thread()
    {
    }

private:
    pthread_t _id;
    fun_t _func;
    ThreadData _data;
    std::string _name;
};

#endif
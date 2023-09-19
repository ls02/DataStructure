/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-04 10:28:36
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-19 22:39:58
 * @FilePath: /DataStructure/ThreadPoolStatic/src/Thread.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __THREAD_HPP__
#define __THREAD_HPP__
#include <iostream>
#include <pthread.h>
#include <string>

namespace Thread
{
    typedef void *(*func_t)(void *);

    class ThreadData
    {
    public:
        std::string _name;
        void *_args;
    };

    class Thread
    {
    public:
        Thread(int num, func_t callback, void *args) : _func(callback)
        {
            char buff[64];

            snprintf(buff, sizeof(buff), "Thread - %d 线程", num);
            _name = buff;

            _data._args = args;
            _data._name = _name;
        }

        void Start()
        {
            pthread_create(&_tid, nullptr, _func, (void *)&_data);
        }

        void Join()
        {
            pthread_join(_tid, nullptr);
        }

    private:
        ThreadData _data;
        pthread_t _tid;
        func_t _func;
        std::string _name;
    };
}
#endif
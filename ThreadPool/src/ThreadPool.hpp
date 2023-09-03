/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-01 15:19:21
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-03 16:51:04
 * @FilePath: /DataStructure/ThreadPool/src/ThreadPool.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__
#include "Thread.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include "guard.hpp"

#define THREAD_NUM 32

template <class T>
class ThreadPool
{
public:
    ThreadPool(int num = THREAD_NUM)
        : _num(num)
    {
        for (int i = 0; i < _num; i++)
        {
            _thread_queue.push_back(new Thread(i, routine, this));
        }
        pthread_cond_init(&_cond, nullptr);
        pthread_mutex_init(&_mtx, nullptr);
    }

    pthread_mutex_t *GetMutex()
    {
        return &_mtx;
    }

    bool IsEmpty()
    {
        return _task_queue.empty();
    }

    void WaitCond()
    {
        pthread_cond_wait(&_cond, &_mtx);
    }

    T GetTask()
    {
        T t = _task_queue.front();
        _task_queue.pop();

        return t;
    }

    static void *routine(void *args)
    {
        ThreadData *td = (ThreadData *)args;
        ThreadPool<T> *tp = (ThreadPool<T> *)td->_args;

        while (true)
        {
            T task;
            {
                LockGurad lock_gurad(tp->GetMutex());

                while (tp->IsEmpty())
                {
                    tp->WaitCond();
                }

                // 取任务
                task = tp->GetTask();
            }
            // 处理任务
            task(td->_name);
        }
    }

    void PushTask(const T &task)
    {
        LockGurad lock_mtx(&_mtx);
        _task_queue.push(task);
        pthread_cond_signal(&_cond);
    }

    void run()
    {
        for (auto &e : _thread_queue)
        {
            e->Start();
        }
    }

    ~ThreadPool()
    {
        for (auto &iter : _thread_queue)
        {
            iter->Join();
            delete iter;
        }

        pthread_mutex_destroy(&_mtx);
    }

private:
    std::vector<Thread *> _thread_queue;
    std::queue<T> _task_queue;
    size_t _num;
    pthread_cond_t _cond;
    pthread_mutex_t _mtx;
};

#endif
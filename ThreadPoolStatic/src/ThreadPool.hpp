/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-04 10:46:54
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-04 16:24:10
 * @FilePath: /DataStructure/ThreadPoolStatic/src/ThreadPool.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __THREAD_POOL_HPP__
#define __THREAD_POLL_HPP__
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Thread.hpp"
#include "LockGuard.hpp"

namespace ThreadPool
{
size_t g_thread_num = 32;

template<class T>
class ThreadPool
{
    public:
    ThreadPool(size_t num = g_thread_num):_num(num)
    {
        for (int i = 0; i < _num; i++)
        {
            _threads.push_back(new Thread::Thread(i, routine, this));
        }

        // _thread_bash = new THread::Thread(0, ThreadBash, this);

        pthread_mutex_init(&_mtx, nullptr);
        pthread_cond_init(&_cond, nullptr);
        _ptr_task_pthread = &_task_a;
        _ptr_task = &_task_b;
    }

    static void* routine(void* args)
    {
        Thread::ThreadData* td = (Thread::ThreadData*)args;
        ThreadPool* tp = (ThreadPool*)td->_args;
        while (true)
        {
            T task;
            {
                LockGuard lock(tp->GetMutex());

                while (tp->IsEmpty())
                {
                    tp->WaitCond();
                }

                // 如果不为空获取任务，为空就什么都不做
                // 获取任务
                task = tp->GetTask();

                // 给派送线程发信号
            }

            //处理任务
            task(td->_name);
        }
    }

    // static void* ThreadBash(void* args)
    // {
        // while (true)
        // {
            // 1. 直接阻塞等待信号
            // 1.2 检查是否需要交换队列

            // 2. 给线程队列发送信号
        // }
    // }

    void run()
    {
        for (auto &iter : _threads)
        {
            iter->Start();
        }

        // _thread_bash->Start();
    }

    void PushTask(const T& task)
    {
        LockGuard lock(&_mtx);
        _ptr_task->push(task);

        if (_ptr_task->size() > 5 && _ptr_task_pthread->empty())
        {
            Swap(_ptr_task_pthread, _ptr_task);
            pthread_cond_signal(&_cond);
        }
    }

    void Swap(std::queue<T>* &t1, std::queue<T>* &t2)
    {
        std::queue<T>* tmp = t1;
        t1 = t2;
        t2 = tmp;
    }

    ~ThreadPool()
    {
        for (auto &iter : _threads)
        {
            iter->Join();

            delete iter;
        }

        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_cond);
    }

    public:
    pthread_mutex_t* GetMutex()
    {
        return &_mtx;
    }

    pthread_cond_t* GetCond()
    {
        return &_cond;
    }

    void WaitCond()
    {
    pthread_cond_wait(&_cond, &_mtx);
    }

    T GetTask()
    {
        // T task = _task.front();
        // _task.pop();

        T task = _ptr_task_pthread->front();
        _ptr_task_pthread->pop();

        return task;
    }

    bool IsEmpty()
    {
        // return _task.empty();
        return _ptr_task_pthread->empty();
    }

    private:
    std::vector<Thread::Thread*> _threads;
    Thread::Thread* _thread_bash;
    std::queue<T>* _ptr_task_pthread;
    std::queue<T>* _ptr_task;
    std::queue<T> _task_a;
    std::queue<T> _task_b;
    pthread_mutex_t _mtx;
    pthread_cond_t _cond;
    size_t _num;
};

}
#endif
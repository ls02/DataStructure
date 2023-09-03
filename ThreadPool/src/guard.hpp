/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-02 18:39:20
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-03 16:54:34
 * @FilePath: /DataStructure/ThreadPool/src/guard.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __GUARD_HPP__
#define __GUARD_HPP__
#include <iostream>
#include <pthread.h>

class mutex
{
public:
    mutex(pthread_mutex_t *mtx) : _pmtx(mtx)
    {
    }

    void lock()
    {
        pthread_mutex_lock(_pmtx);
    }

    void unlock()
    {
        pthread_mutex_unlock(_pmtx);
    }

private:
    pthread_mutex_t *_pmtx;
};

class LockGurad
{
public:
    LockGurad(pthread_mutex_t *pmtx)
        : _mtx(pmtx)
    {
        _mtx.lock();
    }

    ~LockGurad()
    {
        _mtx.unlock();
    }

private:
    mutex _mtx;
};

#endif
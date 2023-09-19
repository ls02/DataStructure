/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-04 11:03:13
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-04 11:06:48
 * @FilePath: /DataStructure/ThreadPoolStatic/src/LockGuard.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __LOCK_GUARD_HPP__
#define __LOCK_GUARD_HPP__
#include <pthread.h>

class Mutex
{
    public:
    Mutex(pthread_mutex_t *pmtx): _pmtx(pmtx)
    {

    }

    void Lock()
    {
        pthread_mutex_lock(_pmtx);
    }

    void UnLock()
    {
        pthread_mutex_unlock(_pmtx);
    }

    private:
    pthread_mutex_t* _pmtx;
};


class LockGuard
{
    public:
    LockGuard(pthread_mutex_t* pmtx):_mtx(pmtx)
    {
        _mtx.Lock();
    }

    ~LockGuard()
    {
        _mtx.UnLock();
    }

    private:
    Mutex _mtx;
};

#endif
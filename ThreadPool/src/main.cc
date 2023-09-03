/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-02 18:47:41
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-03 17:06:24
 * @FilePath: /DataStructure/ThreadPool/src/main.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "guard.hpp"
#include "ThreadPool.hpp"
#include <string>
#include <unistd.h>
#include "Task.hpp"
#include <cstdlib>

#define NUM 4

int count = 0;
pthread_mutex_t mtx;
pthread_cond_t cond;
bool flags = false;

void *run(void *args)
{
    std::string str("我是线程");
    int i = *(int *)args;

    str += std::to_string(i);

    while (count < 100)
    {
        if (flags)
        {
        }
        LockGurad gurad_mtx(&mtx);
        count++;

        std::cout << str << ": count>: " << count << std::endl;
        usleep(10000);
        flags = false;
    }
}

void Test1()
{
    pthread_t t1[NUM];
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cond, nullptr);

    for (int i = 0; i < NUM; i++)
    {
        pthread_create(&t1[i], nullptr, run, (void *)&i);
    }

    for (int i = 0; i < NUM; i++)
    {
        pthread_join(t1[i], nullptr);
    }

    std::cout << "count: " << count << std::endl;
}

int add(int x, int y)
{
    return x + y;
}

int main()
{
    srand((unsigned long)time(nullptr));
    ThreadPool<Task> *tp = new ThreadPool<Task>();

    tp->run();

    while (true)
    {
        int x = rand() % 100 + 1;
        usleep(1000);
        int y = rand() % 100 + 1;
        Task t(x, y, add);

        std::cout << "制作任务完成: " << x << " + " << y << "= ? " << std::endl;

        tp->PushTask(t);
        // sleep(1);
    }

    return 0;
}
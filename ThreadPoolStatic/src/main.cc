/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-04 11:26:43
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-04 16:26:48
 * @FilePath: /DataStructure/ThreadPoolStatic/src/main.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include "ThreadPool.hpp"
#include "Task.hpp"

int add(int x, int y)
{
    return x + y;
}

void Test1()
{
    ThreadPool::ThreadPool<Task::Task>* ptp = new ThreadPool::ThreadPool<Task::Task>();
    ptp->run();

    while (true)
    {
        int x = rand() % 100 + 1;
        usleep(10000);
        int y = rand() % 100 + 1;

        std::cout << "任务生成： " << x << " + " << y << " = " << " ? " << std::endl;

        Task::Task task(x, y, [](int x, int y)->int{
            return x + y;
        });

        // Task::Task task(x, y, add);

        ptp->PushTask(task);

        // sleep(1);
        // usleep(100000);
    }
}

int main()
{
    srand((unsigned long)time(nullptr));

    Test1();

    return 0;
}
#include "Heap.hpp"
#include <iostream>
#include <ctime>
#include <vector>

void Test1()
{
    Heap h1;

    for (int i = 0; i < 10; i++)
    {
        h1.PushBack(rand() % 100 + 1);
    }

    h1.Print();

    while (!h1.Empty())
    {
        std::cout << h1.Top() << "  ";
        h1.PopBack();
    }

    printf("\n");
}

void Test2()
{
    std::cout << "=========== begin ===============" << std::endl;

    int data[20] = { 0 } ;

    for (int i = 0; i < 20; i++)
    {
        int x = rand() % 100 + 1;
        data[i] = x;
        std::cout << x << "  ";
    }

    printf("\n");

    HeapSort(data, 20); 

    for (int i = 0; i < 20; i++)
    {
        std::cout << data[i] << "  ";
    }

    printf("\n");

    std::cout << "============ end =================" << std::endl;
}

int main()
{
    srand((size_t)time(nullptr));

    Test1();

    Test2();

    return 0;
}

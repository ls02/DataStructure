#include <stdio.h>
#include "Heap.h"
#include <time.h>

void Test1()
{
    printf("=========start==========\n");
    HP* php = (HP*)malloc(sizeof(HP));
    if (NULL == php)
    {
        perror("malloc fail");
        exit(1);
    }

    HeapInit(php);

    for (int i = 0; i < 10; i++)
    {
        HeapPush(php, rand() % 100 + 1);
    }

    HeapPrint(php);

    while (!HeapEmpty(php))
    {
        printf("%2d  ", HeapTop(php));
        HeapPop(php);
    }

    printf("\n");;
    printf("===========end=============\n");

    HeapDestroy(php);
    free(php);
}

void Test2()
{
    int arr[10] = { 0 };
    for (int i = 0; i < 10; i++)
    {
        arr[i] = rand() % 100 + 1;
        printf("%d  ", arr[i]);
    }

    printf("\n");

    HeapSort(arr, 10);

    for (int i = 0; i < 10; i++)
    {
        printf("%d  ", arr[i]);
    }
    printf("\n");
}

int main()
{
    srand(time(NULL));
    Test1();
    Test2();
    
    return 0;
}

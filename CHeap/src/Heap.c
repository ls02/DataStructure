#include "Heap.h"

void HeapInit(HP *php)
{
    assert(php);
    // 默认开辟四个空间
    php->data = (HPDataType*)malloc(sizeof(HPDataType) * 4);
    php->size = 0;
    php->capacity = 4;
}

void HeapPush(HP *php, HPDataType val)
{
    assert(php);
    assert(php->data);

    if (php->size == php->capacity)
    {
        HPDataType *tmp = (HPDataType*)realloc(php->data, sizeof(HPDataType) * php->capacity * 2);
        if (NULL == tmp)
        {
            perror("realloc fail\n");
            
            exit(1);
        }

        php->data = tmp;
        php->capacity *= 2;
    }

    php->data[php->size++] = val;
    AdjustUp(php->data, php->size - 1);
}

void HeapPop(HP *php)
{
    assert(php);
    assert(php->data);

    HeapSwap(php->data, php->data + php->size - 1);
    php->size--;
    AdjustDown(php->data, 0, php->size);
}

HPDataType HeapTop(HP *php)
{
    return php->data[0];
}

bool HeapEmpty(HP *php)
{
    return 0 == php->size;
}

void HeapSort(HPDataType *data, int size)
{
    // 建堆
    for (int i = (size - 2) >> 1; i >= 0; i--)
    {
        AdjustDown(data, i, size);
    }

    int end = size - 1;

    while (end >= 0)
    {
        HeapSwap(data, data + end);
        AdjustDown(data, 0, end);
        end--;
    }
}

void HeapPrint(HP *php)
{
    for (int i = 0; i < php->size; i++)
    {
        printf("%2d  ", php->data[i]);
    }

    printf("\n");
}

void AdjustDown(HPDataType *data, int parent, int size)
{
    assert(data);

    int child = parent * 2 + 1;
    while (child < size)
    {
        if (child + 1 < size && data[child + 1] > data[child])
        {
            child++;
        }

        if (child < size && data[child] > data[parent])
        {
            HeapSwap(data + child, data + parent);
            parent = child;
            child = parent * 2 + 1;
        }
        else 
        {
            break;
        }
    }
}

void AdjustUp(HPDataType *data, int child)
{
    assert(data);

    int parent = (child - 1) >> 1;
    while (child > 0)
    {
        if (data[child] > data[parent])
        {
            HeapSwap(data + child, data + parent);
            child = parent;
            parent = (child - 1) >> 1;
        }
        else 
        {
            break;
        }
    }
}

void HeapSwap(HPDataType *x, HPDataType *y)
{
    HPDataType tmp = *x;
    *x = *y;
    *y = tmp;
}

void HeapDestroy(HP *php)
{
    assert(php);
    assert(php->data);

    free(php->data);
    php->size = 0;
    php->capacity = 0;
}

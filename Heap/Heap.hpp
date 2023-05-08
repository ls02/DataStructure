#ifndef __HEAP_HPP__
#define __HEAP_HPP__ 

#include <iostream>
#include <ctime>
#include <algorithm>

typedef int HPData;

class Heap 
{
private:
    HPData *_data;
    int _size;
    int _capacity;

public:
    Heap(int capacity = 4)
        :_capacity(capacity)
        ,_size(0)
    {
        _data = new HPData[4];
    }

    ~Heap()
    {
        delete[] _data;
        _size = 0;
        _capacity = 0;
    }

    int Size()
    {
        return _size;
    }

    int Capacity()
    {
        return _capacity;
    }

    HPData Top()
    {
        return _data[0];
    }

    void Swap(int* x, int* y)
    {
        HPData temp = *x;
        *x = *y;
        *y = temp;
    }

    void AdjustUp(int child)
    {
        int parent =  (child - 1) >> 1;
        while (child > 0)
        {
            if (_data[child] > _data[parent])
            {
                Swap(_data + child, _data + parent);
                child = parent;
                parent = (child - 1) >> 1;
            }
            else 
            {
                break;
            }
        }
    }

    void AdjustDown(int parent, int size)
    {
        int child = (parent << 1) + 1;
        while (child < size)
        {
            if (child + 1 < size && _data[child + 1] > _data[child])
            {
                child++;
            }

            if (_data[child] > _data[parent])
            {
                Swap(_data + child, _data + parent);
                parent = child;
                child = (parent << 1) + 1;
            }
            else 
            {
                break;
            }
        }
    }

    void PushBack(const HPData &val)
    {
        CheckCapacity();
        _data[_size++] = val;
        AdjustUp(_size - 1);
    }

    void PopBack()
    {
        Swap(_data, _data + _size - 1);
        _size--;

        AdjustDown(0, _size);
    }

    // 检查扩容
    void CheckCapacity()
    {
        if (_size == _capacity)
        {
            ReSize(_capacity * 2);
        }
    }

    // 设置空间大小
    void ReSize(int n)
    {
        if (n > _size)
        {
            HPData* tmp = new HPData[n];

            for (int i = 0; i < _size; i++)
            {
                tmp[i] = _data[i];
            }
            _capacity = n;

            delete[] _data;
        _data = tmp;
        }
    }

    bool Empty()
    {
        return _size == 0;
    }

    void Print()
    {
        for (int i = 0; i < _size; i++)
        {
            std::cout << _data[i] << "  ";
        }

        printf("\n");
    }
};

void Swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void AdjustUp(HPData *data, int child)
{
    int parent =  (child - 1) >> 1;
    while (child > 0)
    {
        if (data[child] > data[parent])
        {
            Swap(data + child, data + parent);
            child = parent;
            parent = (child - 1) >> 1;
        }
        else 
        {
            break;
        }
    }
}

void AdjustDown(HPData *data, int parent, int size)
{
    int child = (parent << 1) + 1;
    while (child < size)
    {
        if (child + 1 < size && data[child + 1] > data[child])
        {
            child++;
        }

        if (data[child] > data[parent])
        {
            Swap(data + child, data + parent);
            parent = child;
            child = (parent << 1) + 1;
        }
        else 
        {
            break;
        }
    }
}

void HeapSort(HPData *data, int size)
{
    for (int i = (size - 2) >> 1; i >= 0; i--)
    {
        AdjustDown(data, i, size);
    }

    int end = size - 1;
    while (end > 0)
    {
        Swap(data, data + end);
        AdjustDown(data, 0, end);
        end--;
    }
}

#endif

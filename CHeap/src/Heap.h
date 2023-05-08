#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>

// 堆存储的数据类型
typedef int HPDataType;

// 堆的结构体类型
typedef struct Heap 
{
    HPDataType* data;
    int size;
    int capacity;
}HP;

// 初始化堆
extern void HeapInit(HP *php);

// 插入数据
extern void HeapPush(HP *php, HPDataType val);

// 删除数据
extern void HeapPop(HP *php);

// 获取堆顶数据
extern HPDataType HeapTop(HP *php);

// 判断堆是否为空，如果为空返回 真
extern bool HeapEmpty(HP *php);

// 释放空间
extern void HeapDestroy(HP *php);

// 向下调整算法
extern void AdjustDown(HPDataType *data, int parent, int size);

// 向上调整算法
extern void AdjustUp(HPDataType *data, int child);

// 堆排序
extern void HeapSort(HPDataType *data, int size);

// 打印堆中数据
extern void HeapPrint(HP *php);

// 交换数据
extern void HeapSwap(HPDataType *x, HPDataType *y);

#endif

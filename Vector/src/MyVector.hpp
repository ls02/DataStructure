/*** 
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-18 17:11:19
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-25 14:40:05
 * @FilePath: /DataStructure/Vector/src/MyVector.hpp
 * @Description: 
 * @
 * @Copyright (c) 2023 by ls02, All Rights Reserved. 
 */

/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *            佛祖保佑     永不宕机     永无BUG
 *
 *        佛曰:
 *                写字楼里写字间，写字间里程序员；
 *                程序人员写程序，又拿程序换酒钱。
 *                酒醒只在网上坐，酒醉还来网下眠；
 *                酒醉酒醒日复日，网上网下年复年。
 *                但愿老死电脑间，不愿鞠躬老板前；
 *                奔驰宝马贵者趣，公交自行程序员。
 *                别人笑我忒疯癫，我笑自己命太贱；
 *                不见满街漂亮妹，哪个归得程序员？
 */

#ifndef __MY_VECTOR_HPP__
#define __MY_VECTOR_HPP__

#include <iostream>
#include <assert.h>
#include "../../ReverseIterator/ReverseIterator.hpp"

namespace ls
{
    template <class T>
    class vector
    {
    public:
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef ReverseIterator<iterator, T&, T*> reverse_iterator;
        typedef ReverseIterator<const_iterator, const T&, const T*> const_reverse_iterator;

        iterator begin()
        {
            return _start;
        }

        iterator end()
        {
            return _finish;
        }

        const_iterator begin() const
        {
            return _start;
        }

        const_iterator end() const
        {
            return _finish;
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

    public:
        /***
         * @description: 默认构造函数
         * @return {*}
         */
        vector()
            : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr)
        {
        }

        vector(size_t n, const T& value = T())
        :_start(nullptr),_finish(nullptr),_end_of_storage(nullptr)
        {
            reserve(n);

            for (size_t i = 0; i < n; i++)
            {
                push_back(value);
            }
        }

        template <class InputIterator>
		vector(InputIterator first, InputIterator last)
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			while(first != last)
			{
				push_back(*first);
				++first;
			}
		}

        /***
         * @description: 拷贝构造
         * @param {vector} &other
         * @return {*}
         */
        vector(const vector<T> &other)
        :_start(nullptr),_finish(nullptr),_end_of_storage(nullptr)
        {
            vector<T> tmp(other.begin(), other.end());
            swap(tmp);
        }

        /***
         * @description: 尾插
         * @return {*}
         */
        void push_back(const T &value)
        {
            insert(size(), value);
        }

        /*** 
         * @description: 赋值重载
         * @return {*} 返回一个深拷贝好的 vector
         */
        vector<T>& operator=(vector<T> v)
        {
            swap(v);

            return *this;
        }

        /***
         * @description: 任意位置插入
         * @param {size_t} pos 如果大于当前元素的size那么将会触发禁言
         * @return {*}
         */
        void insert(size_t pos, const T &value = T())
        {
            assert(pos <= size());

            // 检查是否需要扩容
            if (size() == capacity())
            {
                int new_capacity = _start == nullptr ? 1 : capacity() * 2;
                reserve(new_capacity);
            }

            // 插入的两种形式
            // 尾插
            if (pos == size())
            {
                *_finish = value;
            }
            else
            {
                // 头插和中间位置插入
                // 如果是头插和中间位置插入需要挪动数据腾出空间
                // 1. 腾出空间
                size_t end = size();
                while (end > pos)
                {
                    _start[end] = _start[end - 1];
                    end--;
                }

                // 2. 在 pos 位置插入数据
                _start[pos] = value;
            }

            _finish++;
        }

        void clear()
        {
            delete[] _start;
            _start = _finish = _end_of_storage = nullptr;
        }

        /***
         * @description: 获取实际元素个数
         * @return 返回实际的个数
         */
        size_t size() const
        {
            return _finish - _start;
        }

        /***
         * @description: 获取容量
         * @return 返回一个容量
         */
        size_t capacity() const
        {
            return _end_of_storage - _start;
        }

        /***
         * @description: 扩容函数,如果n大于capacity的容量，那么将会扩容小于则什么都不做
         * @param {size_t} n 设置要扩充的容量
         * @return {*}
         */
        void reserve(size_t n)
        {
            // 如果不大于当前容量什么都不做
            if (n < capacity())
            {
                return;
            }

            // 如果n大于当前容量，那么就需要扩容
            T *new_start = new T[n];
            T *new_finish = new_start + size();
            T *new_end_of_storage = new_start + n;
            T *old_start = _start;

            // 拷贝数据到新的空间里
            for (int i = 0; i < size(); i++)
            {
                new_start[i] = old_start[i];
            }

            _start = new_start;
            _finish = new_finish;
            _end_of_storage = new_end_of_storage;

            // 释放旧空间的数据
            delete[] old_start;
        }

        /***
         * @description: 设置实际个数大小，如果个数大小大于当前容量会扩容
         * @param {size_t} n 要设置新的大小个数
         * @param { T} value 默认给value，如果是内置类型会初始化成0
         * @return {*}
         */
        void resize(size_t n, const T value = T())
        {
            if (n > capacity())
            {
                reserve(n);

                for (int i = size(); i < n; i++)
                {
                    _start[i] = value;
                }
            }
            else
            {
                _finish -= n;
            }
        }

        void pop_back()
        {
            erase(size());
        }

        /***
         * @description: 任意位置删除
         * @param {size_t} pos 删除 pos 位置的值
         * @return {*} 返回删除后的位置，防止迭代器失效
         */
        iterator erase(size_t pos)
        {
            assert(pos <= size());

            for (int i = 0; i < size() - 1; i++)
            {
                _start[pos + i] = _start[pos + i + 1];
            }

            _finish--;

            return _start + pos;
        }

        /*** 
         * @description: []函数重载
         * @param {size_t} pos 
         * @return {*} 返回当前下标的元素
         */
        int operator[](size_t pos)
        {
            return _start[pos];
        }

        /*** 
         * @description: 获取pos位置的元素
         * @param {size_t} pos
         * @return {*} 返回当前下标的元素
         */
        int at(size_t pos)
        { return _start[pos];
        }

        void Printf() const
        {
            for (int i = 0; i < size(); i++)
            {
                std::cout << _start[i] << ' ';
            }

            printf("\n");
        }

        /*** 
         * @description: 交换函数
         * @param {vector<T>} &other
         * @return {*}
         */
        void swap(vector<T> &other)
        {
            std::swap(_start, other._start);
			std::swap(_finish, other._finish);
			std::swap(_end_of_storage, other._end_of_storage);
        }

        friend std::ostream &operator<<(std::ostream &out, const vector<int> &array);

    private:
        iterator _start;
        iterator _finish;
        iterator _end_of_storage;
    };

    std::ostream &operator<<(std::ostream &out, const vector<int> &array)
    {
        for (auto e : array)
        {
            out << e << ' ';
        }

        return out;
    }

}

#endif
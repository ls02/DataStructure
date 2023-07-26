/*** 
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-26 11:47:19
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-26 15:14:57
 * @FilePath: /DataStructure/PriorityQueue/src/PriorityQueue.hpp
 * @Description: 
 * @
 * @Copyright (c) 2023 by ls02, All Rights Reserved. 
 */
#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__
#include <iostream>
#include <vector>

namespace ls
{
    template<class T,class Container = std::vector<T>, class Compare = std::less<T>>
    class priority_queue
    {
        private:

            /*** 
             * @description: 向下调整算法
             * @param {int} parent 
             * @return {*}
             */
            void AdjustDown(int parent)
            {
                Compare com;

                int child = parent * 2 + 1;
                while (child < size())
                {
                    // 看那个孩子更小 or 更大
                    if (child + 1 < size() && com(_con[child], _con[child + 1]))
                    {
                        child++;
                    }

                    if (com(_con[parent], _con[child]))
                    {
                        std::swap(_con[child], _con[parent]);
                        parent = child;
                        child = (parent << 1) + 1;
                    }
                    else 
                    {
                        break;
                    }
                }
            }

            /*** 
             * @description: 向上调整算法
             * @param {int} child 
             * @return {*}
             */
            void AdjustUp(int child)
            {
                Compare com;
                int parent = (child - 1) >> 1;

                while (child > 0)
                {
                    if (com(_con[parent], _con[child]))
                    {
                        std::swap(_con[child], _con[parent]);
                        parent = child;
                        child = (child - 1) >> 1;
                    }
                    else 
                    {
                        break;
                    }
                }
            }

        public:
        priority_queue()
        {}

        template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
        {
            while (first != last)
            {
                _con.push_back(*first);
                ++first;
            }

            // 建堆
            for (int i = (size() - 2) >> 1; i >= 0; --i)
            {
                AdjustDown(i);
            }
        }

        ~priority_queue()
        {}

        void push(const T& value)
        {
            _con.push_back(value);
            AdjustUp(size() - 1);
        }

        void pop()
        {
            std::swap(_con[0], _con[size() - 1]);
            _con.pop_back();

            AdjustDown(0);
        }

        const T& top() const
        {
            return _con.front();
        }

        size_t size() const
        {
            return _con.size();
        }

        bool empty() const
        {
            return _con.empty();
        }

        private:
            Container _con;
    };
}

#endif
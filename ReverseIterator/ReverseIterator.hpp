/*** 
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-25 10:30:04
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-25 14:24:34
 * @FilePath: /DataStructure/ReverseIterator/ReverseIterator.hpp
 * @Description: 
 * @
 * @Copyright (c) 2023 by ls02, All Rights Reserved. 
 */

#ifndef __REVERSE_ITERATOR_HPP__
#define __REVERSE_ITERATOR_HPP__

namespace ls
{
    template<class Iterator, class Ref, class Ptr>
    struct ReverseIterator
    {
        typedef ReverseIterator<Iterator, Ref, Ptr> self;
        Iterator _it;

        ReverseIterator(Iterator it) : _it(it) {}

        self& operator++()
        {
            --_it;

            return *this;
        }

        self operator++(int)
        {
            self tmp(*this);
            _it--;

            return tmp;
        }

        self &operator--()
        {
            ++_it;

            return *this;
        }

        self operator--(int)
        {
            self tmp(*this);
            _it++;

            return tmp;
        }

        Ref operator*()
        {
            Iterator tmp = _it;

            return *(--tmp);
        }

        Ptr operator->()
        {
            return &(operator*());
        }

        bool operator==(const self &rhs) const
        {
            return _it == rhs._it;
        }

        bool operator!=(const self &rhs) const
        {
            return _it != rhs._it;
        }
    };
}

#endif
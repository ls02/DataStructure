/***
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-24 09:16:16
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-24 10:32:54
 * @FilePath: /List/src/MyList.hpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ls02, All Rights Reserved.
 */
#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <iostream>
#include <assert.h>
#include "../../ReverseIterator/ReverseIterator.hpp"

namespace ls
{
    template <class T>
    struct list_node
    {
        list_node(const T &value)
            : _value(value), _next(nullptr), _prev(nullptr)
        {
        }

        list_node<T> *_next;
        list_node<T> *_prev;

        T _value;
    };

    template <class T, class Ref, class Ptr>
    struct __list_iterator
    {
        typedef __list_iterator<T, Ref, Ptr> self;
        typedef list_node<T> node;

        __list_iterator(node *node)
            : _node(node) {}

        self &operator++()
        {
            _node = _node->_next;
            return *this;
        }

        self operator++(int)
        {
            self tmp(*this);
            _node = _node->_next;

            return tmp;
        }

        self &operator--()
        {
            // std::cout << _node->_prev->_value << std::endl;
            _node = _node->_prev;

            return *this;
        }

        self operator--(int)
        {
            self tmp(*this);
            _node = _node->_prev;

            return tmp;
        }

        Ref operator*()
        {
            return _node->_value;
        }

        Ptr operator->()
        {
            return &(operator*());
        }

        bool operator!=(const self &it) const
        {
            return _node != it._node;
        }

        bool operator==(const self &it) const 
        {
            return _node == it._node;
        }

        node *_node;
    };

    template <class T>
    class list
    {
        typedef list_node<T> node;
    public:
        typedef __list_iterator<T, T &, T *> iterator;
        typedef __list_iterator<T, const T &, const T *> const_iterator;
        typedef ReverseIterator<iterator, T &, T *> reverse_iterator;
        typedef ReverseIterator<const_iterator,const T &,const T *> const_reverse_iterator;

    public:
        iterator begin()
        {
            return iterator(_head->_next);
        }

        iterator end()
        {
            return iterator(_head);
        }

        const_iterator begin() const
        {
            return const_iterator(_head->_next);
        }

        const_iterator end() const
        {
            return const_iterator(_head);
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
        list(const T &value = T())
        {
            _head = new node(value);
            _head->_next = _head;
            _head->_prev = _head;
            _size = 0;
        }

        list(const list<T>& l)
        {
            _head = new node(l._head->_value);
            _head->_next = _head;
            _head->_prev = _head;
            _size = 0;

            node *lcur = l._head->_next;

            while (lcur != l._head)
            {
                push_back(lcur->_value);
                lcur = lcur->_next;
            }
        }

        list<T> &operator=(list<T> lt)
        {
            swap(lt);

            return *this;
        }

        ~list()
        {
            clear();
            delete _head;

            _head = nullptr;
            _size = 0;
        }

        void clear()
        {
            node *cur = _head->_next;
            while (cur != _head)
            {
                node *old_node = cur;
                cur = cur->_next;
                delete old_node;
            }
        }

        iterator insert(iterator pos, const T &value)
        {
            node *new_node = new node(value);
            node *cur = pos._node;
            node *prev = cur->_prev;
            node *next = cur->_next;
            new_node->_prev = cur;
            new_node->_next = next;
            cur->_next = new_node;
            next->_prev = new_node;
            ++_size;

            return iterator(new_node);
        }

        void insert(iterator it, size_t pos, const T &value)
        {
            assert(pos >= 0 && pos <= size());

            node *cur = it._node;
            while (pos)
            {
                cur = cur->_next;
                --pos;
            }

            node *new_node = new node(value);
            node *next = cur->_next;
            node *prev = cur->_prev;
            new_node->_prev = cur;
            new_node->_next = next;
            cur->_next = new_node;
            next->_prev = new_node;
            _size++;
        }

        iterator erase(iterator pos)
        {
            node *cur = pos._node;
            node *next = cur->_next;
            node *prev = cur->_prev;

            prev->_next = next;
            next->_prev = prev;

            delete cur;
            _size--;

            return iterator(next);
        }

        iterator erase(iterator first, iterator last)
        {
            node *cur = first._node;
            node *prev = cur->_prev;
            while (cur != last._node)
            {
                node *next = cur->_next;
                prev->_next = next;
                next->_prev = prev;

                delete cur;
                cur = next;
            _size--;
            }

            return iterator(cur);
        }

        void push_back(const T &value)
        {
            insert(end(), size(), value);
        }

        size_t size() const
        {
            return _size;
        }

        void swap(list<T> &other)
        {
            std::swap(other._head, _head);
            std::swap(other._size, _size);
        }

        void print() const
        {
            node *cur = _head->_next;

            while (cur != _head)
            {
                std::cout << cur->_value << " ";
                cur = cur->_next;
            }

            printf("\n");
        }

    private:
        node *_head;
        size_t _size;
    };
}

#endif
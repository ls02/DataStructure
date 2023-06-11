#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <iostream>

namespace ls
{
    template <class T>
    struct list_Node
    {
        list_Node(const T &val = T())
            : _data(val), _next(nullptr), _prev(nullptr)
        {
        }

        T _data;
        list_Node<T> *_next;
        list_Node<T> *_prev;
    };

    // 迭代器
    template <class T, class Ref, class Ptr>
    struct __list_iterator
    {
        typedef list_Node<T> Node;
        typedef __list_iterator<T, Ref, Ptr> iterator;

        // typedef bidirectional_iterator_tag iterator_category;
        // typedef T value_type;
        // typedef Ptr pointer;
        // typedef Ref reference;
        // typedef ptrdiff_t difference_type;

        Node *_node;

        __list_iterator(Node *node)
            : _node(node)
        {
        }

        bool operator!=(const iterator &it) const
        {
            return _node != it._node;
        }

        Ref &operator*()
        {
            return _node->_data;
        }

        Ptr operator->()
        {
            return &(operator*());
        }

        // ++it
        iterator &operator++()
        {
            _node = _node->_next;

            return *this;
        }
    };

    template <class T>
    class list
    {
        typedef list_Node<T> Node;

    public:
        typedef __list_iterator<T, T &, T *>
            iterator;
        typedef __list_iterator<T, const T &, const T *> const_iterator;

        iterator begin()
        {
            return iterator(_head->_next);
        }

        iterator end()
        {
            return iterator(_head);
        }

        list()
        {
            _head = new Node();
            _head->_next = _head;
            _head->_prev = _head;
        }

        void push_back(const T &val = T())
        {
            Node *tail = _head->_prev;
            Node *newNode = new Node(val);

            tail->_next = newNode;
            newNode->_prev = tail;
            newNode->_next = _head;
            _head->_prev = newNode;
        }

        void Print() const
        {
            Node *curr = _head->_next;
            while (curr != _head)
            {
                std::cout << curr->_data << " ";
                curr = curr->_next;
            }
        }

    private:
        Node *_head;
    };
}

#endif
#ifndef __BS_TREE_HPP__
#define __BS_TREE_HPP__

#include <iostream>

template <class T> struct BSTreeNode {
    typedef BSTreeNode<T> Node;
    BSTreeNode(const T &val = T())
        : _left(nullptr), _right(nullptr), _val(val) {}

    BSTreeNode<T> *_left;
    BSTreeNode<T> *_right;
    T _val;
};

template <class T> class BSTree {
    typedef BSTreeNode<T> Node;

  public:
    BSTree() : _root(nullptr) {}

    bool insert(const T &val) {
        if (_root == nullptr) {
            _root = new Node(val);
            return true;
        }

        // 如果不为空开始找合适的位置插入 val
        // 1. 如果 val 大于 _root 那么就去左边，小于去右边，为空就说明找到了
        Node *cur = _root;
        Node *parent = nullptr;
        Node *new_node = new Node(val);

        while (cur != nullptr) {
            if (cur->_val > val) {
                parent = cur;
                cur = cur->_left;
            } else if (cur->_val < val) {
                parent = cur;
                cur = cur->_right;
            } else {
                // 如果相等说明就有问题了，我们的数据结构不允许有相等的存在
                return false;
            }
        }

        // 如果走到这还没退出说名走到要插入的位置了
        // 需要先判断一下 parent 是否合法
        if (parent == nullptr) {
            // 如果这能等于 nullptr 说明 cur 就是空，这就有问题
            return false;
        }

        // 这里的 parent 理论来讲不可能为空
        if (parent->_left == cur) {
            parent->_left = new_node;
        } else {
            parent->_right = new_node;
        }

        return true;
    }

    void print() {
        _print(_root);
        std::cout << std::endl;
    }

    void _print(Node *root) {
        if (root == nullptr) {
            return;
        }

        if (root->_left != nullptr) {
            _print(root->_left);
        }

        if (root->_right != nullptr) {
            _print(root->_left);
        }

        std::cout << root->_val << " ";
    }

  private:
    Node *_root;
};

#endif // !__BS_TREE_HPP__

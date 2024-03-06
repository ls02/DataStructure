#include "BSTree.hpp"

int main() {
    BSTree<int> t1;

    t1.insert(1);
    t1.insert(2);
    t1.insert(3);

    t1.print();

    return 0;
}

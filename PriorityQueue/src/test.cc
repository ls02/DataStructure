#include <iostream>
#include "PriorityQueue.hpp"

int main()
{
    std::vector<int> v;

    v.push_back(8);
    v.push_back(6);
    v.push_back(3);
    v.push_back(2);
    v.push_back(1);
    v.push_back(11);

    ls::priority_queue<int> pq(v.begin(), v.end());


    while (!pq.empty())
    {
        std::cout << pq.top() << " ";
        pq.pop();
    }

    return 0;
}
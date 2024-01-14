#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <bits/extc++.h>

using ll = long long;
using ull = unsigned ll;
using namespace __gnu_pbds;
template<typename T>
using Tree = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

int main() {
    Tree<int> s;
    while(true) {
        int mode;
        std::cin >> mode;
        if(mode == 0) {
            int value;
            std::cin >> value;
            s.insert(value);
        }
        else if(mode == 1) {
            int value;
            std::cin >> value;
            s.erase(value);
        }
        else if (mode == 2) {
            // print the number of elements that is lower than value
            int value;
            std::cin >> value;
            std::cout << s.order_of_key(value) << std::endl;
        }
        else { 
            // get the k-th elements in the set
            int value;
            std::cin >> value;
            std::cout << *s.find_by_order(value) << std::endl;
        }
    }
}
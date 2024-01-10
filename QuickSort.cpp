#include <iostream>
#include <algorithm>
#include <vector>

using ll = long long;
using ull = unsigned ll;
ll MININT = -1e18;
ll MAXINT = 1e18;

void recur1(std::vector<int>& v, int left, int right) {
    if(right <= left) return;
    int z = v[left];
    int i = left + 1;
    for(int q = left + 1;q <= right;q++) {
        if(v[q] <= z) {
            std::swap(v[q],v[i]);
            ++i;
        }
    }
    std::swap(v[left],v[i - 1]);
    recur1(v,left,i - 2);
    recur1(v,i,right);
}

void quick_sort(std::vector<int>& v) {
    recur1(v,0,v.size() - 1);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int number = (ull)get_rand_value() % 10000000;
    std::vector<int> test;
    for(int q = 0;q < number;q++) {
        test.push_back(get_rand_value() % 1000000);
    }
    std::vector<int> ans(test.begin(),test.end());
    quick_sort(test);
    std::sort(ans.begin(),ans.end());
    for(int q = 0;q < ans.size();q++) {
        if(test[q] != ans[q]) {
            return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 0;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}


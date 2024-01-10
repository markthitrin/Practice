#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long ;
using ull = unsigned ll;

const ll MIXINT = -1e18;
const ll MAXINT = 1e18;

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void recur1(std::vector<int>& v, int left, int right) {
    if(right <= left) return;
    ll mid = get_mid(left,right);
    recur1(v,left,mid);
    recur1(v,mid + 1,right);
    std::vector<int> temp(right - left + 1,0);
    int cleft = left;
    int cright = mid + 1;
    for(int q = 0;q < right - left + 1;q++) {
        if(cleft == mid + 1) temp[q] = v[cright++];
        else if(cright == right + 1) temp[q] = v[cleft++];
        else if(v[cleft] < v[cright]) temp[q] = v[cleft++];
        else temp[q] = v[cright++];
    } 
    for(int q = left;q <= right;q++) {
        v[q] = temp[q - left];
    }
    return;
}

void merge_sort(std::vector<int>& v) {
    recur1(v,0,v.size() - 1);    
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int number = (ull)get_rand_value() % 1000000;
    std::vector<int> v;
    for(int q = 0;q < number;q++) {
        v.push_back(get_rand_value() % 100000);
    }
    std::vector<int> ans(v.begin(),v.end());
    std::sort(ans.begin(),ans.end());
    merge_sort(v);
    for(int q = 0;q  <v.size();q++) {
        if(v[q] != ans[q]) return false;
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
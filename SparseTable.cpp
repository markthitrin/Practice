#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

std::vector<std::vector<ll>> get_sparse_table(std::vector<ll>& a) {
    std::vector<std::vector<ll>> res;
    res.push_back(std::vector<ll>(a.begin(),a.end()));
    ll k = 2;
    while(k <= a.size()) {
        res.push_back(std::vector<ll>(a.size(),0));
        for(int q = 0;q + k <= a.size();q++) {
            ll half = k / 2;
            res.back()[q] = std::min(res[res.size() - 2][q], res[res.size() - 2][q + half]);
        }
        k = k << 1;
    }
    return res;
}

ll get_min_sparse_table(std::vector<std::vector<ll>>& a, ll left,ll right) {
    if(right == left) return a[0][left];
    ll k = 63 - __builtin_clzll((right - left + 1));
    return std::min(a[k][left],a[k][right - (1ll << k) + 1]);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int number = (ull)get_rand_value() % 10000;
    std::vector<ll> a(number);
    for(int q = 0;q < a.size();q++) {
        a[q] = get_rand_value() % (40000);
    }
    std::vector<std::vector<ll>> a_sparse_table = get_sparse_table(a);

    auto do_get_min = [&](ll left,ll right) {
        ll res = MAXINT;
        for(int q = left;q <= right;q++) {
            res = std::min(res,a[q]);
        }
        ll get_res = get_min_sparse_table(a_sparse_table,left,right);
        return get_res == res;
    };

    for(int q = 0;q < 10000;q++) {
        ll left = (ull)get_rand_value() % a.size();
        ll right = (ull)get_rand_value() % a.size();
        if(right < left) std::swap(left,right);
        if(!do_get_min(left,right)) {
            return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
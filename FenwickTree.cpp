#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

void add_fenwick(std::vector<ll>& fen, ll pos, ll value) {
    ++pos;
    for(int q = pos;q < fen.size();q+=(q&-q)) {
        fen[q] += value;
    }
}

ll get_fenwick(std::vector<ll>& fen, ll pos) {
    ++pos;
    ll res = 0;
    for(int q = pos;q > 0;q-=(q&-q)) {
        res += fen[q];
    }
    return res;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll range = (ull)get_rand_value() % 100000;
    std::vector<ll> fen(range + 1);
    std::vector<ll> ans_arr(range);

    auto do_add = [&](ll pos,ll value) {
        ans_arr[pos] += value;
        add_fenwick(fen,pos,value);
    };
    auto do_get_sum = [&](ll pos) {
        ll ans = 0;
        for(int q = 0;q <= pos;q++) {
            ans += ans_arr[q];
        }
        ll try_ans = get_fenwick(fen,pos);
        if(try_ans != ans) {
            std::cout << "dfsdf";
        }
        return try_ans == ans;
    };

    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll value = get_rand_value() % 1000000;
            ll pos = (ull)get_rand_value() % range;
            do_add(pos,value);
        }
        else {
            ll pos = (ull)get_rand_value() % range;
            if(!do_get_sum(pos)) 
                return false;
        }
    }
    return true;
}

int main() {
    int _  = 0;
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
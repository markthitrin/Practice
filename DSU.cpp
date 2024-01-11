#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

std::vector<ll> DD;

ll get_head(ll a) {
    std::vector<ll> c;
    while(DD[a] != a) {
        c.push_back(a);
        a = DD[a];
    }
    for(int q = 0;q < c.size();q++) {
        DD[c[q]] = a;
    }
    return a;
}

void join(ll a,ll b) {
    ll heada = get_head(a);
    ll headb = get_head(b);
    DD[heada] = headb;    
}

bool check(ll a,ll b) {
    ll heada = get_head(a);
    ll headb = get_head(b);
    return heada == headb;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool check_if_reach(std::vector<std::vector<ll>>& f,ll u,ll p,ll t) {
    if(u == t) return true;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q] != p) {
            if(check_if_reach(f,f[u][q],u,t)) return true;
        }
    }
    return false;
}

bool run_test() {
    DD.clear();
    ll number = (ull)get_rand_value() % 1000000;
    std::vector<std::vector<ll>> f;

    for(int q = 0;q < number;q++) DD.push_back(q);
    for(int q = 0;q < number;q++) f.push_back(std::vector<ll>());

    auto do_join = [&](int a,int b) {
        if(check_if_reach(f,a,-1,b)) return;
        join(a,b);
        f[a].push_back(b);
        f[b].push_back(a);
    };
    auto do_check = [&](int a,int b) {
        return check_if_reach(f,a,-1,b) == check(a,b);
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll a = (ull)get_rand_value() % number;
            ll b = (ull)get_rand_value() % number;
            do_join(a,b);
        }
        else {
            ll a = (ull)get_rand_value() % number;
            ll b = (ull)get_rand_value() % number;
            if(!do_check(a,b)) return false;
        }
    }
    return true;
}

int main () {
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
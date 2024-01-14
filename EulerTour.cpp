#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class vertex1 {
public:
    ll value = 0;
    ll parent = -1;
    ll euler_number_in;
    ll euler_number_out;
};

class edge{
public:
    ll to;
    edge() {};
    edge(ll _to) : to(_to) {}
};

std::vector<vertex1> v;
std::vector<std::vector<edge>> f;
std::vector<ll> fen;

void add_fen(std::vector<ll>& fen,ll pos,ll value) {
    ++pos;
    for(int q = pos;q < fen.size();q+=(q&-q)) {
        fen[q] += value;
    }
}

ll get_sum_fen(std::vector<ll>& fen,ll pos) {
    ll res = 0;
    ++pos;
    for(int q = pos;q >0;q-=(q&-q)) {
        res += fen[q];
    }
    return res;
}

void recur1(ll u,ll p,ll& number) {
    v[u].euler_number_in = number;
    ++number;

    for(int q = 0;q < f[u].size();q++){
        if(f[u][q].to != p) {
            recur1(f[u][q].to,u,number);
        }
    }

    v[u].euler_number_out = number;
    ++number;
}

void euler_tour(ll root) {
    ll number = 0;
    recur1(root,-1,number);
}

void add_value(ll vertex_number,ll value) {
    add_fen(fen,v[vertex_number].euler_number_out,value);
}

ll get_sum_subtree(ll vertex_number) {
    return get_sum_fen(fen,v[vertex_number].euler_number_out) - get_sum_fen(fen,v[vertex_number].euler_number_in);
}

ll recur2(ll u,ll p) {
    ll res = v[u].value;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            res += recur2(f[u][q].to,u);
        }
    }
    return res;
}

ll get_sum_subtree_brutefoorce(ll vertex_number) {
    return recur2(vertex_number,v[vertex_number].parent);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    v.clear();
    f.clear();
    fen.clear();
    fen.push_back(0);

    ll vertex_number = (ull)get_rand_value() % 10000 + 10000;
    for(int q = 0;q < vertex_number;q++) {
        v.push_back(vertex1());
        f.push_back(std::vector<edge>());
        fen.push_back(0);
        fen.push_back(0);
    }

    for(int q = 1;q < vertex_number;q++) {
        ll parent = (ull)get_rand_value() % (q);
        v[q].parent = parent;
        f[parent].push_back(edge(q));
        f[q].push_back(edge(parent));
    }
    euler_tour(0);
    auto do_add = [&](ll vertex_number,ll value) {
        add_value(vertex_number,value);
        v[vertex_number].value += value;
    };
    auto do_get_sum = [&](ll vertex_number) {
        return get_sum_subtree(vertex_number) == get_sum_subtree_brutefoorce(vertex_number);
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll number = (ull)get_rand_value() % (vertex_number);
            ll value = get_rand_value() % 100000;
            do_add(number,value);
        }
        else {
            ll number = (ull)get_rand_value() % (vertex_number);
            if(!do_get_sum(number)) return false;
        }
    }
    return true;
}

int main() {
    int  _ = 0;
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
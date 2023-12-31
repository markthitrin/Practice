#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class vertex{
public:
    ll value;
    std::pair<ll,ll> min_pair;
};

class edge{
public:
    ll to;
    ll min_value;
    ll inv;
};

std::vector<std::vector<edge>> f;
std::vector<vertex> v;

void put_min(ll vertex_number,ll value) {
    v[vertex_number].min_pair.first = std::min(value,v[vertex_number].min_pair.first);
    if(v[vertex_number].min_pair.first < v[vertex_number].min_pair.second) {
        std::swap(v[vertex_number].min_pair.first,v[vertex_number].min_pair.second);
    }
}

void recur1(ll u,ll p,ll index) {
    f[u][index].min_value = v[u].value;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur1(f[u][q].to,u,f[u][q].inv);
            f[u][index].min_value = std::min(f[u][index].min_value, f[f[u][q].to][f[u][q].inv].min_value);
            put_min(u,f[f[u][q].to][f[u][q].inv].min_value);
        }
    }
}


void recur2(ll u,ll p) {
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            if(f[f[u][q].to][f[u][q].inv].min_value == v[u].min_pair.second) {
                f[u][q].min_value = v[u].min_pair.first;
            }
            else {
                f[u][q].min_value = v[u].min_pair.second;
            }
            put_min(f[u][q].to,f[u][q].min_value);
        }
    }
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur2(f[u][q].to,u);
        }
    }
}

void build() {
    for(int q = 0;q < f[0].size();q++) {
        recur1(f[0][q].to,0,f[0][q].inv);
        put_min(0,f[f[0][q].to][f[0][q].inv].min_value);
    }
    recur2(0,-1);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

ll brute_force_getmin_subtree(ll u,ll p) {
    ll ans = v[u].value;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            ans = std::min(brute_force_getmin_subtree(f[u][q].to,u),ans);
        }
    }
    return ans;
}

bool run_test() {
    v.clear();
    f.clear();

    ll vertex_number = (ull)get_rand_value() % (10000) + 10000;
    for(int q = 0;q < vertex_number;q++) f.push_back(std::vector<edge>());
    for(int q = 0;q < vertex_number;q++) {
        v.push_back(vertex());
        v.back().min_pair = {MAXINT,MAXINT};
        v.back().value = get_rand_value() % 100;
    }
    for(int q = 1;q<vertex_number;q++) {
        ll parent = (ull)get_rand_value() % (q);
        f[q].push_back(edge());
        f[parent].push_back(edge());
        f[q].back().to = parent;
        f[parent].back().to = q;
        f[q].back().min_value = MAXINT;
        f[parent].back().min_value = MAXINT;
        f[q].back().inv = f[parent].size() - 1;
        f[parent].back().inv = f[q].size() - 1;
    }
    build();

    for(int q = 0;q < 10000;q++) {
        ll vertex = (ull)get_rand_value() % vertex_number;
        ll parent = (ull)get_rand_value() % f[vertex].size();
        if(f[vertex][parent].min_value != brute_force_getmin_subtree(vertex,f[vertex][parent].to)) {
            std::cout << f[vertex][parent].min_value << " ," << brute_force_getmin_subtree(vertex,f[vertex][parent].to) << "\n";
            return false;
        }
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
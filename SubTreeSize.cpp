#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MAXINT = 1e18;
const ll MININT = -1e18;

class edge {
public:
    ll to;
    ll sub_size = 0;
    ll inv;
};

std::vector<std::vector<edge>> f;
int N = 0;

void recur1(ll u,ll p,ll index) {
    for(int q = 0; q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur1(f[u][q].to, u, f[u][q].inv);
            f[u][index].sub_size += f[f[u][q].to][f[u][q].inv].sub_size;
        }
    }
    f[u][index].sub_size++;
    f[f[u][index].to][f[u][index].inv].sub_size = N - f[u][index].sub_size;
}

void build() {
    for(int q = 0;q < f[0].size();q++) {
        recur1(f[0][q].to,0,f[0][q].inv);
    }
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

ll brute_force_get_subtree_size(int u,int p) {
    ll sum = 1;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            sum += brute_force_get_subtree_size(f[u][q].to ,u);
        }
    }
    return sum;
}

bool run_test() {
    f.clear();

    N = (ull)get_rand_value() % (1000) + 1000;
    for(int q = 0;q < N;q++) f.push_back(std::vector<edge>());
    for(int q = 1;q< N;q++) {
        ll parent = (ull)get_rand_value() % q;
        f[q].push_back(edge());
        f[parent].push_back(edge());
        f[q].back().to = parent;
        f[parent].back().to = q;
        f[q].back().inv = f[parent].size() - 1;
        f[parent].back().inv = f[q].size() - 1;
    }
    build();

    for(int q = 0;q < 10000;q++) {
        ll vertex = (ull)get_rand_value() % (N);
        ll parent = (ull)get_rand_value() % (f[vertex].size());
        if(brute_force_get_subtree_size(vertex,f[vertex][parent].to) != f[vertex][parent].sub_size) {
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
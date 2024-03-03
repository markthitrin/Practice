// This code is tested on https://open.kattis.com/problems/shortestpath3

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

const ll MAXLL = 1e18;
const ll MINLL = -1e18;

class edge {
public:
    int to;
    ll weight;
}put_in;

class bellman_node {
public:
    ll min_distance = MAXLL;
    bool is_neg_cycle_connect = false;
    bool check = false;
};

void dfs(std::vector<bellman_node>& v,const std::vector<std::vector<edge>>& f,int u) {
    if(v[u].check) return;
    v[u].check = true;
    v[u].is_neg_cycle_connect = true;
    for(int q = 0;q < f[u].size();q++) {
        dfs(v,f,f[u][q].to);
    }
}

void bellman_ford(std::vector<bellman_node>& v,const std::vector<std::vector<edge>>& f,int s) {
    v[s].min_distance = 0;
    int round = 0;
    int N = f.size();
    while(round < N - 1) {
        for(int q = 0;q < f.size();q++) {
            if(v[q].min_distance == MAXLL) continue;
            for(int w = 0;w <f[q].size();w++) {
                if(v[f[q][w].to].min_distance > v[q].min_distance + f[q][w].weight) {
                    v[f[q][w].to].min_distance = v[q].min_distance + f[q][w].weight;
                }
            }
        }    
        ++round;
    }
    std::vector<int> cycle_list;
    for(int q = 0;q < f.size();q++) {
        if(v[q].min_distance == MAXLL) continue;
        for(int w = 0;w < f[q].size();w++) {
            if(v[f[q][w].to].min_distance > v[q].min_distance + f[q][w].weight) {
                v[f[q][w].to].is_neg_cycle_connect = true;
                cycle_list.push_back(f[q][w].to);
            }
        }
    }
    for(int q = 0;q <cycle_list.size();q++) {
        dfs(v,f,cycle_list[q]);
    }
}

bool solve() {
    int N,M,Q,S;
    std::cin >> N >> M >> Q >> S;
    if(N == 0 && M == 0 && Q == 0 && S == 0) return false;
    std::vector<std::vector<edge>> f(N);
    for(int q = 0;q < M;q++) {
        int u,v,w;
        std::cin >> u >> v >> w;
        put_in.to = v;
        put_in.weight = w;
        f[u].push_back(put_in);
    }
    std::vector<bellman_node> v(N);
    bellman_ford(v,f,S);
    for(int q = 0;q < Q;q++) {
        int input;
        std::cin >> input;
        if(v[input].is_neg_cycle_connect) {
            std::cout << "-infinity\n";
        }
        else if(v[input].min_distance == MAXLL) {
            std::cout << "impossible\n";
        }
        else {
            std::cout << v[input].min_distance << "\n";
        }
    }
    return true;
}

int main() {
    while(solve()) {

    }
    return 0;
}
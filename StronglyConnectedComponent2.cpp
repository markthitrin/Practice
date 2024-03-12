// This code is tested by https://cses.fi/problemset/task/1683

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

class vertex{
public:
    int value = -1;
    int low = -1;
    int comp = -1;
};

void dfs1(int u,std::vector<vertex>& v,const std::vector<std::vector<int>>& f,int comp) {
    v[u].comp = comp;
    for(int q = 0;q < f[u].size();q++) {
        if(v[f[u][q]].comp == -1) {
            dfs1(f[u][q],v,f,comp);
        }
    }
}

void dfs2(int u,std::vector<vertex>& v,const std::vector<std::vector<int>>& f,int& count,int& comp) {
    v[u].value = count;
    v[u].low = count;
    ++count;
    for(int q = 0;q < f[u].size();q++) {
        if(v[f[u][q]].comp != -1)
            continue;
        if(v[f[u][q]].value == -1) {
            dfs2(f[u][q],v,f,count,comp);
        }
        v[u].low = std::min(v[u].low,v[f[u][q]].low);
    }
    if(v[u].low == v[u].value) {
        dfs1(u,v,f,comp);
        comp++;
    }
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> f(N);
    std::vector<vertex> v(N);
    for(int q = 0;q < M;q++){
        int u,v;
        std::cin >> u >> v;
        --u,--v;
        f[u].push_back(v);
    }
    int counter1 = 0;
    int comp = 0;
    for(int q = 0;q < N;q++) {
        if(v[q].comp == -1) {
            dfs2(q,v,f,counter1,comp);
        }
    }
    std::cout << comp << std::endl;
    for(int q = 0;q < v.size();q++) {
        std::cout << v[q].comp + 1 << " ";
    }
}

int main() {
    solve();
}
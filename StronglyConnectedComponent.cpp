// This code is tested by https://cses.fi/problemset/task/1683

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

void dfs1(int u,std::vector<bool>& check,const std::vector<std::vector<int>>& f,std::vector<int>& res) {
    if(check[u]) return;
    check[u] = true;
    for(int q = 0;q < f[u].size();q++) {
        dfs1(f[u][q],check,f,res);
    }
    res.push_back(u);
}

void dfs2(int u,std::vector<bool>& check,const std::vector<std::vector<int>>& f,int group,std::vector<int>& res) {
    if(check[u]) return;
    check[u] = true;
    res[u] = group;
    for(int q = 0;q < f[u].size();q++) {
        dfs2(f[u][q],check,f,group,res);
    }
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> f(N);
    std::vector<bool> check(N,false);
    std::vector<bool> check2(N,false);
    std::vector<int> stack1;
    std::vector<int> res(N);
    for(int q = 0;q < M;q++) {
        int u,v;
        std::cin >> u >> v;
        --u,--v;
        f[u].push_back(v);
    }
    for(int q = 0;q < N;q++) {
        dfs1(q,check,f,stack1);
    }
    std::vector<std::vector<int>> f2(N);
    for(int q = 0;q <f.size();q++) {
        for(int w = 0;w < f[q].size();w++) {
            f2[f[q][w]].push_back(q);
        }
    }
    int count = 0;
    while(stack1.size()) {
        int q = stack1.back();
        stack1.pop_back();
        if(!check2[q])
            dfs2(q,check2,f2,count++,res);
    }
    std::cout << count << "\n";
    for(int q = 0;q < res.size();q++) {
        std::cout << res[q] + 1 << " ";
    }
}

int main() {
    solve();
}
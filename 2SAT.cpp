// This code is tested by https://cses.fi/problemset/task/1684

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

void dfs1(int u,std::vector<bool>& check,std::vector<int>& order,const std::vector<std::vector<int>>& f) {
    if(check[u]) return;
    check[u] = true;
    order.push_back(u);
    for(int q = 0;q < f[u].size();q++) {
        dfs1(f[u][q],check,order,f);
    }
}

void dfs2(int u,int p,std::vector<int>& comp,const std::vector<std::vector<int>>& f) {
    if(comp[u] != -1) {
        return;
    }
    comp[u] = p;
    for(int q =  0;q < f[u].size();q++) {
        dfs2(f[u][q],p,comp,f);
    }
}

void solve() {
    int n,m;
    std::cin >> n >> m;
    std::vector<int> comp(2 * m);
    std::vector<bool> check(2 * m);
    std::vector<std::vector<int>> f(2*m);
    std::vector<int> get_order;
    for(int q = 0;q < n;q++) {
        char a1,a2;
        int b1,b2;
        std::cin >> a1 >> b1 >> a2 >> b2;
        --b1,--b2;
        int i1 = b1 * 2 + (a1 == '+'),i2 = b2 * 2 + (a2 == '-');
        f[i1].push_back(i2);
        f[i2].push_back(i1);
    }
    for(int q = 0;q < m;q++) {
        dfs1(q,check,get_order,f);
    }
    for(int q = get_order.size() - 1;q >= 0;q--) {
        
    }
}

int main() {
    solve();
}
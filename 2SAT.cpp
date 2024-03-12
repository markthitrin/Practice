// This coded is tested by https://cses.fi/problemset/task/1684

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

void dfs1(int u,std::vector<bool>& check,std::vector<int>& order,const std::vector<std::vector<int>>& f) {
    if(check[u]) return;
    check[u] = true;
    for(int q = 0;q < f[u].size();q++) {
        dfs1(f[u][q],check,order,f);
    }
    order.push_back(u);
}

void dfs2(int u,std::vector<int>& comp,const std::vector<std::vector<int>>& f,int comp_number) {
    if(comp[u] != -1) return;
    comp[u] = comp_number;
    for(int q =0;q <f[u].size();q++) {
        dfs2(f[u][q],comp,f,comp_number);
    }
}

int flip(int x) {
    return x ^ 1;
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> f(2*M);
    std::vector<std::vector<int>> inv_f(2*M);
    std::vector<bool> check(2*M,false);
    std::vector<int> order;
    std::vector<int> comp(2*M,-1);
    for(int q = 0;q < N;q++) {
        char a1,a2;
        int b1,b2;
        std::cin >> a1 >> b1 >> a2 >> b2;
        b1--,b2--;
        b1 = (b1 << 1) | (a1 == '-');
        b2 = (b2 << 1) | (a2 == '-');
        f[flip(b1)].push_back(b2);
        f[flip(b2)].push_back(b1);
        inv_f[b2].push_back(flip(b1));
        inv_f[b1].push_back(flip(b2));
    }
    for(int q = 0;q < 2*M;q++) {
        dfs1(q,check,order,f);
    }
    int comp_number = 0;
    for(int q =0;q <2*M;q++) {
        int select = order.back();
        order.pop_back();
        if(comp[select] == -1){
            dfs2(select,comp,inv_f,comp_number++);
        }
    }
    std::vector<bool> ans(M);
    bool is_yes = true;
    for(int q = 0;q < M;q++) {
        if(comp[q * 2] == comp[q * 2 + 1]) {
            is_yes = false;
        }
        ans[q] = comp[q * 2] > comp[q * 2 + 1];
    }
    if(is_yes) {
        for(int q = 0;q < ans.size();q++) {
            if(ans[q]) {
                std::cout << "+ ";
            }
            else {
                std::cout << "- ";
            }
        }
    }
    else {
        std::cout << "IMPOSSIBLE\n";
    }
}

int main() {
    solve();
}
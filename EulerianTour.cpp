// This code is tested by https://cses.fi/problemset/task/1691

#include <iostream>
#include <algorithm>
#include <vector>

using ll = long long;

class edge {
public:
    int to;
    int inv;
    bool enable = true;
}put_in;

class linklist {
public:
    int pos;
    linklist* right = nullptr;
};

void add_linklist(linklist* left,int new_pos) {
    linklist* right_handle = left->right;
    left->right = new linklist();
    left->right->pos = new_pos;
    left->right->right = right_handle;
}

void euler_tour(int u,linklist* left_node,std::vector<std::vector<edge>>& f,std::vector<int>& it) {
    add_linklist(left_node,u);
    while(it[u] < f[u].size()) {
        if(!f[u][it[u]].enable) {
            ++it[u];
            continue;
        }
        f[u][it[u]].enable = false;
        f[f[u][it[u]].to][f[u][it[u]].inv].enable = false;
        it[u]++;
        euler_tour(f[u][it[u]-1].to,left_node->right,f,it);
    }
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<edge>> f(N);
    std::vector<int> it(N,0);
    linklist* init = new linklist();
    for(int q = 0;q < M;q++) {
        int u,v;
        std::cin >> u >> v;
        --u,--v;
        put_in.to = v;
        put_in.inv = f[v].size();
        f[u].push_back(put_in);
        put_in.to = u;
        put_in.inv = f[u].size() - 1;
        f[v].push_back(put_in);
    }
    bool is_yes = false;
    for(int q = 0;q < f.size();q++) {
        if(f[q].size() % 2) is_yes = true;
    }
    euler_tour(0,init,f,it);
    int size = 0;
    linklist* ss = init->right;
    while(ss != nullptr) {
        size++;
        ss = ss->right;
    }
    if(size != M + 1) is_yes = true;
    if(!is_yes) {
        linklist* select = init->right;
        while(select != nullptr){
            std::cout << select->pos + 1 << " ";
            select = select->right;
        }
    }
    else {
        std::cout << "IMPOSSIBLE";
    }
    std::cout << "\n";
}

int main() {
    solve();
    return 0;
}
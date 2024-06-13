// This code is tested by https://cses.fi/problemset/task/1694

#include <iostream>
#include <vector>
#include <queue>

using ll = long long;

class edge {
public:
    int to;
    ll cap;
    int inv;
} put_in;

bool fetch(std::vector<std::vector<edge>>& f,ll& res) {
    std::queue<edge*> g;
    std::vector<bool> visited(f.size(),false);
    std::vector<edge*> gg(f.size(), nullptr);
    g.push(nullptr);
    while(g.size()) {
        edge* select = g.front();
        g.pop();
        int i = 0;
        if(select) i = select->to;
        if(visited[i]) continue;
        visited[i] = true;
        for(int q = 0;q < f[i].size();q++) {
            if(f[i][q].cap > 0 && !gg[f[i][q].to] && f[i][q].to != 0) {
                gg[f[i][q].to] = &f[i][q];
                g.push(&f[i][q]);
            }
        }
    }

    int N = f.size();

    if(gg[N - 1] == nullptr) return false;

    ll get_min = 1000000000;
    edge* p;
    p = gg[N - 1];
    while(p) {
        get_min = std::min(p->cap,get_min);
        p = gg[f[p->to][p->inv].to];
    }

    p = gg[N - 1];
    while(p) {
        p->cap -= get_min;
        f[p->to][p->inv].cap += get_min;
        p = gg[f[p->to][p->inv].to];
    }

    res += get_min;
    
    return true;
}

int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<edge>> f(N);
    for(int q = 0;q < M;q++) {
        ll u,v,c;
        std::cin >> u >> v >> c;
        --u,--v;
        put_in.to = v;
        put_in.cap = c;
        put_in.inv = f[v].size();
        f[u].push_back(put_in);
        put_in.to = u;
        put_in.cap = 0;
        put_in.inv = f[u].size() - 1;
        f[v].push_back(put_in);
    }
    ll res = 0;
    while(fetch(f,res)) {
       
    }
    std::cout << res << std::endl;
    return 0;
}

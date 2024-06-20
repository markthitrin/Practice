// This code is tested by https://open.kattis.com/problems/thewrathofkahn

#include <iostream>
#include <vector>
#include <queue>

using ll = long long;

class edge{
public:
    int to = 0;
    ll cap;
    int inv;
};

int count_dfs = 0;

void fix(edge* e,std::vector<std::vector<edge>>& f,ll value) {
    e->cap -= value;
    f[e->to][e->inv].cap += value;
}

ll dfs(int u,std::vector<std::vector<edge*>>& fp,std::vector<std::vector<edge>>& f,std::vector<bool>& check, ll get_min = 1000000000000) {
    if(u == fp.size() - 1) {
        return get_min;
    }
    else if(check[u]) {
        return -1;
    }
    check[u] = true;
    for(int q = 0;q < fp[u].size();q++){
        ll flow = dfs(fp[u][q]->to,fp,f,check,std::min(get_min,fp[u][q]->cap));
        if(flow > 0) {
            fix(fp[u][q],f,flow);
            return flow;
        }
    }
    return -1;
}

ll get_max_flow(std::vector<std::vector<edge>>& f) {
    const int N = f.size();
    ll res = 0;
    std::vector<std::vector<edge*>> f_level(N);
    std::queue<int> bfs;
    std::vector<int> level(N,-1);
    std::vector<bool> check(N,false);
    while(true) {
        for(int q = 0;q < N;q++) {
            f_level[q].clear();
            level[q] = -1;
            check[q] = false;
        }
        bfs.push(0);
        level[0] = 0;
        while(bfs.size()) {
            int select = bfs.front();
            bfs.pop();
            for(int q = 0;q < f[select].size();q++) {
                if(f[select][q].cap && (level[f[select][q].to] == -1 || level[f[select][q].to] - 1 == level[select])) {
                    f_level[select].push_back(&f[select][q]);
                    if(level[f[select][q].to] == -1) 
                        bfs.push(f[select][q].to);
                    level[f[select][q].to] = level[select] + 1;
                }
            }
        }
        if(level[N - 1] == -1) break;
        ll flow = dfs(0,f_level,f,check);
        res += flow;
    }
    return res;
}

void add_edge(int u,int v,int cap,std::vector<std::vector<edge>>& f) {
    edge e;
    e.to = v;
    e.inv = f[v].size();
    e.cap = cap;
    f[u].push_back(e);
    e.to = u;
    e.inv = f[u].size() - 1;
    e.cap = 0;
    f[v].push_back(e);
}

int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> _f(2*N + 2);
    std::vector<std::vector<edge>> f(2*N + 2);
    std::vector<int> in(2*N + 2);
    for(int q = 0;q < M;q++) {
        int u,v;
        std::cin >> u >> v;
        _f[u].push_back(v);
        in[v]++;
    }
    for(int _ = 0;_ < N;_++) {
        for(int q = 0;q < N;q++) {
            if(in[q] == 0) {
                for(int w = 0;w < _f[q].size();w++) {
                    add_edge(q + 1,N + _f[q][w] + 1,1,f);
                    in[_f[q][w]]--;
                }
            }
        }
    }
    for(int q = 0;q < N;q++) {
        add_edge(0,q + 1,1,f);
        add_edge(q + N + 1,2 * N + 1,1,f);
    }
    int flow = get_max_flow(f);
    std::cout << N - flow << std::endl;
    return 0;
}
// This code is tested by https://cses.fi/problemset/task/1694

#include <iostream>
#include <vector>

using ll = long long;

class edge{
public:
    int u,v;
    ll cap;
    ll flow;
    int connect(int from) {
        if(from == u) return v;
        return u;
    }
    ll avaliable_flow(int from) {
        if(from == u) return cap - flow;
        return flow;
    }
    void fix(int from,ll value) {
        if(from == u) flow += value;
        else flow -= value;
    }

};

ll dfs(int u,std::vector<std::vector<edge*>>& f,std::vector<bool>& visited,ll& theshold,ll flow = 1000000000000) {
    visited[u] = true;
    if(u == f.size() - 1) 
        return flow;
    for(int q = 0;q < f[u].size();q++) {
        if(!visited[f[u][q]->connect(u)] && f[u][q]->avaliable_flow(u) >= theshold) {
            int r = dfs(f[u][q]->connect(u),f,visited,theshold,std::min(f[u][q]->avaliable_flow(u),flow));
            if(r > 0) {
                f[u][q]->fix(u,r);
                return r;
            }
        }
    }
    return 0;
}

bool max_flow(std::vector<std::vector<edge*>>& f,ll& theshold,ll& res) {
    std::vector<bool> visited(f.size(),false);
    ll get_flow = dfs(0,f,visited,theshold);
    res += get_flow;
    return get_flow;
}

int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<edge*>> f(N);
    ll theshold = 0;
    ll res = 0;
    for(int q = 0;q < M;q++) {
        edge* e = new edge();
        std::cin >> e->u >> e->v;
        e->u--,e->v--;
        std::cin>> e->cap;
        theshold += e->cap;
        f[e->u].push_back(e);
        f[e->v].push_back(e);
    }
    while(theshold) {
        if(!max_flow(f,theshold,res)) {
            theshold >>= 1;
        }
    }
    std::cout << res << std::endl;
}
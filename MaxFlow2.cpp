// This code is tested by https://cses.fi/problemset/task/1694

#include <iostream>
#include <vector>
#include <queue>

using ll = long long;

class edge{
public:
    ll cap;
    ll flow = 0;
    int u;
    int v;
    ll avaliable_flow(int from) {
        if(from == u) return cap - flow;
        return flow;
    }
    int connect(int from) {
        if(from == u) return v;
        return u;
    }
    void fix(int from,int value) {
        if(from == u) flow += value;
        else flow -= value;
    }
};

ll get_max_flow(std::vector<std::vector<edge*>>& f) {
    std::vector<int> h(f.size());
    std::vector<ll> ex(f.size());
    std::queue<int> g;
    int N = f.size();
    h[0] = N;
    h.back() = 0;
    ex[0] = 1000000000000000;
    g.push(0);
    while(g.size()) {
        int select = g.front();
        g.pop();
        if(ex[select] == 0 || select == N - 1) continue;
        bool found_edge = false;
        for(int q = 0;q < f[select].size() && ex[select] > 0;q++) {
            if(h[select] > h[f[select][q]->connect(select)] && f[select][q]->avaliable_flow(select)) {
                int d = std::min(ex[select],f[select][q]->avaliable_flow(select));
                ex[select] -= d;
                ex[f[select][q]->connect(select)] += d;
                f[select][q]->fix(select,d);
                if(ex[f[select][q]->connect(select)] == d)
                    g.push(f[select][q]->connect(select));
                found_edge = true;
            }
        }
        if(!found_edge) {
            h[select]++;
        }
        if(select != 0 && ex[select]) g.push(select);
    }
    return ex.back();
}

int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<edge*>> f(N);
    for(int q = 0;q < M;q++) {
        edge* e = new edge();
        std::cin >> e->u >> e->v;
        e->u--,e->v--;
        std::cin >> e->cap;
        f[e->u].push_back(e);
        f[e->v].push_back(e);
    }
    std::cout << get_max_flow(f) << std::endl;
    return 0;
}


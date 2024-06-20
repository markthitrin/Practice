// This code is tested by https://dmoj.ca/problem/wac4p6

#include <iostream>
#include <vector>
#include <queue>

using ll = long long;

class edge{
public:
    int to;
    ll cap;
    int inv;
};

void add_edge(int u,int v,ll cap, std::vector<std::vector<edge>>& f) {
    edge e;
    e.to = v;
    e.cap = cap;
    e.inv = f[v].size();
    f[u].push_back(e);
    e.to = u;
    e.cap = 0;
    e.inv = f[u].size() - 1;
    f[v].push_back(e);
}

void add_flow(edge& e,ll value,std::vector<ll>& ex,std::vector<std::vector<edge>>& f) {
    e.cap -= value;
    f[e.to][e.inv].cap += value;
    ex[f[e.to][e.inv].to] -= value;
    ex[e.to] += value;
}

void max_flow(int start,int end,std::vector<ll>& ex,std::vector<std::vector<edge>>& f) {
    const int N = f.size();
    std::queue<int> b;
    std::vector<int> level(N);
    level[start] = N;
    for(int q = 0;q < f[start].size();q++) {
        add_flow(f[start][q],f[start][q].cap,ex,f);
    }
    for(int q = 0;q < ex.size();q++) {
        if(ex[q] > 0) b.push(q);
    }
    while(b.size()) {
        int u = b.front();
        b.pop();
        if(u == start || u == end) continue;
        if(ex[u] <= 0) continue;
        for(int q = 0;q < f[u].size() && ex[u] > 0;q++) {
            if(f[u][q].cap && level[f[u][q].to] < level[u]) {
                bool was_none = ex[f[u][q].to] <= 0;
                add_flow(f[u][q],std::min(ex[u],f[u][q].cap),ex,f);
                if(was_none && ex[f[u][q].to]) {
                    b.push(f[u][q].to);
                }
            }
        }
        if(ex[u]) {
            level[u]++;
            if(level[u] == 2 * N + 1) return;
            b.push(u);
        }
    }
}

void solve() {
    ll min_ans,max_ans;
    int N,M;
    std::cin >> N >> M;
    std::vector<int> a(N),b(N);
    std::vector<int> e(M),f(M);
    std::vector<std::vector<int>> c(N,std::vector<int>(M)),d(N,std::vector<int>(M));
    for(int q = 0;q < N;q++) {
        std::cin >> a[q] >> b[q];
    }
    for(int q = 0;q <N;q++) {
        for(int w =0;w < M;w++) {
            std::cin >> c[q][w];
        }
    }
    for(int q = 0;q < N;q++) {
        for(int w = 0;w < M;w++) {
            std::cin >> d[q][w];
        }
    }
    for(int q = 0;q < M;q++){
        std::cin >> e[q] >> f[q];
    }

    std::vector<std::vector<edge>> f1(2 + N + M);
    std::vector<ll> ex1(2 + N + M,0);
    for(int q = 0;q < N;q++) {
        add_edge(0,q + 1,b[q] - a[q],f1);
        ex1[q + 1] += a[q];
    }
    for(int q = 0;q < N;q++) {
        for(int w = 0;w < M;w++) {
            add_edge(q + 1,N + w + 1,d[q][w] - c[q][w],f1);
            ex1[q + 1] -= c[q][w];
            ex1[w + N + 1] += c[q][w];
        }
    }
    for(int q = 0;q < M;q++) {
        add_edge(N + q + 1,N + M + 1,f[q] - e[q],f1);
        ex1[q + 1 + N] -= e[q];
        ex1[N + M + 1] += e[q];
    }
    max_flow(0,N + M + 1,ex1,f1);
    max_ans = ex1.back();
    bool is_yes = true;
    for(int q = 1;q < ex1.size() - 1;q++) if(ex1[q] != 0) is_yes = false;
    if(!is_yes) {
        std::cout << -1 << std::endl;
        return;
    }

    std::vector<ll> ex2(ex1.size(),0);
    max_flow(N + M + 1,0,ex2,f1);
    min_ans = max_ans - ex2[0];

    std::cout << min_ans << "\n" << max_ans << std::endl;
    return;
}

int main() {
    solve();
    return 0;
}
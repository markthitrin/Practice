// This code is tested by https://codeforces.com/contest/818/problem/G

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <bits/extc++.h>

#define all(v) v.begin(),v.end()
#define rep(i,s,t) for(int i = s;i < t;i++)

using ll = long long;
using namespace std;

const ll INF = numeric_limits<ll>::max() / 4;
typedef vector<ll> VL;
typedef vector<int> vi;
typedef std::pair<int,int> pii;

struct MCMF {
    int N;
    vector<vi> ed, red;
    vector<VL> cap,flow,cost;
    vi seen;
    VL dist, pi;
    vector<pii> par;

    MCMF(int N) : N(N), ed(N), red(N), cap(N, VL(N)), flow(cap), cost(cap),
    seen(N), dist(N), pi(N), par(N) {}

    void addEdge(int from,int to,ll cap,ll cost) {
        this->cap[from][to] = cap;
        this->cost[from][to] = cost;
        ed[from].push_back(to);
        red[to].push_back(from);
    }

    void path(int s) {
        fill(all(seen), 0);
        fill(all(dist), INF);
        dist[s] = 0; ll di;

        __gnu_pbds::priority_queue<pair<ll, int>> q;
        vector<decltype(q)::point_iterator> its(N);
        q.push({0,s});

        auto relax = [&](int i,ll cap,ll cost, int dir) {
            ll val = di - pi[i] + cost;
            if(cap && val < dist[i]) {
                dist[i] = val;
                par[i] = {s, dir};
                if(its[i] == q.end()) its[i] = q.push({-dist[i], i});
                else q.modify(its[i], {-dist[i],i});
            }
        };

        while(!q.empty()) {
            s = q.top().second; q.pop();
            seen[s] = 1; di = dist[s] + pi[s];
            for(int i : ed[s]) if(!seen[i])
                relax(i, cap[s][i] - flow[s][i], cost[s][i], 1);
            for(int i : red[s]) if(!seen[i])
                relax(i, flow[i][s], -cost[i][s], 0);
        }
        rep(i,0,N) pi[i] = min(pi[i] + dist[i], INF);
    }

    pair<ll,ll> maxflow(int s,int t) {
        ll totflow = 0, totcost = 0;
        while(path(s),seen[t]) {
            ll fl = INF;
            for(int p,r,x = t;tie(p,r) = par[x],x != s;x = p) 
                fl = min(fl, r ? cap[p][x] - flow[p][x] : flow[x][p]);
            totflow += fl;
            for(int p,r,x = t;tie(p,r) = par[x], x != s; x = p)
                if(r) flow[p][x] += fl;
                else flow[x][p] -= fl;
        }
        rep(i,0,N) rep(j,0,N) totcost += cost[i][j] * flow[i][j];
        return {totflow, totcost};
    }

    //if some costs can be negatuve, call this before maxflow
    void setpi(int s) {
        fill(all(pi),INF); pi[s] = 0;
        int it = N, ch = 1; ll v;
        while(ch-- && it--)
            rep(i,0,N) if(pi[i] != INF)
                for(int to : ed[i]) if(cap[i][to])
                    if((v = pi[i] + cost[i][to]) < pi[to])
                        pi[to] = v, ch = 1;
        assert(it >= 0); //negative cost cycle
    }
};


void solve() {
    int N;
    std::cin >> N;
    std::vector<int> a(N);
    for(int q = 0;q < N;q++) {
        std::cin >> a[q];
    }
    MCMF graph(2 * N + 3);
    graph.addEdge(2 * N,2 * N + 1,4,0);
    for(int q = 0;q < N;q++) {
        graph.addEdge(q,q + N,1,-1);
        graph.addEdge(q + N,2 * N + 2,1,0);
        graph.addEdge(2 * N + 1,q,1,0);
    }
    for(int q = 0;q < N;q++) {
        for(int w = q + 1;w < N;w++) {
            if(a[q] - a[w] == 1) {
                graph.addEdge(q + N,w,1,0);
                graph.addEdge(q,w,1,0);
                break;
            }
        }

        for(int w = q + 1;w < N;w++) {
            if(a[q] - a[w] == -1) {
                graph.addEdge(q + N,w,1,0);
                graph.addEdge(q,w,1,0);
                break;
            }
        }

        for(int w = q + 1;w < N;w++) {
            if((a[q] - a[w]) % 7 == 0) {
                graph.addEdge(q + N,w,1,0);
                graph.addEdge(q,w,1,0);
                break;
            }
        }
    }
    graph.setpi(2 * N);
    auto sss = graph.maxflow(2 * N,2 * N + 2);
    std::cout << -sss.second << std::endl;
}

int main() {
    solve();
}
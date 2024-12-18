// This code is tested by https://dmoj.ca/problem/wac4p6

#include <iostream>
#include <vector>
#include <queue>
#define vi std::vector<int>
#define sz(v) v.size()
#define rep(i,a,b) for(int i = a;i < b;i++)
using namespace std;

using ll = long long;

struct PushRelabel {
struct Edge {
int dest, back;
ll f, c;
};
vector<vector<Edge>> g;
vector<ll> ec;
vector<Edge*> cur;
vector<vi> hs; vi H;
PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}
void addEdge(int s, int t, ll cap, ll rcap=0) {
if (s == t) return;
Edge e;
e.dest = t;e.back = sz(g[t]);e.f = 0;e.c = cap;
g[s].push_back(e);
e.dest = s;e.back = sz(g[s]) - 1;e.f = 0;e.c = rcap;
g[t].push_back(e);
}
void addFlow(Edge& e, ll f) {
Edge &back = g[e.dest][e.back];
if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
e.f += f; e.c -= f; ec[e.dest] += f;
back.f -= f; back.c += f; ec[back.dest] -= f;
}
ll calc(int s, int t) {
int v = sz(g); H[s] = v; ec[t] = 1;
vi co(2*v); co[0] = v-1;
rep(i,0,v) cur[i] = g[i].data();
for (Edge& e : g[s]) addFlow(e, e.c);
for (int hi = 0;;) {
while (hs[hi].empty()) if (!hi--) return -ec[s];
int u = hs[hi].back(); hs[hi].pop_back();
while (ec[u] > 0) // discharge u
if (cur[u] == g[u].data() + sz(g[u])) {
H[u] = 1e9;
for (Edge& e : g[u]) if (e.c && H[u] > H[e.dest]+1)
H[u] = H[e.dest]+1, cur[u] = &e;
if (++co[H[u]], !--co[hi] && hi < v)
rep(i,0,v) if (hi < H[i] && H[i] < v)
--co[H[i]], H[i] = v + 1;
hi = H[u];
} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
addFlow(*cur[u], min(ec[u], cur[u]->c));
else ++cur[u];
}
}
bool leftOfMinCut(int a) { return H[a] >= sz(g); }
};


int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<ll> a(N),b(N),e(M),f(M);
    std::vector<std::vector<ll>> c(N,std::vector<ll>(M)),d(N,std::vector<ll>(M));
    std::vector<ll> in(N + M + 2);
    std::vector<ll> out(N + M + 2);
    for(int q = 0;q < N;q++) {
        std::cin >> a[q] >> b[q];
        out[0] += a[q];
        in[q + 1] += a[q];
    }
    for(int q = 0;q < N;q++) {
        for(int w = 0;w < M;w++) {
            std::cin >> c[q][w];
            out[q + 1] += c[q][w];
            in[w + N + 1] += c[q][w];
        }
    }
    for(int q = 0;q < N;q++) {
        for(int w = 0;w < M;w++) {
            std::cin >> d[q][w];
        }
    }
    for(int q = 0;q < M;q++) {
        std::cin >> e[q] >> f[q];
        in[M + N + 1] += e[q];
        out[q + N + 1] += e[q]; 
    }

    ll sum_in = 0;
    PushRelabel PR(N + M + 4);
    PR.addEdge(N + M + 1,0,1e18);
    for(int q = 0;q < N;q++) {
        PR.addEdge(0,q + 1,b[q] - a[q]);
    }
    for(int q = 0;q < N;q++) {
        for(int w = 0;w < M;w++) {
            PR.addEdge(q + 1,w + N + 1,d[q][w] - c[q][w]);
        }
    }
    for(int q = 0;q < M;q++) {
        PR.addEdge(q + N + 1,N + M + 1,f[q] - e[q]);
    }
    for(int q = 0;q < N + M + 2;q++) {
        PR.addEdge(N + M + 2,q,in[q]);
        PR.addEdge(q,N + M + 3,out[q]);
        sum_in += in[q];
    }
    ll flow = PR.calc(N + M + 2,N + M + 3);
    if(flow != sum_in) {
        std::cout << -1 << std::endl;
        return 0;
    }

    ll default_flow = 0; 
    for(int q = 0;q < PR.g[N + M + 1].size();q++) {
        if(PR.g[N + M + 1][q].dest == 0) {
            default_flow = PR.g[N + M + 1][q].f;
            break;
        }
    }
    
    PushRelabel maxPR(N + M + 4);
    for(int q = 0;q < N + M + 4;q++) {
        for(int w = 0;w < PR.g[q].size();w++) {
            PushRelabel::Edge& e = PR.g[q][w];
            if(q == N + M + 1 && e.dest == 0) {
                continue;
            }
            if(q == 0 && e.dest == N + M + 1) {
                continue;
            }
            maxPR.addEdge(q,e.dest,e.c);
        }
    }
    ll max_flow_ans = maxPR.calc(0,N + M + 1);
    max_flow_ans += default_flow;

    PushRelabel minPR(N + M + 4);
    for(int q = 0;q < N + M + 4;q++) {
        for(int w = 0;w < PR.g[q].size();w++) {
            PushRelabel::Edge& e = PR.g[q][w];
            if(q == 0 && e.dest == N + M + 1) {
                continue;
            }
            if(q == N + M + 1 && e.dest == 0) {
                continue;
            }
            minPR.addEdge(q,e.dest,e.c);
        }
    }
    ll min_flow_ans = default_flow;
    min_flow_ans -= minPR.calc(N + M + 1,0);

    std::cout << min_flow_ans << std::endl << max_flow_ans << std::endl;
    return 0;
}
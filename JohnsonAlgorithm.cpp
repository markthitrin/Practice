#include <iostream>
#include <vector>
#include <queue>

using ll = long long;
using ull = unsigned ll;

class bellman{
public:
    class edge{
    public:
        int to;
        ll distance;
    };
    std::vector<std::vector<edge>> f;
    bellman(int N) : f(N) {}
    void add_edge(int from,int to,int distance,bool bidirection = false) {
        edge e;
        e.to = to;
        e.distance = distance;
        f[from].push_back(e);
        if(bidirection) {
            e.to = from;
            e.distance = distance;
            f[to].push_back(e);
        }
    }
    std::vector<ll> calc(int s) {
        std::vector<ll> ans(f.size(),1e18);
        ans[s] = 0;
        for(int q = 0;q < ans.size();q++) {
            for(int w = 0;w < ans.size();w++) {
                for(int e = 0;e < f[w].size();e++) {
                    if(ans[w] == 1e18) continue;
                    if(ans[w] + f[w][e].distance < ans[f[w][e].to]) {
                        ans[f[w][e].to] = ans[w] + f[w][e].distance;
                    }
                }
            }
        }
        return ans;
    }
};

class dijkstra{
public:
    class edge{
    public:
        int to;
        ll distance;
    };
    class queue_node{
    public:
        int node;
        ll distance;
        bool operator<(const queue_node& other) const{
            return distance > other.distance;
        }
    } put_in;
    std::vector<std::vector<edge>> f;
    dijkstra(int N) : f(N) {}
    void add_edge(int from,int to,ll distance,bool bidirection = false) {
        edge e;
        e.to = to;
        e.distance = distance;
        f[from].push_back(e);
        if(bidirection) {
            e.to = from;
            e.distance = distance;
            f[to].push_back(e);
        }
    }
    std::vector<ll> calc(int s) {
        std::vector<ll> ans(f.size(),1e18);
        std::priority_queue<queue_node> g;
        ans[s] = 0; put_in.node = s,put_in.distance = 0;
        g.push(put_in);
        while(g.size()) {
            int u = g.top().node;
            int d = g.top().distance;
            g.pop();
            if(d != ans[u]) continue;
            for(int q = 0;q < f[u].size();q++) {
                if(ans[u] + f[u][q].distance < ans[f[u][q].to]) {
                    ans[f[u][q].to] = ans[u] + f[u][q].distance;
                    put_in.node = f[u][q].to;
                    put_in.distance = ans[f[u][q].to];
                    g.push(put_in);
                }
            }
        }
        return ans;
    }
};

class johnson{
public:
    class edge{
    public:
        int to;
        ll distance;
    };
    std::vector<std::vector<edge>> f;
    std::vector<ll> pot;
    johnson(int N) : f(N) {}
    void add_edge(int from,int to,ll distance,bool bidirection = false) {
        edge e;
        e.to = to;
        e.distance = distance;
        f[from].push_back(e);
        if(bidirection) {
            e.to = from;
            e.distance = distance;
            f[to].push_back(e);
        }
    }
    void calc1() {
        bellman bm(f.size() + 1);
        for(int q = 0 ;q < f.size();q++) {
            bm.add_edge(f.size(),q,0);
            for(int w = 0;w < f[q].size();w++) {
                bm.add_edge(q,f[q][w].to,f[q][w].distance);
            }
        }
        pot = bm.calc(f.size());
    }
    std::vector<ll> calc2(int s) {
        std::vector<ll> ans;
        dijkstra dj(f.size());
        for(int q = 0;q < f.size();q++) {
            for(int w = 0;w < f[q].size();w++) {
                dj.add_edge(q,f[q][w].to,f[q][w].distance + pot[q] - pot[f[q][w].to]);
            }
        }
        ans = dj.calc(s);
        for(int q = 0;q < ans.size();q++) {
            ans[q] -= pot[s];
            ans[q] += pot[q];
        }
        return ans;
    }
};

class floyd{
public:
    std::vector<std::vector<ll>> f;
    floyd(int N) : f(N,std::vector<ll>(N,1e18)) {}
    void add_edge(int from,int to,ll distance,int bidirection = false) {
        f[from][to] = distance;
        if(bidirection) f[to][from] = distance;
    }
    std::vector<std::vector<ll>> calc() {
        for(int q = 0;q < f.size();q++) {
            f[q][q] = 0;
        }
        for(int q = 0;q < f.size();q++) {
            for(int w = 0;w < f.size();w++) {
                for(int e = 0;e < f.size();e++) {
                    if(f[w][q] != 1e18 && f[q][e] != 1e18) {
                        if(f[w][q] + f[q][e] < f[w][e]) {
                            f[w][e] = f[w][q] + f[q][e];
                        }
                    }
                }
            }
        }
        return f;
    }
};

ll get_rand_value() {
    #define UR (ll)std::rand()
    return UR << 49 | UR << 35 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int N = (ull)get_rand_value() % 100 + 100;
    int M = (ull)get_rand_value() % 10;
    std::vector<std::vector<ll>> f(N,std::vector<ll>(N,1e18));
    int v;
    for(int q = 1;q < N;q++) {
        for(int w = 0;w < M;w++) {
            v = (ull)get_rand_value() % q;
            f[q][v] = (ull)get_rand_value() % 100000;
            f[v][q] = f[q][v];
        }
    }
    floyd F(N);
    johnson J(N);
    for(int q = 0;q < N;q++) {
        for(int w = 0;w < N;w++) {
            if(f[q][w] != 1e18) {
                F.add_edge(q,w,f[q][w]);
                J.add_edge(q,w,f[q][w]);
            }
        }
    }
    J.calc1();
    std::vector<std::vector<ll>> ans = F.calc();
    for(int q = 0;q < N;q++) {
        std::vector<ll> get_ans = J.calc2(q);
        for(int w = 0;w < N;w++) {
            if(get_ans[w] != ans[q][w]) 
                return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int i= 1;i <=_;i++) {
        std::cout << "testcase " << i << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class edge{
public:
    ll to;
    ll distance;
    edge() {}
    edge(ll _to,ll _distance) : to(_to), distance(_distance) {}
};

class vertex1{
public:
    ll v;
    ll distance;
    vertex1() {;}
    vertex1(ll _v,ll _distance) : v(_v) , distance(_distance) {}
    bool operator<(const vertex1& other) const{
        return distance > other.distance;
    }
};

ll dijkstra(const std::vector<std::vector<edge>>& f,ll s,ll e) {
    std::vector<bool> check(f.size(),false);
    std::vector<ll> distance(f.size(),MAXINT);
    std::priority_queue<vertex1> g;
    g.push(vertex1(s,0));
    distance[s] = 0;
    while(!g.empty()) {
        vertex1 select = g.top();
        g.pop();
        if(check[select.v]) continue;
        check[select.v] = true;

        for(int q = 0;q < f[select.v].size();q++) {
            const ll& to = f[select.v][q].to;
            if(distance[to] > select.distance + f[select.v][q].distance) {
                distance[to] = select.distance + f[select.v][q].distance;
                g.push(vertex1(to,distance[to]));
            }
        }
    }
    return distance[e];
}

ll floyd_warshell(const std::vector<std::vector<edge>>& f,ll s,ll e) {
    std::vector<std::vector<ll>> distance(f.size(),std::vector<ll>(f.size(),MAXINT));
    for(int q = 0;q < f.size();q++) {
        distance[q][q] = 0;
        for(int w = 0;w < f[q].size();w++) {
            distance[q][f[q][w].to] = f[q][w].distance;
        }
    }
    for(int w = 0;w < f.size();w++) {
        for(int q = 0;q < f.size();q++) {
            for(int e = q + 1;e < f.size();e++) {
                distance[q][e] = std::min(distance[q][w] + distance[w][e],distance[q][e]);
                distance[e][q] = distance[q][e];
            }
        }
    }
    return distance[s][e];
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll number = (ull)get_rand_value() % 100;
    std::vector<std::vector<edge>> f(number,std::vector<edge>());
    for(int q = 1;q < number;q++) {
        ll parent = (ull)get_rand_value() % (q);
        ll distance = (ull)get_rand_value() % 1000;
        f[parent].push_back(edge(q,distance));
        f[q].push_back(edge(parent,distance));
    }
    for(int q = 0;q < 50;q++) {
        ll u = (ull)get_rand_value() % (number);
        ll v = (ull)get_rand_value() % number;
        ll distance = (ull)get_rand_value() % 1000;

        bool do_exist = u == v;
        for(int q = 0;q < f[u].size();q++) do_exist |= f[u][q].to == v;
        if(do_exist) continue;

        f[u].push_back(edge(v,distance));
        f[v].push_back(edge(u,distance));
    }
    for(int q = 0;q < 100;q++) {
        ll u = (ull)get_rand_value() % number;
        ll v = (ull)get_rand_value() % number;
        if(floyd_warshell(f,u,v) != dijkstra(f,u,v)) return false;
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++) {
        std::cout << "testcas " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
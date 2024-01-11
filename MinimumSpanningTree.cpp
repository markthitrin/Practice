#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

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
    bool operator<(const edge& other) const{
        return distance > other.distance;
    }
};

class edge1{
public:
    ll from;
    ll to;
    ll distance;
    edge1() {}
    edge1(ll _from,ll _to,ll _distance) : to(_to), from(_from), distance(_distance) {}
    bool operator<(const edge1& other) const{
        return distance > other.distance;
    }  
};

ll prim_spanning(const std::vector<std::vector<edge>>& f) {
    ll sum_res = 0;
    std::priority_queue<edge> g;
    g.push(edge(0,0));
    std::vector<bool> check(f.size(),false);
    ll check_count = 0;
    while(check_count < f.size()) {
        edge select = g.top();
        g.pop();
        if(check[select.to]) continue;
        check[select.to] = true;
        check_count++;
        sum_res += select.distance;
        for(int q = 0;q < f[select.to].size();q++) {
            if(!check[f[select.to][q].to]) {
                g.push(f[select.to][q]);
            }
        }
    }
    return sum_res;
}

ll get_head(std::vector<ll>& head,ll a) {
    std::vector<ll> c;
    while(head[a] != a) {
        c.push_back(a);
        a = head[a];
    }
    for(int q = 0;q < c.size();q++) {
        head[c[q]] = a;
    }
    return a;
}

void join(std::vector<ll>& head,ll a,ll b) {
    ll heada = get_head(head,a);
    ll headb = get_head(head,b);
    head[heada] = headb;
}

bool check_if_connect(std::vector<ll>& head,ll a,ll b) {
    ll heada = get_head(head,a);
    ll headb = get_head(head,b);
    return heada == headb;
}

ll kaskal_spanning(const std::vector<std::vector<edge>>& f) {
    ll res_sum = 0;
    std::vector<ll> head;
    for(int q = 0;q < f.size();q++) {
        head.push_back(q);
    }
    std::priority_queue<edge1> g;
    for(int q = 0;q < f.size();q++) {
        for(int w = 0;w < f[q].size();w++) {
            g.push(edge1(q,f[q][w].to,f[q][w].distance));
        }
    }
    while(!g.empty()) {
        edge1 select = g.top();
        g.pop();
        if(check_if_connect(head,select.from,select.to)) continue;
        join(head,select.from,select.to);
        res_sum += select.distance;
    }
    return res_sum;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll number = (ull)get_rand_value() % 1000000;
    std::vector<std::vector<edge>> f(number,std::vector<edge>());
    for(int q = 1;q < number;q++) {
        ll parent = (ull)get_rand_value() % q;
        ll distance = (ull)get_rand_value() % 1000;
        f[parent].push_back(edge(q,distance));
        f[q].push_back(edge(parent,distance));
    }
    for(int q = 0;q < number / 100;q++) {
        ll u = (ull)get_rand_value() % (number);
        ll v = (ull)get_rand_value() % (number);
        ll distance = (ull)get_rand_value() % 1000;

        bool is_exist = u == v;
        for(int q = 0;q < f[u].size();q++) 
            if(f[u][q].to == v) is_exist = true;
        if(is_exist) continue;

        f[u].push_back(edge(v,distance));
        f[v].push_back(edge(u,distance));
    }

    return kaskal_spanning(f) == prim_spanning(f);
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
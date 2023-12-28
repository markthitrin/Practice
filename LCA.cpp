#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class edge {
    public:
    ll to;
    ll distance;
};

class vertex {
public:
    ll value = 0;
    ll level = 0;
    std::vector<edge> parents;
    std::vector<ll> min_vertex;
};

class result_class{
    public:
    ll lca = 0;
    ll distance = 0;
    ll min_value = MAXINT;
};

std::vector<std::vector<edge>> f;
std::vector<vertex> v;

void recur_build(ll u,ll p,ll dis) {
    v[u].level = v[p].level + 1;
    v[u].parents.push_back({p,dis});
    v[u].min_vertex.push_back(std::min(v[u].value,v[p].value));

    ll pp = p;
    ll ii = 0;
    while(v[pp].parents.size() > ii) {
        v[u].parents.push_back({v[pp].parents[ii].to,v[pp].parents[ii].distance + v[u].parents.back().distance});
        v[u].min_vertex.push_back(std::min(v[u].min_vertex.back(),v[pp].min_vertex[ii]));
        pp = v[u].parents.back().to;
        ii++;
    }

    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p)
            recur_build(f[u][q].to,u,f[u][q].distance);
    }
}

void build(ll root) {
    for(int q = 0;q < f[root].size();q++) {
        recur_build(f[root][q].to, root, f[root][q].distance);
    }
}

result_class LCA(ll x,ll y) {
    result_class res;
    if(v[y].level < v[x].level) std::swap(x,y);
    ll level_dff = v[y].level - v[x].level;
    for(int q = 30;q>=0;q--) {
        if((1 << q) & (level_dff)) {
            level_dff -= (1 << q);
            res.distance += v[y].parents[q].distance;
            res.min_value = std::min(res.min_value,v[y].min_vertex[q]);
            y = v[y].parents[q].to;
        }       
    }
    int ii = 30;
    while(true) {
        if(ii < 0) break;
        if(v[x].parents.size() > ii && v[x].parents[ii].to != v[y].parents[ii].to) {
            res.distance += v[x].parents[ii].distance + v[y].parents[ii].distance;
            res.min_value = std::min(std::min(v[x].min_vertex[ii],v[y].min_vertex[ii]),res.min_value);
            x = v[x].parents[ii].to;
            y = v[y].parents[ii].to;
        }
        --ii;
    }
    if(x != y) {
        res.lca = v[x].parents[0].to;
        res.distance += v[x].parents[0].distance + v[y].parents[0].distance;
        res.min_value = std::min(std::min(v[x].min_vertex[0],v[y].min_vertex[0]),res.min_value);
        return res;
    }
    else if(x == y) {
        res.lca = x;
        return res;
    }
}

result_class LCA_brute_force(ll x,ll y) {
    result_class res;
    res.min_value = std::min(v[x].value,v[y].value);

    if(v[y].level < v[x].level) std::swap(x,y);
    while(v[y].level > v[x].level) {
        res.lca = v[y].parents[0].to;
        res.distance += v[y].parents[0].distance;
        res.min_value = std::min(res.min_value,v[y].min_vertex[0]);
        y = v[y].parents[0].to;
    }
    while(x != y) {
        res.lca = v[x].parents[0].to;
        res.distance += v[y].parents[0].distance + v[x].parents[0].distance;
        res.min_value = std::min(std::min(v[x].min_vertex[0],v[y].min_vertex[0]),res.min_value);
        x = v[x].parents[0].to;
        y = v[y].parents[0].to;
    }
    return res;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);    
}

bool run_test() {
    f.clear();
    v.clear();

    ll vertex_number = ((ull)get_rand_value() % (4000)) + 2000;
    for(int q = 0;q < vertex_number;q++) 
        f.push_back(std::vector<edge>());
    for(int q = 0;q < vertex_number;q++) {
        v.push_back(vertex());
        v.back().value = get_rand_value() % 500;
        if(q != 0) {
            ll parent = (ull)get_rand_value() % (v.size() - 1);
            ll distance = get_rand_value() % 1000;
            f[parent].push_back({q,distance});
            f[q].push_back({parent,distance});
        }
    }
    build(0);

    for(int q = 0;q < 10000;q++) {
        int x = std::rand() % v.size(), y = std::rand() % v.size();
        result_class get_ans = LCA_brute_force(x,y);
        result_class get_lca = LCA(x,y);
        if(get_ans.lca != get_lca.lca || get_ans.distance != get_lca.distance || get_ans.min_value != get_lca.min_value) {
            return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(!run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
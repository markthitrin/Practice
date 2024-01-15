#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class vertex{
public:
    ll value;
    ll euler_number_in;
    ll euler_number_out;
    ll level;
    std::vector<ll> parents;
};

class edge{
public:
    ll to;
    edge() {}
    edge(ll _to) : to(_to) {}
};

class query{
public:
    ll query_number;
    ll left;
    ll Lca;
    ll right;
};

std::vector<vertex> v;
std::vector<std::vector<edge>> f;
std::vector<query> qu;

void recur1(ll u,ll p) {
    v[u].level = v[p].level + 1;
    v[u].parents.push_back(p);
    ll pp = p;
    while(v[pp].parents.size() > v[u].parents.size() - 1) {
        v[u].parents.push_back(v[pp].parents[v[u].parents.size() - 1]);
        pp = v[u].parents.back();
    }

    for(int q = 0;q < f[u].size();q++){
        if(f[u][q].to != p) {
            recur1(f[u][q].to, u);
        }
    }
}

void built_LCA(ll root) {
    v[0].level = 0;
    for(int q = 0;q < f[root].size();q++){
        recur1(f[root][q].to, root);
    }
}

void recur2(ll u,ll p,ll& number) {
    v[u].euler_number_in = number++;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur2(f[u][q].to,u,number);
        }
    }
    v[u].euler_number_out = number++;
}

void euler_tour(ll root) {
    ll number = 0;
    recur2(0,-1,number);
}

ll LCA(ll x,ll y) {
    if(v[x].level > v[y].level) std::swap(x,y);
    ll diff = v[y].level - v[x].level;
    for(int q = 60;q >=0;q--) {
        if((1ll << q) & diff) {
            y = v[y].parents[q];
        }
    }
    for(int q = 60;q>=0;q--){
        if(v[x].parents.size() > q) {
            if(v[x].parents[q] != v[y].parents[q]) {
                x =v[x].parents[q];
                y = v[y].parents[q];
            }
        }
    }
    if(x == y) return x;
    return v[x].parents[0];
}

void recur3(ll u,ll p,std::vector<ll>& euler_order) {
    euler_order[v[u].euler_number_in] = u;
    euler_order[v[u].euler_number_out] = u;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur3(f[u][q].to,u,euler_order);
        }
    }
}

std::vector<ll> Mo_algorithm(std::vector<query> qu) {
    for(int q = 0;q < qu.size();q++) {
        qu[q].left = v[qu[q].left].euler_number_out - 1;
        qu[q].right = v[qu[q].right].euler_number_in;
        if(qu[q].left > qu[q].right) std::swap(qu[q].left,qu[q].right);
    }
    ll sqrt_N = std::sqrt(v.size());
    std::sort(qu.begin(),qu.end(),[&](query a,query b) {
        if(a.left / sqrt_N != b.left / sqrt_N) return a.left < b.left;
        return a.right < b.right;
    });
    std::vector<ll> euler_order(v.size() * 2);
    recur3(0,-1,euler_order);
    ll cur_left = -1;
    ll cur_right = -1;
    std::map<ll,ll> number_count;
    ll distict_count = 0;
    auto decrease_numbber = [&](ll a) {
        number_count[a]--;
        if(number_count[a] == 0) distict_count--;
    };
    auto increase_number = [&](ll a) {
        number_count[a]++;
        if(number_count[a] == 1) distict_count++;
    };
    std::map<ll,bool> vertex_active;
    std::vector<ll> ans(qu.size());
    for(int q = 0;q < qu.size();q++) {
        while(cur_left > qu[q].left) {
            vertex_active[euler_order[cur_left]] = !vertex_active[euler_order[cur_left]];
            if(vertex_active[euler_order[cur_left]]) increase_number(v[euler_order[cur_left]].value);
            else decrease_numbber(v[euler_order[cur_left]].value);
            --cur_left;
        }
        while(cur_left < qu[q].left) {
            ++cur_left;
            vertex_active[euler_order[cur_left]] = !vertex_active[euler_order[cur_left]];
            if(vertex_active[euler_order[cur_left]]) increase_number(v[euler_order[cur_left]].value);
            else decrease_numbber(v[euler_order[cur_left]].value);
        }
        while(cur_right < qu[q].right) {
            ++cur_right;
            vertex_active[euler_order[cur_right]] = !vertex_active[euler_order[cur_right]];
            if(vertex_active[euler_order[cur_right]]) increase_number(v[euler_order[cur_right]].value);
            else decrease_numbber(v[euler_order[cur_right]].value);
        }
        while(cur_right > qu[q].right) {
            vertex_active[euler_order[cur_right]] = !vertex_active[euler_order[cur_right]];
            if(vertex_active[euler_order[cur_right]]) increase_number(v[euler_order[cur_right]].value);
            else decrease_numbber(v[euler_order[cur_right]].value);
            --cur_right;
        }
        if(qu[q].left == qu[q].right) {
            ans[qu[q].query_number] = 1;
        }
        else if(euler_order[qu[q].left + 1] == qu[q].Lca || euler_order[qu[q].right] == qu[q].Lca) {
            ans[qu[q].query_number] = distict_count;
        }
        else {
            if(number_count[v[qu[q].Lca].value] == 0) {
                ans[qu[q].query_number] = distict_count + 1;
            }
            else {
                ans[qu[q].query_number] = distict_count;
            }
        }
    }
    return ans;
}

bool recur4(ll u,ll p,ll des,std::map<ll,ll>& mm) {
    mm[v[u].value]++;
    if(u == des) return true;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            bool result = recur4(f[u][q].to,u,des,mm);
            if(result) return true;
        }
    }
    mm[v[u].value]--;
    if(mm[v[u].value] == 0) mm.erase(v[u].value);
    return false;
}

ll get_distinct_value_on_path_brute_force(ll x,ll y) {
    std::map<ll,ll> get_res;
    recur4(x,-1,y,get_res);
    return get_res.size();
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    v.clear();
    f.clear();
    qu.clear();

    ll vertex_number = (ull)get_rand_value() % 500 + 500;
    ll query_number = (ull)get_rand_value() % 500 + 500;
    for(int q = 0;q < vertex_number;q++) {
        v.push_back(vertex());
        v[q].value = get_rand_value() % 200;
        f.push_back(std::vector<edge>());
    }
    for(int q = 1;q < vertex_number;q++) {
        ll parent = (ull)get_rand_value() % q;
        f[parent].push_back(edge(q));
        f[q].push_back(edge(parent));
    }
    built_LCA(0);
    euler_tour(0);
    for(int q = 0;q < query_number;q++) {
        qu.push_back(query());
        qu.back().left = (ull)get_rand_value() % vertex_number;
        qu.back().right = (ull)get_rand_value() % vertex_number;
        qu.back().Lca = LCA(qu.back().left,qu.back().right);
        qu.back().query_number = q;
    }
    std::vector<ll> ans(qu.size());
    for(int q = 0;q < query_number;q++) {
        ans[q] = get_distinct_value_on_path_brute_force(qu[q].left,qu[q].right);
    }
    std::vector<ll> get_try = Mo_algorithm(qu);
    return ans == get_try;
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
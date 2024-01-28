#include <iostream>
#include <vector>
#include <queue>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node{
public:
    node* left = nullptr;
    node* right = nullptr;
    ll value = 0;
    ll s = 0;
    node() {}
    node(ll _value) : value(_value) {} 
};

node* merge(node* p,node* q) {
    if(!p || !q) return !p?q:p;
    if(p->value < q->value) std::swap(p,q);
    node* r = new node(*p); r->right = merge(p->right,q);
    if(r->right && (!r->left || r->right->s > r->left->s)) {
        std::swap(r->right,r->left);
    } 
    r->s = r->left?r->left->s + 1:1;
    return r;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    std::vector<node*> r;
    r.push_back(nullptr);
    std::vector<std::priority_queue<ll>> g;
    g.push_back(std::priority_queue<ll>());
    auto do_push = [&](int version, ll value) {
        g.push_back(g[version]);
        g.back().push(value);
        r.push_back(merge(r[version],new node(value)));
    };
    auto do_pop = [&](int version) {
        if(g[version].empty()) return;
        g.push_back(g[version]);
        g.back().pop();
        r.push_back(merge(r[version]->left,r[version]->right));
    };
    auto do_get_max = [&](int version) {
        if(g[version].empty()) return true;
        return g[version].top() == r[version]->value;
    };
    for(int q = 0;q < 100000;q++) {
        int mode = std::rand() % 3;
        if(mode == 0) {
            ll version = (ull)get_rand_value() % g.size();
            ll value = get_rand_value() % 1000;
            do_push(version,value);
        }
        else if(mode == 1) {
            ll version = (ull)get_rand_value() % g.size();
            do_pop(version);
        }
        else {
            ll version = (ull)get_rand_value() % g.size();
            if(!do_get_max(version)) return false;
        }
    }
    return true;
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
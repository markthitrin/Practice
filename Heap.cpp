#include <iostream>
#include <vector>
#include <queue>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node{
public:
    ll value;
    ll s = 0;
    node* left = nullptr;
    node* right = nullptr;
    node() {}
    node(ll _value) : value(_value) {}
};

node* merge(node* p,node* q) {
    if(p == nullptr) return q;
    if(q == nullptr) return p;
    if(p->value < q->value) std::swap(p,q);
    p->right = merge(p->right,q);
    if(p->right) {
        if(p->left == nullptr || p->right->s > p->left->s)
            std::swap(p->left,p->right);
    }
    p->s = p->left!=nullptr?p->left->s + 1:1;
    return p;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    node* root = nullptr;
    std::priority_queue<ll> g;
    auto do_push = [&](ll value) {
        g.push(value);
        root = merge(root, new node(value));
    };
    auto do_pop = [&]() {
        if(g.empty()) return;
        g.pop();
        root = merge(root->left,root->right);  
    };
    auto do_get_max = [&]() {
        if(g.empty()) return true;
        return g.top() == root->value;
    };
    for(int q = 0;q < 1000000;q++) {
        int mode = std::rand() % 3;
        if(mode == 0) {
            ll new_value = get_rand_value() % 10000;
            do_push(new_value);
        }
        else if(mode == 1) {
            do_pop();
        }
        else {
            if(!do_get_max()) return false;
        }
    }
    return true;
}

int main() {
    int _ = 0 ;
    std::cin >> _;
    for(int q = 1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else { 
            std::cout << "failed\n";
        }
    }
}
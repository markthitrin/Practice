#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node{
public:
    ll key; // heap key
    ll value; // binary tree key
    ll s;
    node* left = nullptr;
    node* right = nullptr;
};

node* merge(node* p, node* q) {
    if(!p || !q) return !p?q:p;
    if(p->key < q->key) std::swap(p,q);
    if(q->value > p->value) p->right = merge(p->right,q);
    else p->left = merge(p->left,q);
    p->s = p->left?p->left->s + 1:1;
    p->s = std::max(p->s,p->right?p->right->s + 1:1);
    return p;
}

ll get_key(node* root,ll value) {
    if(!root) return 0;
    if(root->value == value) return root->key;
    if(value < root->value) return get_key(root->left,value);
    else return get_key(root->right,value);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int vector_size = (ull)get_rand_value() % 100000 + 100000;
    node* root = nullptr;
    std::vector<node> a(vector_size);
    for(int q = 0;q < a.size();q++) {
        a[q].key = get_rand_value();
        a[q].value = q;
        root = merge(root,&a[q]);
    }
    auto do_check = [&](ll value) {
        ll ans = a[value].key;
        ll get_ans = get_key(root,value);
        return ans == get_ans;
    };
    for(int q = 0;q < 1000000;q++) {
        ll value = (ull)get_rand_value() % vector_size;
        if(!do_check(value)) return false;
    }
    std::cout << "size = " << root->s << " ; "; 
    return true;
}

int main() {
    int  _ = 0;
    std::cin >> _;
    for(int q = 1;q<=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test() ){
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
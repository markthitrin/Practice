#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

ll get_rand_value();

class node{
public:
    node* left = nullptr;
    node* right = nullptr;
    ll min;
    ll max;
    ll value; // binary value
    ll key; // heap key
    node() : key(get_rand_value()) {}
    node(ll _value) : value(_value), min(_value), max(_value), key(get_rand_value()) {}
};

void fix(node* root) {
    if(!root) return;
    if(!root->left && !root->right) return ;
    root->min = root->max = root->value;
    root->min = std::min(root->min,root->left?root->left->min:root->right->min);
    root->max = std::max(root->max,root->right?root->right->max:root->left->max);
    return;
}

void split(node* root,node* &left,node* &right,ll x) {
    if(root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    if(root->value <= x) {
        left = root;
        split(root->right,left->right,right,x);
    }
    else {
        right = root;
        split(root->left,left,right->left,x);
    }
    fix(root);
}

node* merge(node* left,node* right) {
    if(!left || !right) return left?left:right;
    if(left->value > right->value) return merge(right,left);
    if(left->key > right->key) {
        left->right = merge(left->right,right);
        fix(left);
        return left;
    }
    else {
        right->left = merge(left,right->left);
        fix(right);
        return right;
    }
}

node* insert(node* root,node* new_node) {
    node *left = nullptr,*right = nullptr;
    split(root,left,right,new_node->value);
    root = merge(left,new_node);
    root = merge(root,right);
    return root;
}

node* find(node* root,ll value) {
    if(!root) return nullptr;
    if(root->value == value) return root;
    if(value < root->value) return find(root->left,value);
    return find(root->right,value);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int vector_size = (ull)get_rand_value() % 100000 + 100000;
    std::vector<node> v;
    std::set<ull> ss;
    for(int q = 0;q < vector_size;q++) {
        ss.insert((ull)get_rand_value());
    }
    for(auto it = ss.begin();it != ss.end();it++) {
        v.push_back(node(*it));
    }
    node* root = nullptr;
    for(int q = 0;q < v.size();q++) {
        root = insert(root,&v[q]);
    }

    auto do_get_key = [&](ll ind) {
        ll ans = v[ind].key;
        ll get_ans = find(root,v[ind].value)->key;
        return ans == get_ans;
    };

    for(int q = 0;q < 100000;q++) {
        ll ind = (ull)get_rand_value() % v.size();
        if(!do_get_key(ind)) return false;
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++){
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
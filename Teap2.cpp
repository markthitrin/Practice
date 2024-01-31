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
    ll key;
    ll size;
    node() : key(get_rand_value()), size(1) {}
};

ll get_size(node* root) {
    if(root) return root->size;
    return 0;
}

void fix(node* root) {
    if(!root) return;
    root->size = 1 + get_size(root->left) + get_size(root->right);
    return; 
}

void split_sub_tree(node* root,node*& left,node*& right,ll s) {
    if(!root)
        left = right = nullptr;
    else if(get_size(root->left) + 1 <= s) {
        left = root;
        split_sub_tree(root->right,left->right,right,s - get_size(root->left) - 1);
    }
    else {
        right = root;
        split_sub_tree(root->left,left,right->left,s);
    }
    fix(root);
}

node* merge(node* left,node* right) {
    if(!left || !right) return left?left:right;
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

node* find(node* root,ll s) {
    if(!root) return nullptr;
    if(get_size(root->left) < s) {
        return find(root->right,s- get_size(root->left) - 1);
    }
    else if(get_size(root->left) == s) {
        return root;
    }
    else {
        return find(root->left,s);
    }
}

node* insert(node* root,node* new_node,ll pos) {
    node *left = nullptr,*right = nullptr;
    split_sub_tree(root,left,right,pos);
    return merge(merge(left,new_node),right);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int vector_size = (ull)get_rand_value() % 5000 + 5000;
    std::set<ll> ss;
    std::vector<ll> v(vector_size);
    for(int q = 0;q < v.size();q++) {
        v[q] = (ull)get_rand_value();
    }
    node* root = nullptr;
    for(int q = 0;q < v.size();q++) {
        node* new_node = new node();
        new_node->key = v[q];
        root = insert(root,new_node,q);
    }
    auto do_cut = [&](int a,int b) {
        std::vector<ll> v1(v.begin(), v.begin() + a);
        std::vector<ll> v2(v.begin() + a,v.begin() + b);
        std::vector<ll> v3(v.begin() + b,v.end());
        v.clear();
        for(int q = 0;q < v1.size();q++) v.push_back(v1[q]);
        for(int q = 0;q < v3.size();q++) v.push_back(v3[q]);
        for(int q = 0;q < v2.size();q++) v.push_back(v2[q]);
        
        node *root1 = nullptr,*root2 = nullptr,*root3 = nullptr;
        split_sub_tree(root,root1,root2,a);
        split_sub_tree(root2,root2,root3,b - a);
        root = merge(merge(root1,root3),root2);
    };
    auto do_get_key = [&](ll pos) {
        ll ans = v[pos];
        ll get_ans = find(root,pos)->key;
        return ans == get_ans;
    };
    for(int q= 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll a = (ull)get_rand_value() % (vector_size - 2) + 1;
            ll b = (ull)get_rand_value() % (vector_size - 2) + 1;
            if(a > b)std::swap(a,b);
            do_cut(a,b);
        }
        else {
            ll pos = (ull)get_rand_value() % (vector_size);
            if(!do_get_key(pos)) return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q<=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
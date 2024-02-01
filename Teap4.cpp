#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
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
    ll key = 0;
    ll value = 0;
    node() : key(get_rand_value()) {}
    node(const ll& _value) : value(_value), key(get_rand_value()) {}
};

bool key_less(node* a,node* b) {
    if(a->key == b->key) {
        return a->value < b->value;
    }
    return a->key < b->key;
}

void split(node* root,node*& left,node*& right,ll x) {
    if(!root) {
        left = right = nullptr;
        return;
    }
    else if(x <= root->value) {
        right = root;
        split(root->left,left,right->left,x);
    }
    else {
        left = root;
        split(root->right,left->right,right,x);
    }
}

node* merge(node* left,node* right) {
    if(!left || !right) return left?left:right;
    if(left->key > right->key){
        left->right = merge(left->right,right);
        return left;
    }
    else {
        right->left = merge(left,right->left);
        return right;
    }
}

node* merge_tree(node* left,node* right) {
    if(!left || !right) return left?left:right;
    if(key_less(left,right)) std::swap(left,right);
    node* right_less = nullptr,*right_more = nullptr;
    split(right,right_less,right_more,left->value);
    left->left = merge_tree(left->left,right_less);
    left->right = merge_tree(left->right,right_more);
    return left;
}

node* insert(node* root,node* new_node) {
    node* left = nullptr,*right = nullptr;
    split(root,left,right,new_node->value);
    return merge(merge(left,new_node),right);
}

node* find(node* root,ll value) {
    if(!root) return root;
    if(value < root->value) return find(root->left,value);
    else if(value == root->value) return root;
    else return find(root->right,value);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int map_number = (ull)get_rand_value() % 20 + 20;
    std::vector<std::map<ll,ll>> v(map_number);
    std::set<ll> value_set;
    for(int q = 0;q < 100000;q++) {
        value_set.insert((ull)get_rand_value());
    }
    auto value_set_it = value_set.begin();
    for(int q = 0; q < v.size();q++) {
        int vector_size = (ull)get_rand_value() % 1000 + 1000;
        for(int w = 0;w < vector_size;w++) {
            v[q][(*value_set_it++)] = get_rand_value();
        }
    }
    std::vector<node*> roots(v.size(),nullptr);
    for(int q = 0 ;q < v.size();q++) {
        for(auto it = v[q].begin();it != v[q].end();it++) {
            node* new_node = new node(it->first);
            new_node->key = it->second;
            roots[q] = insert(roots[q],new_node);
        }
    }
    auto do_merge_check = [&](std::vector<bool>& select_merge) {
        node* root = nullptr;
        for(int q = 0;q < select_merge.size();q++){
            if(select_merge[q]) {
                root = merge_tree(root,roots[q]);
            }
        }
        std::map<ll,ll> merge_map;
        for(int q = 0;q < select_merge.size();q++) {
            if(select_merge[q]) {
                merge_map.merge(v[q]);
            }
        }
        int ccc = 0;
        for(auto it = merge_map.begin();it != merge_map.end();it++) {
            ++ccc;
            ll ans = it->second;
            ll get_ans = find(root,it->first)->key;
            if(ans != get_ans) 
                return false;
        }
        return true;
    };
    std::vector<bool> select_merge(v.size(),false);
    for(int q = 0;q < select_merge.size();q++) {
        select_merge[q] = true;
    }
    if(!do_merge_check(select_merge))
        return false;    
    return true;
}

int main() {
    int _ = 0;
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
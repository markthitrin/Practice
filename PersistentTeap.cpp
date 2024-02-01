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
    ll s = 1;
    ll key = 0;
    bool do_reverse = false;
    node() : key(get_rand_value()) {}
};

ll get_size(node* root) {
    if(!root) return 0;
    return root->s;
}

node* mirror(node* root) {
    if(!root) return nullptr;
    return new node(*root);
}

void fix(node* root) {
    if(!root) return;
    root->s = 1 + get_size(root->left) + get_size(root->right);
}

node* set_reverse(node* root) {
    if(!root) return nullptr;
    node* new_node = mirror(root);
    new_node->do_reverse = !new_node->do_reverse;
    std::swap(new_node->left,new_node->right);
    return new_node;
}

node* push_down(node* root) {
    if(!root) return root;
    if(!root->do_reverse) return root;
    node* new_node = mirror(root);
    new_node->left = set_reverse(new_node->left);
    new_node->right = set_reverse(new_node->right);
    new_node->do_reverse = false;
    return new_node;
}

void split(node* root,node*& left,node*& right,ll s) {
    if(!root) {
        left = right = nullptr;
        return;
    }
    root = push_down(root);
    if(get_size(root->left) + 1 <= s) {
        left = mirror(root);
        split(root->right,left->right,right,s - get_size(root->left) - 1);
        fix(left);
    }
    else {
        right = mirror(root);
        split(root->left,left,right->left,s);
        fix(right);
    }
}

node* merge(node* left,node* right) {
    if(!left || !right) return mirror(left?left:right);
    node* new_node;
    left = push_down(left);
    right = push_down(right);
    if(left->key > right->key) {
        new_node = mirror(left);
        new_node->right = merge(left->right,right);
        fix(new_node);
    }
    else {
        new_node = mirror(right);
        new_node->left = merge(left,right->left);
        fix(new_node);
    }
    return new_node;
}

node* insert(node* root,node* new_node,ll pos) {
    node* left = nullptr,*right = nullptr;
    split(root,left,right,pos);
    return merge(merge(left,new_node),right);
}

node* erase(node* root,ll pos) {
    node* left = nullptr,*middle = nullptr,*right= nullptr;
    split(root,left,middle,pos);
    split(middle,middle,right,1);
    return merge(left,right);
}

node* reverse(node* root,ll left,ll right) {
    if(!root) return nullptr;
    node *left_tree = nullptr,*middle_tree = nullptr,*right_tree = nullptr;
    split(root,left_tree,middle_tree,left);
    split(middle_tree,middle_tree,right_tree,right - left + 1);
    middle_tree = set_reverse(middle_tree);
    return merge(merge(left_tree,middle_tree),right_tree);
}

node* find(node* root,ll pos) {
    if(!root) return nullptr;
    root = push_down(root);
    if(get_size(root->left) < pos) {
        return find(root->right,pos - get_size(root->left) - 1);
    }
    else if(pos == get_size(root->left)) {
        return root;
    }
    else {
        return find(root->left,pos);
    }
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll init_vector_size = (ull)get_rand_value() % 1000 + 1000;
    std::vector<ll> v(init_vector_size);
    for(int q = 0;q < v.size();q++) {
        v[q] = get_rand_value();
    }
    std::vector<node*> roots(1,nullptr);
    for(int q = 0;q < v.size();q++) {
        node* new_node = new node();
        new_node->key = v[q];
        roots.back() = insert(roots.back(),new_node,q);
    }
    std::vector<std::vector<ll>> vers_ans;
    vers_ans.push_back(v);
    auto do_add = [&](ll version,ll pos) {
        ll new_key = get_rand_value();

        vers_ans.push_back(vers_ans[version]);
        vers_ans.back().insert(vers_ans.back().begin() + pos,new_key);

        node* new_node = new node();
        new_node->key = new_key;
        roots.push_back(insert(roots[version],new_node,pos));
    };
    auto do_erase = [&](ll version,ll pos) {
        vers_ans.push_back(vers_ans[version]);
        vers_ans.back().erase(vers_ans.back().begin() + pos);

        roots.push_back(erase(roots[version],pos));
    };
    auto do_reverse = [&](ll version,ll left,ll right) {
        vers_ans.push_back(vers_ans[version]);
        int _left = left,_right = right;
        while(_left < _right) {
            std::swap(vers_ans.back()[_left++],vers_ans.back()[_right--]);
        }
        
        roots.push_back(reverse(roots[version],left,right));
    };
    auto do_find = [&](ll version,ll pos) {
        ll ans = vers_ans[version][pos];

        ll get_ans = find(roots[version], pos)->key;
        return ans == get_ans;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 4;
        if(mode == 0) {
            ll version = (ull)get_rand_value() % vers_ans.size();
            ll pos = (ull)get_rand_value() % vers_ans[version].size();
            do_add(version,pos);
        }
        else if(mode == 1) {
            ll version = (ull)get_rand_value() % vers_ans.size();
            ll pos = (ull)get_rand_value() % vers_ans[version].size();
            do_erase(version,pos);
        }
        else if(mode == 2) {
            ll version = (ull)get_rand_value() % vers_ans.size();
            ll left = (ull)get_rand_value() % vers_ans[version].size();
            ll right = (ull)get_rand_value() % vers_ans[version].size();
            if(left > right) std::swap(left,right);
            do_reverse(version,left,right);
        }
        else {
            ll version = (ull)get_rand_value() % vers_ans.size();
            ll pos = (ull)get_rand_value() % vers_ans[version].size();
            if(!do_find(version,pos))
                return false;
        }
    }
    return true;
}

int main() {
    int  _ = 0;
    std::cin >> _;
    for(int q =1;q<=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
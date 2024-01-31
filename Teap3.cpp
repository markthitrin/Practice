#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

ll get_rand_value();

class node{
public:
    node* left = nullptr;
    node* right = nullptr;
    bool do_reverse = false;
    ll key;
    ll s = 1;
    node() : key(get_rand_value()) {}
};

ll get_size(node* root) {
    return root?root->s:0;
}

void fix(node* root) {
    if(!root) return;
    root->s = 1 + get_size(root->left) + get_size(root->right);
}

void set_reverse(node* root) {
    if(!root) return;
    root->do_reverse = !root->do_reverse;
    std::swap(root->left,root->right);
}

void push_down(node* root) {
    if(!root) return;
    if(!root->do_reverse) return;
    set_reverse(root->left);
    set_reverse(root->right);
    root->do_reverse = false;
    return;
}

void split(node* root,node*& left,node*& right,ll s) {
    if(root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    push_down(root);
    if(get_size(root->left) + 1 <= s) {
        left = root;
        split(root->right,left->right,right,s - get_size(root->left) - 1);
    }
    else {
        right = root;
        split(root->left,left,right->left,s);
    }
    fix(root);
}

node* merge(node* left,node* right) {
    if(!left || !right) return left?left:right;
    push_down(left);
    push_down(right);
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

node* insert(node* root,node* new_node,ll pos) {
    node* left_tree = nullptr,*right_tree = nullptr;
    split(root,left_tree,right_tree,pos);
    return merge(merge(left_tree,new_node),right_tree);
}

node* erase(node* root,ll pos) {
    node* left_tree = nullptr,*middle_tree = nullptr,*right_tree = nullptr;
    split(root,left_tree,middle_tree,pos);
    split(middle_tree,middle_tree,right_tree,1);
    return merge(left_tree,right_tree);
}

node* find(node* root,ll pos) {
    if(!root) return nullptr;
    push_down(root);
    if(get_size(root->left) < pos) {
        return find(root->right,pos - get_size(root->left) - 1);
    }
    else if(get_size(root->left) == pos) {
        return root;
    }
    else {
        return find(root->left,pos);
    }
}

node* reverse(node* root,int left,int right) {
    node* left_tree = nullptr,*middle_tree = nullptr,* right_tree = nullptr;
    split(root,left_tree,middle_tree,left);
    split(middle_tree,middle_tree,right_tree,right + 1 - left);
    set_reverse(middle_tree);
    return merge(merge(left_tree,middle_tree),right_tree);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int init_vector_size = (ull)get_rand_value() % 5000 + 5000;
    std::vector<ll> v(init_vector_size);
    for(int q = 0;q < v.size();q++) {
        v[q] = get_rand_value();
    }
    node* root = nullptr;
    for(int q = 0;q < v.size();q++) {
        node* new_node = new node();
        new_node->key = v[q];
        root = insert(root,new_node,q);
    }
    auto do_add = [&](ll pos) {
        ll new_node_key = get_rand_value();
        v.insert(v.begin() + pos,new_node_key);

        node* new_node = new node();
        new_node->key = new_node_key;
        root = insert(root,new_node,pos);
    };
    auto do_erase = [&](ll pos) {
        v.erase(v.begin() + pos);
        
        root = erase(root,pos);
    };
    auto do_reverse = [&](ll left,ll right) {
        ll _left = left;
        ll _right = right;
        while(_left < _right) {
            std::swap(v[_left++],v[_right--]);
        }

        reverse(root,left,right);
    };
    auto do_get_key = [&](ll pos) {
        ll ans = v[pos];
        ll get_ans = find(root,pos)->key;
        return ans == get_ans;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 4;
        if(mode == 0) {
            ll pos = (ull)get_rand_value() % v.size();
            do_add(pos);
        }
        else if(mode == 1) {
            ll pos = (ull)get_rand_value() % v.size();
            do_erase(pos);
        }
        else if(mode == 2) {
            ll left = (ull)get_rand_value() % v.size();
            ll right = (ull)get_rand_value() % v.size();
            if(left > right) std::swap(left,right);
            do_reverse(left,right);
        }
        else {
            ll pos = (ull)get_rand_value() % v.size();
            if(!do_get_key(pos)) return false;;
        }
    }
    return true;
}

int main() {
    int _  = 0;
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

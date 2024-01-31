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
    ll key;
    ll value;
    bool do_reverse;
    ll shift = 0;
    ll min;
    ll max;
    node* left = nullptr;
    node* right = nullptr;
    node() : key(get_rand_value()) {}
    node(ll _value) : value(_value) , min(_value), max(_value) , key(get_rand_value()) {}
};

void push_shift(node* root,ll shift) {
    if(!root) return;
    root->shift += shift;
    root->min += shift;
    root->max += shift;
    root->value += shift;
}

void push_reverse(node* root,ll left,ll right) {
    if(!root) return;
    root->shift = root->shift + left + right - root->min - root->min;
    root->min = left + right - root->min;
    root->max = left + right - root->max;
    root->value = left + right - root->value;
    root->do_reverse = !root->do_reverse;
}

void push_down(node* root) {
    push_shift(root->left,root->shift);
    push_shift(root->right,root->shift);
    root->shift = 0;
    
    if(!root->do_reverse) return;
    push_reverse(root->left,root->min,root->max);
    push_reverse(root->right,root->min,root->max);
    root->do_reverse = false;
}

void fix(node* root) {
    if(!root) return;
    if(!root->left && !root->right) return;
    root->min = root->left?root->left->min:root->right->min;
    root->max = root->right?root->right->max:root->left->max;
}

node* merge(node* p, node* q) {
    if(!p || !q) return !p?q:p;
    push_down(p);
    push_down(q);
    if(p->key < q->key) std::swap(p,q);
    if(q->value < p->value) p->left = merge(p->left,q);
    else p->right = merge(p->right,q);
    fix(p);
    return p;
}

void tree_check(node* root) {
    if(!root) return;
    if(root->left && root->left->value > root->value) 
        std::cout << "hey\n";
    if(root->right && root->right->value < root->value)
        std::cout << "sdfff";
    if(root->left && root->key < root->left->key)
        std::cout << " sdfsdfff";
    if(root->right && root->key < root->right->key)
        std::cout << "sssssssss";
    tree_check(root->left);
    tree_check(root->right);
}

void reverse(node* root,ll left,ll right) {
    if(!root) return;
    if(right < root->min && left > root->max) return;
    if(left <= root->min && root->max <= right) {
        root->do_reverse = !root->do_reverse;
        root->shift = root->shift + right - root->min + left - root->min;
        root->min = right - root->min + left;
        root->max = right - root->max + left;
        root->value = right - root->value + left;
    }
    else {
        push_down(root);
        reverse(root->left,left,right);
        reverse(root->right,left,right);
        fix(root);
    }
}

node* find(node* root,ll value) {
    if(root == nullptr)
        std::cout << "sdf";
    if(root->value == value) return root;
    push_down(root);
    if(value < root->value) 
        return find(root->left,value);
    return find(root->right,value);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int vector_size = (ull)get_rand_value() % 3000 + 3000;
    std::vector<node> v(vector_size);
    for(int q = 0;q < v.size();q++) {
        v[q] = node((ull)get_rand_value() % 10000);
    }
    std::sort(v.begin(),v.end(),[](node a,node b) {
        return a.value < b.value;
    });
    for(int q = 0;q < v.size() - 1;q++) {
        if(v[q].value == v[q + 1].value) {
            v.erase(v.begin() + q); --q;
        }
    }
    node* root = nullptr;
    for(int q = 0;q < v.size();q++) {
        root = merge(root,new node(v[q]));
    }

    auto do_add = [&](ll value) {
        for(int q = 0;q < v.size();q++) {
            if(v[q].value == value) return;
            else if(value < v[q].value) {
                if(value == 9038) {
                    std::cout << "dsfsddf";
                }
                v.insert(v.begin() + q, node(value));
                root = merge(root,new node(v[q]));
                return;
            }
        }
        v.insert(v.end(), node(value));
        root = merge(root,new node(v.back()));
        tree_check(root);
    };
    auto do_get_key = [&](ll value) {
        ll ans = 0;
        for(int q = 0;q < v.size();q++) {
            if(v[q].value == value) ans = v[q].key;
        }
        ll get_ans = find(root,value)->key;
        return ans == get_ans;
    };
    auto do_reverse = [&](ll left,ll right) {
        int start_ind = -1;
        int end_ind = 0;
        for(int q = 0;q < v.size();q++) {
            if(left <= v[q].value && v[q].value <= right) {
                v[q].value = left + right - v[q].value;
                if(start_ind == -1) start_ind = q;
                end_ind = q;
            }
        }
        while(start_ind < end_ind) {
            std::swap(v[start_ind],v[end_ind]);
            ++start_ind;
            --end_ind;
        }
        reverse(root,left,right);
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 3;
        if(mode == 0) {
            ll value = (ull)get_rand_value() % 10000;
            do_add(value);
        }
        else if(false) {
            ll left = (ull)get_rand_value();
            ll right = (ull)get_rand_value();
            if(left > right) std::swap(left,right);
            do_reverse(left,right);
        }
        else {
            ll ind = (ull)get_rand_value() % v.size();
            if(!do_get_key(v[ind].value)) return false;
        }
    }
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

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <map>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const int MINRANGE = 0;
const int MAXRANGE = 1e6;

class node{
public:
    node* left = nullptr;
    node* right = nullptr;
    ll max1 = 0;
    ll max2 = 0;
    ll max1_c = 0;
    ll sum = 0;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void fix(node* root) {
    if(root->left == nullptr && root->right == nullptr) return;

    root->sum = 0;
    if(root->left) root->sum += root->left->sum;
    if(root->right) root->sum += root->right->sum;

    std::map<ll, int> h;
    if(root->left) {
        h[root->left->max1] += root->left->max1_c;
        h[root->left->max2] += 0;
    }
    if(root->right) {
        h[root->right->max1] += root->right->max1_c;
        h[root->right->max2] += 0;
    }
    auto it = h.end();

    --it;
    root->max1 = it->first;
    root->max1_c = it->second;

    --it;
    root->max2 = it->first;

    return;
}

void push_min(node* root,ll x) {
    if(root == nullptr) return;
    if(x > root->max1) return;
    root->sum -= (root->max1 - x) * root->max1_c;
    root->max1 = x;
    return;
}

void push_down(node* root) {
    if(root == nullptr) return;
    push_min(root->left,root->max1);
    push_min(root->right,root->max1);
}

node* build(const std::vector<ll>& a,int left = MINRANGE,int right = MAXRANGE) {
    if(left >= a.size() || right < 0) return nullptr;
    node* root = new node();
    if(left == right) {
        root->max1 = a[left];
        root->max2 = MININT;
        root->max1_c = 1;
        root->sum = a[left];
        return root;
    }
    ll mid = get_mid(left,right);
    root->left = build(a,left,mid);
    root->right = build(a,mid + 1,right);
    fix(root);
    return root;
}

void set_range_to_min(node* root,int s,int e,ll x,int left = MINRANGE,int right = MAXRANGE) {
    if(right < s || left > e) return;
    if(root == nullptr) return;
    if(x > root->max1) return;
    ll mid = get_mid(left,right);
    if(s <= left && right <= e && x > root->max2) {
        push_min(root, x);
    }
    else {
        push_down(root);
        set_range_to_min(root->left,s,e,x,left,mid);
        set_range_to_min(root->right,s,e,x,mid + 1,right);
        fix(root);
    }
}

ll get_sum(node* root,int s,int e,int left = MINRANGE,int right = MAXRANGE) {
    if(right < s || left > e) return 0;
    if(root == nullptr) return 0;
    if(s <= left && right <= e) {
        return root->sum;
    }
    ll mid = get_mid(left,right);
    push_down(root);
    ll get_sum_left = get_sum(root->left,s,e,left,mid);
    ll get_sum_right = get_sum(root->right,s,e,mid + 1,right);
    return get_sum_left + get_sum_right;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int vector_size = (ull)get_rand_value() % 3000 + 3000;
    std::vector<ll> a(vector_size);
    for(int q = 0;q < a.size();q++) {
        a[q] = get_rand_value() % 1000000;
    }
    node* root = build(a);
    
    auto do_set_range_min = [&](int left,int right,ll x) {
        for(int q = left;q <= right;q++) {
            a[q] = std::min(x,a[q]);
        }
        set_range_to_min(root,left,right,x);
    };
    auto do_get_sum = [&](int left,int right) {
        ll ans = 0;
        for(int q = left;q<=right;q++) {
            ans += a[q];
        }
        ll get_ans = get_sum(root,left,right);
        return ans == get_ans;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            int left = (ull)get_rand_value() % vector_size;
            int right =  (ull)get_rand_value() % vector_size;
            if(left > right) std::swap(left,right);
            ll x = get_rand_value() % 1000000;
            do_set_range_min(left,right,x);
        }
        else {
            int left = (ull)get_rand_value() % vector_size;
            int right = (ull)get_rand_value() % vector_size;
            if(left > right) std::swap(left,right);
            if(!do_get_sum(left,right)) return false;
        }
    }
    return true;
}

int main() {
    int _ =  0;
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
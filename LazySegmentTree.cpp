#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;
const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node {
public:
    ll value = 0;
    ll total_sum;
    node* left = nullptr;
    node* right = nullptr;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void fix(node* root,ll left,ll right) {
    if(root->left == nullptr) root->left = new node();
    if(root->right == nullptr) root->right = new node();
    ll mid = get_mid(left,right);
    root->left->total_sum += (root->total_sum);
    root->left->value += (mid - left + 1) * (root->total_sum);
    root->right->total_sum += (root->total_sum);
    root->right->value += (right - mid) * (root->total_sum);
    root->total_sum = 0;
}

void add(node* root,ll s,ll e,ll x,ll left = MININT,ll right = MAXINT) {
    if(s <= left && right <= e) {
        root->value += x * (right - left + 1);
        root->total_sum += x;
        return;
    }
    if(s > right || e < left) return;
    ll mid = get_mid(left,right);
    fix(root,left,right);
    add(root->left,s,e,x,left,mid);
    add(root->right,s,e,x,mid + 1,right);
    root->value = root->left->value + root->right->value;
    return;
}

ll get_sum(node* root,ll s, ll e,ll left = MININT,ll right = MAXINT) {
    if(s <= left && right <= e) return root->value;
    if(s > right || e < left) return 0;
    ll mid = get_mid(left,right);
    fix(root,left,right);
    return get_sum(root->left,s,e,left,mid) + get_sum(root->right,s,e,mid + 1,right);
}

node* root = new node();

ll get_rand_number() {
    return (ull)std::rand() << 49 | (ull)std::rand() << 34 |
    (ull)std::rand() << 19 | (ull)std::rand() << 4 | (ull)std::rand() & (15);
}

std::pair<ll,ll> get_rand_range(ll min, ll max) {
    std::pair<ll,ll> res((ull)get_rand_number() % (max - min + 1) + min,(ull)get_rand_number() % (max - min + 1) + min);
    if(res.first > res.second) std::swap(res.first,res.second);
    return res;
}

bool run_test() {
    root = new node();
    std::pair<ll,ll> range = get_rand_range(-5000,5000);
    std::vector<ll> ans(range.second - range.first + 1, 0);
    auto do_add = [&](ll left,ll right,ll x) {
        add(root,left,right,x);
        for(int i = left - range.first;i <= right - range.first;i++) {
            ans[i] += x;
        }
    };
    auto do_get_sum = [&](ll left,ll right) {
        ll sum = get_sum(root,left,right);
        ll ans_sum = 0;
        for(int i = left - range.first;i <= right - range.first;i++) {
            ans_sum += ans[i];
        }
        return sum == ans_sum;
    };
    for(int i = 0;i < 10000;i++) {
        ll mode = std::rand() % 2;
        if(mode) {
            std::pair<ll,ll> r = get_rand_range(range.first,range.second);
            ll value = get_rand_number() % 1000000;
            do_add(r.first,r.second,value);
        }
        else {
            std::pair<ll,ll> r = get_rand_range(range.first,range.second);
            if(!do_get_sum(r.first,r.second)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int _ =  0;
    std::cin >> _;
    for(int q = 1;q <= _;q++) {
        std::cout << "testcase number " << q << " : "; 
        if(run_test()) {
            std::cout << "Passed\n";
        }
        else {
            std::cout << "Fail\n";
        }
    }
    return 0;
}
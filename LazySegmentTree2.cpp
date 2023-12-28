// this version will give you the number of min value
#include <iostream>
#include <vector>
using ll = long long;
using ull = unsigned ll;

const ll MAXINT = 1e18;
const ll MININT = -1e18;

class node{
public:
    ll value = 0;
    ll count = 0;
    ll total_add = 0;
    node* left = nullptr;
    node* right = nullptr;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

node* get_new_node(ll left, ll right) {
    node* res = new node();
    res->count = right - left + 1;
    return res;
}

void fix(node* root,ll left,ll right) {
    if(right == left) return;
    ll mid = get_mid(left,right);
    if(root->left == nullptr) root->left = get_new_node(left,mid);
    if(root->right == nullptr) root->right = get_new_node(mid + 1,right);
    root->left->value += root->total_add;
    root->left->total_add += root->total_add;
    root->right->value += root->total_add;
    root->right->total_add += root->total_add;
    root->total_add = 0;
}

void add(node* root,ll s,ll e,ll x,ll left = MININT,ll right = MAXINT) {
    if(s <= left && right <= e) {
        root->value += x;
        root->total_add += x;
        return;
    }
    if(s > right || e < left) {
        return;
    }
    fix(root,left,right);
    ll mid = get_mid(left,right);
    add(root->left,s,e,x,left,mid);
    add(root->right,s,e,x,mid + 1,right);
    if(root->left->value == root->right->value) {
        root->value = root->left->value;
        root->count = root->left->count + root->right->count;
    }
    else if(root->left->value > root->right->value) {
        root->value = root->right->value;
        root->count = root->right->count;
    }
    else {
        root->value = root->left->value;
        root->count = root->left->count;
    }
    return;
}

std::pair<ll,ll> get_min(node* root,ll s,ll e,ll left = MININT, ll right = MAXINT) {
    if(root == nullptr) return {MAXINT,0};
    if(s <= left && right <= e) return {root->value,root->count};
    if(s > right || e < left) return {MAXINT,0};
    fix(root,left,right);
    ll mid = get_mid(left,right);
    std::pair<ll,ll> get_left = get_min(root->left,s,e,left,mid);
    std::pair<ll,ll> get_right = get_min(root->right,s,e,mid + 1,right);
    if(get_left.first == get_right.first) return {get_left.first,get_left.second + get_right.second};
    return std::min(get_left,get_right);
}

ll get_rand_number() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

std::pair<ll,ll> get_rand_range(ll min,ll max) {
    std::pair<ll,ll> res((ull)get_rand_number() % (max - min + 1) + min,(ull)get_rand_number() % (max - min + 1) + min);
    if(res.first > res.second) std::swap(res.first,res.second);
    return res;
}

bool run_test() {
    std::pair<ll,ll> range = get_rand_range(-10000,10000);
    node* root = get_new_node(range.first,range.second);
    std::vector<ll> arr(range.second - range.first + 1, 0);
    auto do_add = [&](ll s,ll e,ll x) {
        for(ll q = s - range.first;q <= e - range.first;q++) {
            arr[q] += x;
        }
        add(root,s,e,x);
    };
    auto do_get_min = [&](ll s,ll e) {
        std::pair<ll,ll> try_min;
        std::pair<ll,ll> ans_min(MAXINT,0);
        for(ll q = s - range.first;q <= e - range.first;q++) {
            ans_min.first = std::min(ans_min.first,arr[q]);
        }
        for(ll q = s - range.first;q <= e - range.first;q++) {
            if(ans_min.first == arr[q]) {
                ans_min.second++;
            }
        }
        try_min = get_min(root,s,e);
        return try_min == ans_min;
    };
    for(int q =0 ;q < 20000;q++) {
        int mode = rand() % 2;
        if(mode) {
            std::pair<ll,ll> rangel = get_rand_range(range.first,range.second);
            ll x = (ll)get_rand_number() % 5;
            do_add(rangel.first,rangel.second,x);
        }
        else {
            std::pair<ll,ll> rangel = get_rand_range(range.first,range.second);
            if(!do_get_min(rangel.first,rangel.second)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int _ =0;
    std::cin >> _;
    for(int q = 1;q <= _;q++ ) {
        std::cout << "test number " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
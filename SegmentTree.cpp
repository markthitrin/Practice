#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using ll = long long;
using ull = unsigned ll;
const ll MININT = -1e18;
const ll MAXINT = 1e18;
class node {
public:
    ll value = MININT;
    node* left = nullptr;
    node* right = nullptr;  
};

ll get_mid(const ll& left, const ll& right) {
    return (right - left) / 2 + left;
}

node* root = new node();
void add(node* seg,ll pos, ll x,ll left = MININT,ll right = MAXINT) {
    if(left == right) {
        seg->value = x;
        return;
    }
    ll mid = get_mid(left, right);
    if(seg->left == nullptr) seg->left = new node();
    if(seg->right == nullptr) seg->right = new node();
    if(pos <= mid) 
        add(seg->left,pos,x,left,mid);
    else
        add(seg->right,pos,x,mid + 1,right);
    seg->value = std::max(seg->left->value,seg->right->value);
}

ll get_max(node* seg,ll s,ll e, ll left = MININT,ll right = MAXINT) {
    if(seg == nullptr) return MININT;
    if(s > right || e < left) return MININT;
    if(s <= left && right <= e) return seg->value;
    ll mid = get_mid(left, right);
    return std::max(get_max(seg->left,s,e,left,mid),get_max(seg->right,s,e,mid + 1,right));
}

ll get_rand_value() {
    return (ll)std::rand() << 49 | (ll)std::rand() << 34 | 
    (ll)std::rand() << 19 | (ll)std::rand() << 4 | (ll)std::rand() & (15);  
}

std::pair<ll,ll> get_rand_range(ll min, ll max) {
    std::pair<ll,ll> res((ull)get_rand_value() % ((ull)max - (ull)min + 1) + min,
                        (ull)get_rand_value() % ((ull)max - (ull)min + 1) + min);
    if(res.first > res.second) std::swap(res.first,res.second);
    return res;
}

bool run_test() {
    std::pair<ll,ll> arr_range = get_rand_range(-5000,5000);
    ll range = arr_range.second - arr_range.first + 1;
    std::vector<ll> arr(range, MININT);
    auto get_max_ans = [&](const ll& left,const ll& right)  {
        ll res = MININT;
        for(ll i = left - arr_range.first;i <= right - arr_range.first;i++) {
            res = std::max(res,arr[i]);
        }
        return res;
    };
    auto add_ans = [&](const ll& pos,const ll& x) {
        arr[pos - arr_range.first] = x;
    };
    for(int i= 0 ;i < 10000;i++) {
        int mode = rand() % 2;
        if(mode) {
            ll pos = (ull)get_rand_value() % range + arr_range.first;
            ll value = get_rand_value();
            add_ans(pos,value);
            add(root,pos,value);
        }
        else {
            std::pair<ll,ll> query = get_rand_range(arr_range.first,arr_range.second);
            if(get_max(root,query.first,query.second) != get_max_ans(query.first,query.second)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    std::srand(std::time(0));
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <= _;q++)  {
        root = new node();
        std::cout << "test " << q << " : ";
        if(run_test()) {
            std::cout << "Passed\n";
        }
        else {
            std::cout << "Fail\n";
        }
    }
    return 0;
}
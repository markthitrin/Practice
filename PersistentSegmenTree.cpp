#include <iostream>
#include <vector>
#include <ctime>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node{
public:
    ll value = MININT;
    node* left;
    node* right;
};

std::vector<node*> seg_vers;

node* lc(node* head) {
    if(head == nullptr) return nullptr;
    return head->left;
}

node* rc(node* head) {
    if(head == nullptr) return nullptr;
    return head->right;
}

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void add(node* head, node* root, ll pos, ll value, ll left = MININT, ll right = MAXINT) {
    if(head != nullptr) {
        root->value = head->value;
        root->left = head->left;
        root->right = head->right;
    }
    if(left == right) {
        root->value = std::max(root->value, value);
        return;
    }
    ll mid = get_mid(left,right);
    if(root->left == nullptr || pos <= mid) root->left = new node();
    if(root->right == nullptr || pos > mid) root->right = new node();
    if(pos <= mid)
        add(lc(head),root->left,pos,value,left,mid);
    else 
        add(rc(head),root->right,pos,value,mid + 1,right);
    root->value = std::max(root->left->value,root->right->value);
}

ll get_max(node* root,ll s,ll e, ll left = MININT, ll right = MAXINT) {
    if(root == nullptr) return MININT;
    if(s <= left && right <= e) return root->value;
    if(s > right || e < left) return MININT;
    ll mid = get_mid(left,right);
    return std::max(get_max(root->left,s,e,left,mid),get_max(root->right,s,e,mid + 1,right));
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

std::pair<ll,ll> get_rand_range(ll min, ll max) {
    std::pair<ll,ll> res((ull)get_rand_value() % (max - min + 1) + min,(ull)get_rand_value() % (max - min + 1) + min);
    if(res.first > res.second) std::swap(res.first,res.second);
    return res;
}

bool run_test() {
    seg_vers.clear();
    std::pair<ll,ll> range = get_rand_range(-500,500);
    std::vector<std::vector<ll>> vers_ans;
    
    seg_vers.push_back(new node());
    vers_ans.push_back(std::vector<ll>(range.second - range.first + 1, MININT));

    auto do_add = [&](int root_version,ll pos,ll value) {
        vers_ans.push_back(std::vector<ll>(vers_ans[root_version]));
        vers_ans.back()[pos - range.first] = std::max(vers_ans.back()[pos - range.first],value);

        seg_vers.push_back(new node());
        add(seg_vers[root_version],seg_vers.back(),pos,value);
    };

    auto do_get_max = [&](int version,ll s,ll e) {
        ll ans = MININT;
        for(int q = s - range.first;q <= e - range.first;q++) {
            ans = std::max(vers_ans[version][q],ans);
        }
        ll try_ans = get_max(seg_vers[version],s,e);
        return try_ans == ans;
    };

    for(int q = 0;q < 10000;q++) {
        int mode = rand() % 2;
        if(mode) {
            ll version = (ull)get_rand_value() % (vers_ans.size());
            ll pos = (ull)get_rand_value() % (range.second - range.first + 1) + range.first;
            ll value = get_rand_value();
            do_add(version,pos,value);
        }
        else {
            std::pair<ll,ll> range_query = get_rand_range(range.first,range.second);
            ll version = (ull)get_rand_value() % (vers_ans.size());
            if(!do_get_max(version,range_query.first,range_query.second)) {
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
    for(int q = 1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
    return 0;
}

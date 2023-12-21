#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node {
public:
    ll value = 0;
    ll to_add = 0;
    node* left = nullptr;
    node* right = nullptr;
};

ll get_mid(ll left, ll right) {
    return (right - left) / 2 + left;
}

node* lc(node* head) {
    if(head == nullptr) return nullptr;
    return head->left;
}

node* rc(node* head) {
    if(head == nullptr) return nullptr;
    return head->right;
}

node* mirror(node* head) {
    node* new_node = new node();
    if(head == nullptr) return new_node;
    new_node->value = head->value;
    new_node->to_add = head->to_add;
    new_node->left = head->left;
    new_node->right = head->right;
    return new_node;
}

ll interception(ll l0,ll l1,ll r0,ll r1) {
    return std::min(r1,r0) - std::max(l0,l1) + 1;
}

void fix(node* root,ll left,ll right) {
    ll mid = get_mid(left,right);
    root->left->value += (root->to_add) * (mid - left + 1);
    root->left->to_add += (root->to_add);
    root->right->value += (root->to_add) * (right - mid);
    root->right->to_add += (root->to_add);
    root->to_add = 0;
} 

void add(node* head,node* root,ll s,ll e,ll x,ll left = MININT,ll right = MAXINT) {
    if(s <= left && right <= e) {
        root->value += (x) * (right - left + 1);
        root->to_add += (x);
        return;
    }
    if(e < left || s > right) {
        return;
    }
    root->left = mirror(lc(head));
    root->right = mirror(rc(head));
    fix(root,left,right);
    ll mid = get_mid(left,right);
    add(lc(head),root->left,s,e,x,left,mid);
    add(rc(head),root->right,s,e,x,mid + 1,right);
    root->value = root->left->value + root->right->value;
}

ll get_sum(node* root,ll s,ll e,ll left = MININT,ll right = MAXINT) {
    if(root == nullptr) return 0;
    if(s <= left && right <= e) return root->value;
    if(s > right || e < left) return 0;
    ll mid = get_mid(left,right);
    return get_sum(root->left,s,e,left,mid) + get_sum(root->right,s,e,mid + 1,right) + interception(s,left,e,right) * root->to_add;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

std::pair<ll,ll> get_rand_range(ll min,ll max) {
    std::pair<ll,ll> res((ull)get_rand_value() % (max - min + 1) + min,(ull)get_rand_value() % (max - min + 1) + min);
    if(res.first > res.second) std::swap(res.first,res.second);
    return res;
}

void destroy_tree(node* root) {
    if(root == nullptr) return;
    if(root->left != NULL) destroy_tree(root->left);
    if(root->right != NULL) destroy_tree(root->right);
    delete root;
}

void destroy_tree_vector(std::vector<node*> vroot) {
    while(vroot.size()) {
        destroy_tree(vroot.back());
        vroot.pop_back();
    }
}

bool run_test() {
    std::pair<ll,ll> range = get_rand_range(-500,500);

    std::vector<node*> seg_vers;
    std::vector<std::vector<ll>> vers_ans;

    seg_vers.push_back(new node());
    vers_ans.push_back(std::vector<ll>(range.second - range.first + 1,0));

    auto do_add = [&](int root_version,ll s,ll e,ll x) {
        vers_ans.push_back(std::vector<ll>(vers_ans[root_version]));
        seg_vers.push_back(mirror(seg_vers[root_version]));

        for(int q = s - range.first;q <= e - range.first;q++) {
            vers_ans.back()[q] += x;
        }
        add(seg_vers[root_version],seg_vers.back(),s,e,x);
    };
    auto do_get_sum = [&](int version,ll s,ll e) {
        ll ans_sum = 0;
        for(int q = s - range.first;q <= e - range.first;q++) {
            ans_sum += vers_ans[version][q];
        }
        ll try_sum = get_sum(seg_vers[version],s,e);
        return try_sum == ans_sum;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = rand() % 2;
        if(mode) {
            ll version = (ull)get_rand_value() % vers_ans.size();
            std::pair<ll,ll> range_query = get_rand_range(range.first,range.second);
            ll value = get_rand_value() % 1000;
            do_add(version,range_query.first,range_query.second,value);
        }
        else {
            ll version = (ull)get_rand_value() % vers_ans.size();
            std::pair<ll,ll> range_query = get_rand_range(range.first,range.second);
            if(!do_get_sum(version,range_query.first,range_query.second)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
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
}
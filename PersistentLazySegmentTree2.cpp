#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node {
    public:
    ll value = 0;
    ll count = 0;
    ll to_add = 0;
    node* left = nullptr;
    node* right = nullptr;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

node* get_new_node(ll left,ll right) {
    node* new_node = new node();
    new_node->count = (right - left + 1);
    return new_node;
}

node* mirror(node* head, ll left,ll right) {
    node* new_node = get_new_node(left,right);
    if(head == nullptr) return new_node;
    new_node->value = head->value;
    new_node->count = head->count;
    new_node->to_add = head->to_add;
    new_node->left = head->left;
    new_node->right = head->right;
    return new_node;
}

void fix(node* root) {
    root->left->value += root->to_add;
    root->left->to_add += root->to_add;
    root->right->value += root->to_add;
    root->right->to_add += root->to_add;
    root->to_add = 0;
}

node* lc(node* head) {
    if(head == nullptr) return nullptr;
    return head->left;
}

node* rc(node* head) {
    if(head == nullptr) return nullptr;
    return head->right;
}

ll get_intercept(ll l0,ll l1,ll r0,ll r1) {
    return std::min(r0,r1) - std::max(l0,l1) + 1;
}

void add(node* head,node* root,ll s,ll e,ll x,ll left = MININT,ll right = MAXINT) {
    if(s <= left && right <= e) {
        root->value += x;
        root->to_add += x;
        return;
    }
    else if(s > right || e < left) {
        return;
    }
    ll mid = get_mid(left,right);
    root->left = mirror(lc(head),left,mid);
    root->right = mirror(rc(head),mid + 1,right);
    fix(root);
    add(lc(head),root->left,s,e,x,left,mid);
    add(rc(head), root->right,s,e,x,mid + 1,right);
    if(root->left->value == root->right->value) {
        root->value = root->left->value;
        root->count = root->left->count + root->right->count;
    }
    else if(root->left->value < root->right->value) {
        root->value = root->left->value;
        root->count = root->left->count;
    }
    else {
        root->value = root->right->value;
        root->count = root->right->count;
    }
    return;
}

std::pair<ll,ll> get_min(node* root,ll s,ll e,ll left = MININT,ll right = MAXINT) {
    if(s > right || e < left) return {MAXINT,0};
    if(root == nullptr) return {0,get_intercept(s,left,e,right)};
    if(s <= left && right <= e) return {root->value,root->count};
    ll mid = get_mid(left,right);
    std::pair<ll,ll> get_left,get_right;
    get_left = get_min(root->left,s,e,left,mid);
    get_right = get_min(root->right,s,e,mid + 1,right);
    get_left.first += root->to_add;
    get_right.first += root->to_add;
    if(get_left.first == get_right.first) return {get_left.first,get_left.second + get_right.second};
    return std::min(get_left,get_right);
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

bool run_test() {
    std::pair<ll,ll> test_range = get_rand_range(-500,500);
    std::vector<std::vector<ll>> ans_vers;
    ans_vers.push_back(std::vector<ll>(test_range.second - test_range.first + 1,0));

    std::vector<node*> seg_vers;
    seg_vers.push_back(get_new_node(test_range.first,test_range.second));
    auto do_add = [&](ll root_version,ll s,ll e,ll x) {
        ans_vers.push_back(std::vector<ll>(ans_vers[root_version]));
        for(int q = s - test_range.first;q <= e - test_range.first;q++) {
            ans_vers.back()[q] += x;
        }

        seg_vers.push_back(mirror(seg_vers[root_version],MININT,MAXINT));
        add(seg_vers[root_version],seg_vers.back(),s,e,x);
    };
    auto do_get_min = [&](ll version,ll s,ll e) {
        std::pair<ll,ll> get_ans(MAXINT,0);
        for(int q = s - test_range.first;q <= e - test_range.first;q++) {
            get_ans.first = std::min(get_ans.first,ans_vers[version][q]);
        }
        for(int q = s - test_range.first;q <= e - test_range.first;q++) {
            if(get_ans.first == ans_vers[version][q]) get_ans.second++;
        }
        
        std::pair<ll,ll> get_query;
        get_query = get_min(seg_vers[version],s,e);
        return get_ans == get_query;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll version = (ull)get_rand_value() % (ans_vers.size());
            std::pair<ll,ll> range = get_rand_range(test_range.first,test_range.second);
            ll value = get_rand_value() % 1000;
            do_add(version,range.first,range.second,value);
        }
        else { 
            ll version = (ull)get_rand_value() % (ans_vers.size());
            std::pair<ll,ll> range = get_rand_range(test_range.first,test_range.second);
            if(!do_get_min(version,range.first,range.second))
                return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q  =1;q <= _ ;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
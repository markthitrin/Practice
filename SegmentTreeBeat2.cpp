#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const ll MINRANGE = 0;
const ll MAXRANGE = 1e6;

class node{
public:
    node* left = nullptr;
    node* right = nullptr;
    ll max1 = 0;
    ll max2 = 0;
    ll max1_c = 0;
    ll min1 = 0;
    ll min2 = 0;
    ll min1_c = 0;
    ll sum = 0;
    ll to_add = 0;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void fix(node* root) {
    if(root == nullptr) return;
    if(root->left == nullptr && root->right == nullptr) return;

    root->sum = 0;
    if(root->left) root->sum += root->left->sum;
    if(root->right) root->sum += root->right->sum;

    std::map<ll, int> h1;
    if(root->left) {
        h1[root->left->max1] += root->left->max1_c;
        h1[root->left->max2] += 0;
    }
    if(root->right) {
        h1[root->right->max1] += root->right->max1_c;
        h1[root->right->max2] += 0;
    }
    auto it1 = h1.end();
    --it1;
    root->max1 = it1->first;
    root->max1_c = it1->second;
    --it1;
    root->max2 = it1->first;

    std::map<ll, int> h2;
    if(root->left) {
        h2[root->left->min1] += root->left->min1_c;
        h2[root->left->min2] += 0;
    }
    if(root->right) {
        h2[root->right->min1] += root->right->min1_c;
        h2[root->right->min2] += 0;
    }
    auto it2 = h2.begin();
    root->min1 = it2->first;
    root->min1_c = it2->second;
    ++it2;
    root->min2 = it2->first;
    
    root->to_add = 0;

    return;
}

node* build(const std::vector<ll>& a,int left = MINRANGE,int right = MAXRANGE) {
    if(right < 0 || left >= a.size()) return nullptr;
    node* root = new node();
    if(left == right) {
        root->max1 = root->min1 = a[left];
        root->max2 = MININT;
        root->min2 = MAXINT;
        root->max1_c = root->min1_c = 1;
        root->sum = a[left];
        root->to_add = 0;
        return root;
    }
    ll mid = get_mid(left,right);
    root->left = build(a,left,mid);
    root->right = build(a,mid + 1,right);
    fix(root);
    return root;
}

void push_min(node* root,ll x, bool L) {
    if(root == nullptr) return;
    if(x > root->max1) return;
    root->sum += (x - root->max1) * root->max1_c;
    root->max1 = x;
    if(L) root->min1 = root->max1;
    else if(root->min1 > x) root->min1 = x;
    else if(root->min2 > x) root->min2 = x;
    return;
}

void push_max(node* root,ll x, bool L) {
    if(root == nullptr) return;
    if(x < root->min1) return;
    root->sum += (x - root->min1) * root->min1_c;
    root->min1 = x;
    if(L) root->max1 = root->min1;
    else if(root->max1 < x) root->max1 = x;
    else if(root->max2 < x) root->max2 = x;
    return;
}

void push_add(node* root,ll x,int left,int right) {
    if(root == nullptr) return;
    root->sum += (right - left + 1) * x;
    root->max1 += x;
    if(root->max2 != MININT) root->max2 += x;
    root->min1 += x;
    if(root->min2 != MAXINT) root->min2 += x;
    root->to_add += x;
    return;
}

void push_down(node* root,int left,int right) {
    ll mid = get_mid(left,right);
    push_add(root->left,root->to_add,left,mid);
    push_add(root->right,root->to_add,mid + 1,right);
    root->to_add = 0;

    push_min(root->left,root->max1, mid == left);
    push_min(root->right,root->max1, mid + 1 == right);

    push_max(root->left,root->min1, mid == left);
    push_max(root->right,root->min1, mid + 1 == right);
}

void set_range_min(node* root,int s,int e,ll x,int left = MINRANGE, int right = MAXRANGE) {
    if(root == nullptr) return;
    if(right < s || left > e) return;
    if(x > root->max1) return;
    if(s <= left && right <= e && x > root->max2) {
        push_min(root,x,left == right);
    }   
    else {
        ll mid = get_mid(left,right);
        push_down(root,left, right);
        set_range_min(root->left,s,e,x,left,mid);
        set_range_min(root->right,s,e,x,mid + 1,right);
        fix(root);
    }
}

void set_range_max(node* root,int s,int e,ll x,int left = MINRANGE,int right = MAXRANGE) {
    if(root == nullptr) return;
    if(right < s || left > e) return;
    if(x < root->min1) return;
    if(s <= left && right <= e && x < root->min2) {
        push_max(root,x, left == right);
    }
    else {
        ll mid = get_mid(left,right);
        push_down(root,left,right);
        set_range_max(root->left,s,e,x,left,mid);
        set_range_max(root->right,s,e,x,mid + 1,right);
        fix(root);
    }
}

void set_range_add(node* root,int s,int e,ll x,int left = MINRANGE,int right = MAXRANGE) {
    if(root == nullptr) return;
    if(right < s || left > e) return;
    if(s <= left && right <= e) {
        push_add(root, x,left,right);
    }
    else {
        ll mid = get_mid(left,right);
        push_down(root,left,right);
        set_range_add(root->left,s,e,x,left,mid);
        set_range_add(root->right,s,e,x,mid + 1,right);
        fix(root);
    }
}

ll get_sum(node* root,int s,int e,int left = MINRANGE, int right = MAXRANGE) {
    if(root == nullptr) return 0;
    if(right < s || left > e) return 0;
    if(s <= left && right <= e) return root->sum;
    ll mid = get_mid(left,right);
    push_down(root,left,right);
    ll get_sum_left = get_sum(root->left,s,e,left,mid);
    ll get_sum_right = get_sum(root->right,s,e,mid + 1,right);
    return get_sum_left + get_sum_right;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll vector_size = (ull)get_rand_value() % 20 + 20;
    std::vector<ll> a(vector_size);
    for(int q = 0;q < a.size();q++) {
        a[q] = get_rand_value() % 10;
    }
    node* root = build(a);

    auto do_set_range_min = [&](int left,int right,ll x) {
        for(int q = left;q <=right;q++) {
            a[q] = std::min(a[q],x);
        }
        set_range_min(root,left,right,x);
    };
    auto do_set_range_max = [&](int left,int right,ll x) {
        for(int q = left;q <= right;q++) {
            a[q] = std::max(a[q],x);
        }
        set_range_max(root,left,right,x);
    };
    auto do_set_range_add = [&](int left,int right,ll x) {
        for(int q = left;q<=right;q++) {
            a[q] += x;
        }
        set_range_add(root,left,right,x);
    };
    auto do_get_sum = [&](int left,int right) {
        ll ans = 0;
        for(int q = left;q <=right;q++) {
            ans += a[q];
        } 
        ll get_ans = get_sum(root,left,right);
        if(ans != get_ans) 
            std::cout << ans << " " << get_ans << std::endl;
        return ans == get_ans;
    };
    for(int q = 0;q < 5000;q++) {
        int mode = std::rand() % 4;
        if(mode == 0) {
            int left = (ull)get_rand_value() % vector_size;
            int right = (ull)get_rand_value() % vector_size;
            if(left > right) std::swap(left,right);
            ll x = get_rand_value() % 10;
            do_set_range_min(left,right,x);
        }
        else if(mode == 1) {
            int left = (ull)get_rand_value() % vector_size;
            int right = (ull)get_rand_value() % vector_size;
            if(left > right) std::swap(left,right);
            ll x = get_rand_value() % 1000000;
            do_set_range_max(left,right,x);
        }
        else if (mode == 2) {
            int left = (ull)get_rand_value() % vector_size;
            int right = (ull)get_rand_value() % vector_size;
            if(left > right) std::swap(left,right);
            ll x = get_rand_value() % 1000000;
            do_set_range_add(left,right,x);
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
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <Math.h>
 
using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
ll N;
ll sqrt_N = 1;

class node {
public:
    ll value = 0;
    ll count = 0;
    node* left = nullptr;
    node* right = nullptr;
};

class query{
public:
    ll query_number =0;
    ll left = 0;
    ll right = 0;
    query() {}
    // bool operator<(const query& other) const{
    //     if(left / sqrt_N != other.left / sqrt_N) return left < other.left;
    //     return right < other.right;
    // }
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void add(node* root,ll pos,ll left = MININT, ll right = MAXINT) {
    if(right == left) {
        root->value = left;
        root->count++;
        return;
    }
    int mid = get_mid(left,right);
    if(pos <= mid) {
        add(root->left,pos,left,mid);
    }
    else {
        add(root->right,pos,mid + 1,right);
    }
    if(root->left->count > root->right->count)  {
        root->value = root->left->value;
        root->count = root->left->count;
    }
    else {
        root->value = root->right->value;
        root->count = root->right->count;
    }
}

void subtract(node* root,ll pos,ll left = MININT,ll right = MAXINT) {
    if(right == left) {
        root->value = left;
        root->count--;
        return;
    }
    int mid = get_mid(left,right);
    if(pos <= mid) {
        add(root->left,pos,left,mid);
    }
    else {
        add(root->right,pos,mid + 1,right);
    }
    if(root->left->count > root->right->count)  {
        root->value = root->left->value;
        root->count = root->left->count;
    }
    else {
        root->value = root->right->value;
        root->count = root->right->count;
    }
}

ll get_mode(node* root) {
    return root->value;
}

std::vector<ll> MO_algorithm(std::vector<ll>& a,std::vector<query> Q) {
    std::sort(Q.begin(),Q.end(),[](query qu1,query qu2) {
        if(qu1.left / sqrt_N != qu2.left / sqrt_N) return qu1.left < qu2.left;
        return qu1.right < qu2.right;
    });
    ll cur_left = a.size();
    ll cur_right = -1;
    std::vector<ll> ans(Q.size(),0);
    node* root = new node();
    for(int q = 0;q < Q.size();q++) {
        while(cur_left < Q[q].left) {
            subtract(root,a[cur_left]);
            ++cur_left;
        }
        while(cur_left > Q[q].left) {
            --cur_left;
            add(root,a[cur_left]);
        }
        while(cur_right > Q[q].right) {
            subtract(root,a[cur_right]);
            --cur_right;
        }
        while(cur_right < Q[q].right) {
            ++cur_right;
            add(root,a[cur_right]);
        }
        ans[Q[q].query_number] = get_mode(root);
    }
    return ans;
}

ll get_mode_brute_force(std::vector<ll>& a,ll left,ll right) {
    std::map<ll,ll> mm;
    ll mode_value;
    ll count = 0;
    for(int q = left;q <= right;q++) {
        mm[a[q]]++;
        if(mm[a[q]] > count) {
            count = mm[a[q]];
            mode_value = a[q];
        }
        else if(mm[a[q]] == count && a[q] > mode_value) {
            count = mm[a[q]];
            mode_value =a[q];
        }
    }
    return mode_value;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    N = (ull)get_rand_value() % 20000 + 20000;
    ll M = (ull)get_rand_value() % 20000 + 20000;
    sqrt_N = std::sqrt(N);
    std::vector<ll> a(N);
    std::vector<query> qu;
    for(int q = 0;q < a.size();q++) {
        a[q] = get_rand_value() % 1000;
    }
    for(int q = 0;q < M;q++) {
        qu.push_back(query());
        qu[q].left = (ull)get_rand_value() % N;
        qu[q].right = (ull)get_rand_value() % N;
        qu[q].query_number = q;
    }
    std::vector<ll> ans(qu.size());
    for(int q =0 ;q <qu.size();q++) {
        ans[q] = get_mode_brute_force(a,qu[q].left,qu[q].right); 
    }
    return ans == MO_algorithm(a, qu);
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
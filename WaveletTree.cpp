#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node{
public:
    node* left = nullptr;
    node* right = nullptr;
    std::vector<int> pos;
    std::vector<int> m_left;
    std::vector<int> m_right;
};

ll get_mid(ll left,ll right) {
    return (right - left) /2 + left;
}

node* recur1(const std::vector<ll>& a,const std::vector<int>& p,ll left,ll right) {
    node* new_node = new node();
    if(right == left) {
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->pos = p;
        new_node->m_left = std::vector(p.size(),0);
        new_node->m_right = std::vector(p.size(),0);
        return new_node;
    }
    ll mid = get_mid(left,right);
    std::vector<int> left_p;
    std::vector<int> right_p;
    for(int q = 0;q < p.size();q++){
        if(a[p[q]] <= mid) {
            left_p.push_back(p[q]);
        }
        else {
            right_p.push_back(p[q]);
        }
        new_node->m_left.push_back(left_p.size());
        new_node->m_right.push_back(right_p.size());
    }
    if(left_p.size())
        new_node->left = recur1(a,left_p,left,mid);
    if(right_p.size())
        new_node->right = recur1(a,right_p,mid + 1,right);
    return new_node;
}

node* built_wavelet(const std::vector<ll>& a) {
    std::vector<int> p(a.size(), 0);
    for(int q = 0;q < a.size();q++){
        p[q] = q;
    }
    return recur1(a,p,MININT,MAXINT);
}

bool recur2(node* root,int left,int right,int& k,int& ans) {
    if(right - left < k) {
        k -= (right - left);
        return false;
    }
    auto map_begin = [&](std::vector<int>& map) {
        if(left == 0) return 0;
        return map[left - 1];
    };
    auto map_end = [&](std::vector<int>& map) {
        if(right == 0) return 0;
        return map[right - 1];
    };
    if(recur2(root->left,map_begin(root->m_left),map_end(root->m_left),k,ans)) return true;
    if(recur2(root->right,map_begin(root->m_right),map_end(root->m_right),k,ans)) return true;
    ans = root->pos[left + k - 1];
    return true;
}

ll get_k_smallest(const std::vector<ll>& a,node* root,int left,int right,int k) {
    int ans = 0;
    recur2(root,left,right + 1,k,ans);
    return a[ans];   
}

ll get_k_smallest_brute_force(std::vector<ll>& a,int left,int right,int k) {
    std::vector<ll>b(a.begin() + left,a.begin() + right + 1);
    std::sort(b.begin(),b.end());
    return b[k - 1];
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int vector_size = (ull)get_rand_value() % 3000 + 3000;
    std::vector<ll> a;
    for(int q = 0;q < vector_size;q++) {
        a.push_back(get_rand_value() % 3000);
    }
    node* root = built_wavelet(a);
    auto do_check = [&](int left,int right,int k) {
        ll ans = get_k_smallest_brute_force(a,left,right,k);
        ll get_ans = get_k_smallest(a,root,left,right,k);
        return true;
    };
    for(int q = 0;q < 3000;q++) {
        int left = (ull)get_rand_value() % vector_size;
        int right =(ull)get_rand_value() % vector_size;
        if(left > right)std::swap(left,right);
        int k = (ull)get_rand_value() % (right - left + 1) + 1;
        if(!do_check(left,right,k)) return false; 
    }
    return true;;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q =1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test() ) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
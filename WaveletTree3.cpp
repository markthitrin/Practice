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
    return (right - left) / 2 + left;
}

node* built_wavelet(const std::vector<ll>& a,const std::vector<int>& p,ll left = MININT,ll right = MAXINT) {
    node* new_node = new node();
    new_node->pos = p;
    if(right == left) {
        new_node->m_left = std::vector<int>(p.size(),0);
        new_node->m_right = std::vector<int>(p.size(),0);
        return new_node;
    }

    ll mid = get_mid(left,right);
    std::vector<int> p_left;
    std::vector<int> p_right;
    for(int q = 0;q < p.size();q++) {
        if(a[p[q]] <= mid) {
            p_left.push_back(p[q]);
        }
        else {
            p_right.push_back(p[q]);
        }
        new_node->m_left.push_back(p_left.size());
        new_node->m_right.push_back(p_right.size());
    }
    if(p_left.size()) {
        new_node->left = built_wavelet(a,p_left,left,mid);
    }   
    if(p_right.size()) {
        new_node->right = built_wavelet(a,p_right,mid + 1,right);
    }
    return new_node;
}

node* push_back_wavelet(node* root,ll pos,ll value,ll left = MININT,ll right = MAXINT) {
    node* r = root;
    ll mid = get_mid(left,right);
    if(r == nullptr) {
        r = new node();
    }
    r->pos.push_back(pos);
    auto back_array = [&](const std::vector<int>& a) {
        if(a.size()) return a.back();
        return 0;
    };
    if(value <= mid) {
        r->m_left.push_back(back_array(r->m_left) + 1);
        r->m_right.push_back(back_array(r->m_right));
        if (left != right) r->left = push_back_wavelet(r->left,pos,value,left,mid);
    }
    else {
        r->m_left.push_back(back_array(r->m_left));
        r->m_right.push_back(back_array(r->m_right) + 1);
        if (left != right) r->right = push_back_wavelet(r->right,pos,value,mid + 1,right);
    }
    return r;
}

ll recur1(node* root,ll left,ll right,ll x,ll y,ll min_value = MININT,ll max_value = MAXINT) {
    if(right - left == 0) return 0;
    if(x <= min_value && max_value <= y) return right - left;
    ll mid = get_mid(min_value,max_value);
    auto get_begin = [&](const std::vector<int>& a) {
        if(left == 0) return 0;
        return a[left - 1];
    };
    auto get_end = [&](const std::vector<int>& a) {
        if(right == 0) return 0;
        return a[right - 1];
    };
    ll get_count_left = recur1(root->left,get_begin(root->m_left),get_end(root->m_left),x,y,min_value,mid);
    ll get_count_right = recur1(root->right,get_begin(root->m_right),get_end(root->m_right),x,y,mid + 1,max_value);
    return get_count_left + get_count_right;
}

ll get_count(node* root,ll left,ll right,ll x,ll y) {
    return recur1(root,left,right + 1,x,y);
}

ll get_count_brute_force(const std::vector<ll>& a,int left,int right,ll x,ll y) {
    std::vector<int>b(a.begin() + left,a.begin() + right + 1);
    std::sort(b.begin(),b.end());
    return std::upper_bound(b.begin(),b.end(),y) - std::lower_bound(b.begin(),b.end(),x);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll vector_size = (ull)get_rand_value() % 3000 + 3000;
    std::vector<ll> a(vector_size,0);
    std::vector<int> p(vector_size);
    for(int q = 0;q <a.size();q++) {
        a[q] = get_rand_value() % 300;
        p[q] = q;
    }
    node* root = built_wavelet(a,p);
    auto do_add = [&](ll value) {
        a.push_back(value);
        push_back_wavelet(root,a.size() - 1,value);
    };
    auto do_check = [&](int left,int right,ll x,ll y) {
        ll ans = get_count_brute_force(a,left,right,x,y);
        ll get_ans = get_count(root,left,right,x,y);
        return ans == get_ans;
    };
    for(int q = 0;q < 4000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll value = get_rand_value() % 300;
            do_add(value);
        }
        else {
            ll left = (ull)get_rand_value() % vector_size;
            ll right = (ull)get_rand_value() % vector_size;
            if(left > right) std::swap(left,right);
            ll x = get_rand_value() % 300;
            ll y = get_rand_value() % 300;
            if(x > y) std::swap(x,y);
            if(!do_check(left,right,x,y)) return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _ ;
    for(int q =1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
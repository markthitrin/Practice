#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node {
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

node* built_wavelet(const std::vector<ll>& a, const std::vector<int>& p,ll left = MININT, ll right = MAXINT) {
    node* new_node = new node;
    new_node->pos = p;
    if(right == left) {
        new_node->m_left = std::vector<int>(p.size(),0);
        new_node->m_right = std::vector<int>(p.size(),0);
        return new_node;
    }
    ll mid = get_mid(left,right);
    std::vector<int> p_left;
    std::vector<int> p_right;
    for(int q = 0;q < p.size();q++){
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

void recur2(ll value,node* root,int local_pos,int to,ll left,ll right) {
    if(root == nullptr) return;
    root->pos[local_pos] = to;
    auto get_map_pos = [&](const std::vector<int>& a) {
        if(local_pos == 0) return 0;
        return a[local_pos - 1];
    };
    ll mid = get_mid(left,right);
    if(value <= mid) {
        recur2(value,root->left,get_map_pos(root->m_left),to,left,mid);
    }
    else {
        recur2(value,root->right,get_map_pos(root->m_right),to,mid + 1,right);
    }
}

void swap_wavelet(const std::vector<ll>& a, node* root,int pos,ll local_pos,ll left = MININT, ll right = MAXINT) {
    if(root == nullptr) return;
    ll mid = get_mid(left,right);
    bool is_left1 = a[pos] <= mid;
    bool is_left2 = a[pos + 1] <= mid;
    auto get_map_pos = [&](const std::vector<int>& a) {
        if(local_pos == 0) return 0;
        return a[local_pos - 1];
    };
    if(is_left1 != is_left2) {
        if(is_left1) {
            root->m_left[local_pos]--;
            root->m_right[local_pos]++;
            recur2(a[pos],root->left,get_map_pos(root->m_left),root->pos[local_pos + 1],left,mid);
            recur2(a[pos + 1],root->right,get_map_pos(root->m_right),root->pos[local_pos],mid + 1,right);
        }
        else {
            root->m_left[local_pos]++;
            root->m_right[local_pos]--;
            recur2(a[pos + 1],root->left,get_map_pos(root->m_left),root->pos[local_pos],left,mid);
            recur2(a[pos],root->right,get_map_pos(root->m_right),root->pos[local_pos + 1],mid + 1,right);
        }
        return;
    }
    else {
        if(a[pos] <= mid) {
            swap_wavelet(a,root->left,pos,get_map_pos(root->m_left),left,mid);
        }
        else {
            swap_wavelet(a,root->right,pos,get_map_pos(root->m_right),mid + 1,right);
        }
    }
}

bool recur1(node* root,int left,int right,int& k,int& ans) {
    if(right - left < k) {
        k -= (right - left);
        return false;
    }
    auto get_begin = [&](const std::vector<int>& a) {
        if(left == 0) return 0;
        return a[left - 1];  
    };
    auto get_end = [&](const std::vector<int>& a) {
        if(right == 0) return 0;
        return a[right - 1];
    };
    if(recur1(root->left,get_begin(root->m_left),get_end(root->m_left),k,ans)) return true;
    if(recur1(root->right,get_begin(root->m_right),get_end(root->m_right),k,ans)) return true;
    ans = root->pos[left + k - 1];
    return true;
}

ll get_k_smallest(const std::vector<ll>& a,node* root,int left,int right,int k) {
    int ans = 0;
    recur1(root,left,right + 1,k,ans);
    return a[ans];
}

ll get_k_smallest_brute_force(const std::vector<ll>& a,int left,int right,int k) {
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
    std::vector<ll> a(vector_size);
    std::vector<int> p(vector_size);
    for(int q = 0;q < a.size();q++) {
        a[q] = get_rand_value() % 300;
        p[q] = q;
    }
    node* root = built_wavelet(a,p);
    auto do_swap = [&](int i) {
        swap_wavelet(a,root,i,i);
        std::swap(a[i],a[i + 1]);
    };
    auto do_get_k_smallest = [&](int left,int right,int k) {
        ll ans = get_k_smallest_brute_force(a,left,right,k);
        ll get_ans = get_k_smallest(a,root,left,right,k);
        if(ans != get_ans)
            std::cout << ans << " " << get_ans << std::endl;
        return ans == get_ans;
    };
    for(int q = 0;q < 3000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            int i  =(ull)get_rand_value() % (vector_size - 1);
            do_swap(i);
        }
        else {
            int left=  (ull)get_rand_value() % (vector_size);
            int right = (ull)get_rand_value() % (vector_size);
            if(left > right) std::swap(left,right);
            int k = (ull)get_rand_value() % (right - left + 1) + 1;
            if(!do_get_k_smallest(left,right,k)) return false;
        }  
    }
    return true;
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
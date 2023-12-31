#include <iostream>
#include <vector>

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

class vertex{
    public:
    ll HLD_seg;
    ll HLD_id;
    ll chain_chid = -1;
    ll value = MININT;
    ll level;
    std::vector<ll> parents;
};

std::vector<std::vector<ll>> f;
std::vector<vertex> v;
std::vector<node*> segs;
std::vector<ll> seg_parent;

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

void add(node* root,ll pos,ll value,ll left = MININT,ll right = MAXINT) {
    if(right == left) {
        root->value = value;
        return;
    }
    ll mid = get_mid(left,right);
    if(root->left == nullptr) root->left = new node();
    if(root->right == nullptr) root->right = new node();
    if(pos <= mid) {
        add(root->left,pos,value,left,mid);
    }
    else {
        add(root->right,pos,value,mid + 1,right);
    }
    root->value = std::max(root->left->value,root->right->value);
}

ll get_max(node* root,ll s,ll e,ll left = MININT,ll right = MAXINT) {
    if(root == nullptr) return MININT;
    if(s <= left && right <= e) return root->value;
    if(s > right || e < left) return MININT;
    ll mid = get_mid(left,right);
    return std::max(get_max(root->left,s,e,left,mid),get_max(root->right,s,e,mid + 1,right));
}

ll recur_build(ll u,ll p) {
    v[u].level = v[p].level + 1;
    v[u].parents.push_back(p);

    ll pp = v[u].parents[0];
    ll ii = 0;
    while(v[pp].parents.size() > ii) {
        v[u].parents.push_back(v[pp].parents[ii]);
        ++ii;
        pp = v[u].parents.back();
    }

    ll max_dept = 0;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q] != p) {
            ll get_dept = recur_build(f[u][q], u);
            if(get_dept > max_dept) {
                v[u].chain_chid = f[u][q];
                max_dept = get_dept;
            }
        }
    }
    return max_dept + 1;
}

void build(ll root) {
    v[root].level = 0;
    for(int q = 0;q < f[root].size();q++) {
        recur_build(f[root][q], root);       
    }
}

void recur_build2(ll u, ll p,ll seg,ll id) {
    v[u].HLD_id = id;
    v[u].HLD_seg = seg;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q] != p) {
            if(f[u][q] == v[u].chain_chid) {
                recur_build2(f[u][q],u,seg,id + 1);
            }
            else {
                seg_parent.push_back(u);
                segs.push_back(new node());
                recur_build2(f[u][q],u,segs.size() - 1,0);
            }
        }
    }
}

ll LCA(ll x,ll y) {
    if(v[y].level < v[x].level) std::swap(x,y);
    ll diff = v[y].level - v[x].level;
    for(int q = 30;q>=0;q--){
        if(diff & (1 << q)) {
            y = v[y].parents[q];
        }
    }
    for(int q = 30;q>=0;q--) {
        if(v[x].parents.size() > q) {
            if(v[x].parents[q] != v[y].parents[q]) {
                x = v[x].parents[q];
                y = v[y].parents[q];
            }
        }
    }
    if(x == y) {
        return x;
    }
    return v[y].parents[0];
}

void change_value(ll u,ll x) {
    add(segs[v[u].HLD_seg],v[u].HLD_id,x);
}

ll query(ll x,ll y) {
    ll lca = LCA(x,y);
    ll ans = MININT;
    for(auto i : {x,y}) {
        while(v[i].HLD_seg != v[lca].HLD_seg) {
            ans = std::max(ans,get_max(segs[v[i].HLD_seg],0,v[i].HLD_id));
            i = seg_parent[v[i].HLD_seg];
        }
        ans = std::max(ans,get_max(segs[v[i].HLD_seg],v[lca].HLD_id,v[i].HLD_id));
    }
    return ans;
}

ll LCA_get_max_brute_force(ll x,ll y) {
    ll ans = std::max(v[x].value,v[y].value);
    if(v[y].level < v[x].level) std::swap(x,y);
    while(v[y].level > v[x].level) {
        y = v[y].parents[0];
        ans = std::max(v[y].value, ans);
    }
    while(x != y) {
        x = v[x].parents[0];
        y = v[y].parents[0];
        ans = std::max(v[y].value,ans);
        ans = std::max(v[x].value,ans);
    }
    return ans;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

void destroy_node(node* root) {
    if(root == nullptr) return;
    destroy_node(root->left);
    destroy_node(root->right);
    delete root;
}

bool run_test() {
    while(segs.size()) {
        destroy_node(segs.back());
        segs.pop_back();
    }
    seg_parent.clear();
    f.clear();
    v.clear();
    
    ll vertex_number = (ull)get_rand_value() % (20000) + 50000;
    v.push_back(vertex());
    for(int q = 0;q < vertex_number;q++) f.push_back(std::vector<ll>());
    for(int q = 1;q < vertex_number;q++) {
        ll parent = (ull)get_rand_value() % (q);
        v.push_back(vertex());
        f[parent].push_back(q);
        f[q].push_back(parent);
    }

    build(0);
    segs.push_back(new node());
    seg_parent.push_back(-1);
    recur_build2(0,-1,0,0);

    auto do_change = [&](ll pos,ll value) {
        v[pos].value = value;
        change_value(pos,value);
    };

    auto do_get_max = [&](ll x, ll y) {
        ll get_ans = LCA_get_max_brute_force(x,y);
        ll try_get_max = query(x,y);
        return try_get_max == get_ans;
    };

    for(int q = 0;q<10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            ll pos = (ull)get_rand_value() % (vertex_number);
            ll value = get_rand_value() % 1000;
            do_change(pos,value);
        }
        else {
            ll x = (ull)get_rand_value() % (vertex_number);
            ll y = (ull)get_rand_value() % (vertex_number);
            if(!do_get_max(x,y)) {
                return false;
            }
        }
    }
    return true;
}

int main () {
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
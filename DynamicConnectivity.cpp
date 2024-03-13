// This coded is test by https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using ll = long long;

const ll MINQ = 0;
const ll MAXQ = 300006;

class node{
public:
    int p;
    int rnk;
    ll sum_value = 0;
};

class edge {
public:
    int u,v;
    bool united = false;
};

class increase{
public:
    int u;
    ll value;
};

class seg_node {
public:
    std::vector<edge> e;
    std::vector<increase> add;
};

class dsu_rollback_node {
public:
    int px,py;
    int prnk;
};

int get_mid(int left,int right) {
    return (right - left) / 2 + left;
}

int get_dsu_parent(const std::vector<node>& v,int x) {
    while(v[x].p != x) x = v[x].p;
    return x;
}

ll get_comp_sum(const std::vector<node>& v,int x) {
    return v[get_dsu_parent(v,x)].sum_value;
}

void increase_value(std::vector<node>& v,int x,ll value) {
    while(v[x].p != x) {
        v[x].sum_value += value;
        x = v[x].p;
    }
    v[x].sum_value += value;
}

bool connect_vertex(std::vector<node>& v,int x,int y,int& px,int& py,int& prnk) {
    x = get_dsu_parent(v,x);
    y = get_dsu_parent(v,y);
    
    if(x == y) return false;
    if(v[x].rnk > v[y].rnk) std::swap(x,y);
    px = x;
    py = y;
    prnk = v[y].rnk;
    v[x].p = y;
    v[y].rnk = std::max(v[y].rnk,v[x].rnk + 1);
    v[y].sum_value += v[x].sum_value;
    return true;
}

void roll_back(std::vector<node>& v,int px,int py,int prnk) {
    if(v[py].p == px) std::swap(px,py);
    v[px].p = px;
    v[py].rnk = prnk;
    v[py].sum_value -= v[px].sum_value;
    return; 
}

void add_united_seg(std::vector<seg_node>& root,edge new_edge,int s,int e,int left = MINQ,int right = MAXQ,int i = 1) {
    if(s <= left && right <= e) {
        root[i].e.push_back(new_edge);
        return;
    }
    if(right < s || left > e) {
        return ;
    }
    int mid = get_mid(left,right);
    add_united_seg(root,new_edge,s,e,left,mid,2*i);
    add_united_seg(root,new_edge,s,e,mid +1,right,2 * i + 1);
}

void add_increase_seg(std::vector<seg_node>& root,increase new_increase,int s,int e,int left = MINQ,int right = MAXQ,int i = 1) {
    if(s <= left && right <= e) {
        root[i].add.push_back(new_increase);
        return;
    }
    if(right < s || left > e) {
        return;
    }
    int mid = get_mid(left,right);
    add_increase_seg(root,new_increase,s,e,left,mid,2*i);
    add_increase_seg(root,new_increase,s,e,mid + 1,right,2*i + 1);
}

void traverse(std::vector<ll>& ans,std::vector<seg_node>& root,std::vector<node>& v,
std::vector<dsu_rollback_node>& dsu_rollback,int left = MINQ,int right = MAXQ,int i = 1) {
    for(int q = 0;q < root[i].e.size();q++) {
        int px,py,prnk;
        root[i].e[q].united = connect_vertex(v,root[i].e[q].u,root[i].e[q].v,px,py,prnk);
        if(root[i].e[q].united) {
            dsu_rollback_node new_node;
            new_node.px = px;
            new_node.py = py;
            new_node.prnk = prnk;
            dsu_rollback.push_back(new_node);
        }
    }
    for(int q = 0;q < root[i].add.size();q++) {
        increase_value(v,root[i].add[q].u,root[i].add[q].value);
    }
    if(left == right) {
        if(ans[left] != -1) {
            ans[left] = get_comp_sum(v,ans[left]);
        }
    }
    else {
        int mid = get_mid(left,right);
        traverse(ans,root,v,dsu_rollback,left,mid,2*i);
        traverse(ans,root,v,dsu_rollback,mid + 1,right,2*i + 1);
    }
    for(int q = 0;q < root[i].add.size();q++) {
        increase_value(v,root[i].add[q].u,-root[i].add[q].value);
    }
    for(int q = root[i].e.size() - 1;q >= 0;q--) {
        if(root[i].e[q].united) {
            roll_back(v,dsu_rollback.back().px,dsu_rollback.back().py,dsu_rollback.back().prnk);
            dsu_rollback.pop_back();
        }
        root[i].e[q].united = false;
    }
}

void solve() {
    int N,Q;
    std::cin >> N >> Q;
    std::vector<node> v(N);
    std::vector<seg_node> root(4 * Q + 5);
    std::vector<ll> ans(Q,-1);
    std::map<std::pair<int,int>,std::vector<int>> mm;
    std::vector<dsu_rollback_node> dsu_rollback;
    for(int q = 0;q < N;q++) {
        std::cin >> v[q].sum_value;
        v[q].p = q;
        v[q].rnk = 1;
    }
    for(int q = 0;q < Q;q++) {
        int mode = 0;
        std::cin >> mode;
        if(mode == 0) {
            int u,v;
            std::cin >> u >> v;
            if(u > v)std::swap(u,v);
            mm[std::pair<int,int>(u,v)].push_back(q);
        }
        else if(mode == 1) {
            int u,v;
            std::cin >> u >> v;
            if(u > v)std::swap(u,v);
            mm[std::pair<int,int>(u,v)].push_back(q);
        }
        else if(mode == 2) {
            int v,x;
            std::cin >> v >> x;
            increase new_increase;
            new_increase.u = v;
            new_increase.value = x;
            add_increase_seg(root,new_increase,q,Q - 1,0,Q - 1);
        }
        else if(mode == 3) {
            int v;
            std::cin >> v;
            ans[q] = v;
        }
    }
    for(auto it = mm.begin();it != mm.end();it++) {
        std::vector<int>& select = it->second;
        edge new_edge;
        new_edge.u = it->first.first;
        new_edge.v = it->first.second;
        int n1 = 0;
        while(n1 + 1 < select.size()) {
            add_united_seg(root,new_edge,select[n1],select[n1 + 1] - 1,0,Q - 1);
            n1 += 2;
        }
        if(n1 < select.size()) {
            add_united_seg(root,new_edge,select[n1],Q - 1,0,Q - 1);
        }
    }
    traverse(ans,root,v,dsu_rollback,0,Q - 1);
    for(int q = 0 ;q < ans.size();q++) {
        if(ans[q] != -1) {
            std::cout << ans[q] << "\n";
        }
    }
}

int main() {
    solve();
    return 0;
}
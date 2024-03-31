// This code is tested by https://cses.fi/problemset/task/1703/

#include <iostream>
#include <algorithm>
#include <vector>

const int MAXN = 200005;

class vertex{
public:
    int number;
    int dfs_label = MAXN;
    int dfs_p = MAXN;
    int dsu_p = 0;
    int sdom = MAXN;
    int min_sdom_node = MAXN;
    int min_sdom = MAXN;
    int idom = MAXN;
    std::vector<int> sdom_in;
};

void dfs(int u,std::vector<vertex>& v,const std::vector<std::vector<int>>& f,std::vector<std::vector<int>>& inv_fd,int& label) {
    v[u].dfs_label = label++;
    for(int q = 0;q < f[u].size();q++) {
        if(v[f[u][q]].dfs_label == MAXN) {
            dfs(f[u][q],v,f,inv_fd,label);
            v[f[u][q]].dfs_p = v[u].dfs_label;
        }
        inv_fd[v[f[u][q]].dfs_label].push_back(v[u].dfs_label);
    }
}

void merge(int x,int par,std::vector<vertex>& v) {
    v[x].dsu_p = par;
}

void fix(int x,std::vector<vertex>& v) {
    if(v[x].dsu_p == x) return;
    fix(v[x].dsu_p,v);
    if(v[v[x].dsu_p].dsu_p == v[x].dsu_p) return;
    if(v[v[x].dsu_p].min_sdom < v[x].min_sdom) {
        v[x].min_sdom = v[v[x].dsu_p].min_sdom;
        v[x].min_sdom_node = v[v[x].dsu_p].min_sdom_node;
    }
    v[x].dsu_p = v[v[x].dsu_p].dsu_p;
}

int get_min_sdom(int x,std::vector<vertex>& v) {
    fix(x,v);
    return v[x].min_sdom;
}

int get_min_sdom_node(int x,std::vector<vertex>& v) {
    fix(x,v);
    return v[x].min_sdom_node;
}

void put_min_sdom(int x,std::vector<vertex>& v) {
    if(v[x].min_sdom > v[x].sdom) {
        v[x].min_sdom = v[x].sdom;
        v[x].min_sdom_node = x;
    }
    fix(x,v);
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> f(N);
    std::vector<std::vector<int>> inv_fd(N);
    std::vector<vertex> v(N);
    for(int q = 0;q < M;q++) {
        int u,v;
        std::cin >> u >> v;
        --u,--v;
        f[u].push_back(v);
    }
    for(int q = 0;q < N;q++) {
        v[q].number = q;
    }
    int dfs_label = 0;
    dfs(0,v,f,inv_fd,dfs_label);
    std::sort(v.begin(),v.end(),[](const vertex& a,const vertex& b) {
        return a.dfs_label < b.dfs_label;
    });
    for(int q = 0;q < N;q++) {
        v[q].dsu_p = q;
    }
    for(int q = v.size() - 1;q>0;q--) {
        // now q = v[q].dfs_label
        if(v[q].dfs_label == MAXN) continue;
        for(int w = 0;w < inv_fd[q].size();w++) {
            if(inv_fd[q][w] < q)
                v[q].sdom = std::min(v[q].sdom,inv_fd[q][w]);
            else 
                v[q].sdom = std::min(v[q].sdom,get_min_sdom(inv_fd[q][w],v));
        }
        v[v[q].sdom].sdom_in.push_back(q);
        put_min_sdom(q,v);
        for(int w = 0;w < v[q].sdom_in.size();w++) {
            if(get_min_sdom(v[q].sdom_in[w],v) == q) v[v[q].sdom_in[w]].idom = q;
            else v[v[q].sdom_in[w]].idom = get_min_sdom_node(v[q].sdom_in[w],v);
        }
        merge(q,v[q].dfs_p,v);
    }
    for(int q = 1;q < v.size();q++) {
        if(v[q].dfs_label == MAXN) continue;
        if(v[q].sdom == 0) v[q].idom = 0;
        else if(v[q].idom != v[q].sdom) v[q].idom = v[v[q].idom].idom;
    }
    // change idom to init number base
    for(int q = 1;q < v.size();q++) {
        if(v[q].dfs_label == MAXN) continue;
        v[q].idom = v[v[q].idom].number;
    }
    std::sort(v.begin(),v.end(),[](const vertex& a,const vertex& b){
        return a.number < b.number;
    });
    std::vector<int> ans;
    int cur_vertex = N - 1;
    while(cur_vertex != 0) {
        ans.push_back(cur_vertex + 1);
        cur_vertex = v[cur_vertex].idom;
    }
    ans.push_back(1);
    std::sort(ans.begin(),ans.end());
    std::cout << ans.size() << std::endl;
    for(int q = 0;q < ans.size();q++) {
        std::cout << ans[q] << " ";
    }
}

int main() {
    solve();
}
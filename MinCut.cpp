// This code is tested by https://cses.fi/problemset/task/1709

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using ll = long long;

class edge{
public:
    int to;
    int cap;
    int inv;
};

void fix(edge& e,std::vector<std::vector<edge>>& f,int value) {
    e.cap -= value;
    f[e.to][e.inv].cap += value;
}

int dfs(int u,std::vector<std::vector<edge*>>& fp,std::vector<std::vector<edge>>& f,std::vector<bool>& check,int get_min = 2000000) {
    if(u == fp.size() - 1) {
        return get_min;
    }
    if(check[u]) return 0;
    check[u] = true;
    for(int q = 0;q < fp[u].size();q++) {
        int flow = dfs(fp[u][q]->to,fp,f,check,std::min(get_min,fp[u][q]->cap));
        if(flow) {
            fix(*fp[u][q],f,flow);
            return flow;
        }
    }
    return 0;
}

void bfs(std::vector<std::vector<edge*>>& fp,std::vector<int>& level,std::vector<std::vector<edge>>& f) {
    std::queue<int> bfs;
    bfs.push(0);
    level[0] = 0;
    while(bfs.size()) {
        int u = bfs.front();
        bfs.pop();
        for(int q = 0;q < f[u].size();q++){
            int v = f[u][q].to;
            if(f[u][q].cap && (level[v] == -1 || level[v] - 1 == level[u])) {
                fp[u].push_back(&f[u][q]);
                if(level[v] == -1) {
                    bfs.push(v);
                }
                level[v] = level[u] + 1;
            }
        }
    }
}

void max_flow(std::vector<std::vector<edge>>& f) {
    const int N = f.size();
    std::vector<bool> check(N);
    std::vector<std::vector<edge*>> fp(N);
    std::vector<int> level(N);
    while(true) {
        for(int q = 0;q < N;q++) {
            fp[q].clear();
            check[q] = false;
            level[q] = -1;
        }
        bfs(fp,level,f);
        if(level[N - 1] == -1) break;
        int flow = dfs(0,fp,f,check);
    }

    for(int q = 0;q < N;q++) {
        fp[q].clear();
        check[q] = false;
        level[q] = -1;
    }

    const int real_N = (N - 2) / 2;
    bfs(fp,level,f);
    std::vector<std::pair<int,int>> ans;
    for(int q = 1;q <= real_N;q++) {
        if(level[q] == -1) {
            ans.push_back({1,q});
        }
    }
    for(int q = real_N + 1;q <= 2 * real_N;q++) {
        if(level[q] != -1) {
            ans.push_back({2,q - real_N});
        }
    }
    std::cout << ans.size() << std::endl;
    for(int q = 0;q < ans.size();q++) {
        std::cout << ans[q].first << " " << ans[q].second << std::endl;
    }
}

void add_edge(std::vector<std::vector<edge>>& f,int u,int v, int cap) {
    edge e;
    e.to = v;
    e.cap = cap;
    e.inv = f[v].size();
    f[u].push_back(e);
    e.to = u;
    e.cap = 0;
    e.inv = f[u].size() - 1;
    f[v].push_back(e);
}

int main() {
    int N;
    std::cin >> N;
    std::vector<std::vector<edge>> f(2 * N + 2);
    for(int q = 0;q < N;q++) {
        std::string input;
        std::cin >> input;
        for(int w = 0;w < N;w++) {
            if(input[w] == 'o'){
                int u = q + 1;
                int v = w + N + 1;
                add_edge(f,u,v,2);
            }
        }
        add_edge(f,0,q + 1,1);
        add_edge(f,q + N + 1,2 * N + 1,1);
    }
    max_flow(f);
    
    return 0;
}
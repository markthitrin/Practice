// This code is tested by https://open.kattis.com/problems/allpairspath

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

const ll MAXLL = 1e18;
const ll MINLL = -1e18;

class edge{
public:
    ll value = MAXLL;
    bool bad = false;
};

void floyd(std::vector<std::vector<edge>>& d) {
    int N = d.size();
    for(int k = 0;k < N;k++) {
        for(int i = 0;i < N;i++) {
            for(int j = 0 ;j <N;j++) {
                if(d[i][k].value == MAXLL || d[k][j].value == MAXLL) continue;
                if(d[i][j].value > d[i][k].value + d[k][j].value) d[i][j].value = d[i][k].value + d[k][j].value;
            }
        }
    }
    for(int k = 0;k < N;k++){
        for(int i = 0;i < N;i++) {
            for(int j = 0;j < N;j++) {
                if(d[i][k].value == MAXLL || d[k][j].value == MAXLL) continue;
                if(d[i][j].value > d[i][k].value + d[k][j].value) d[i][j].bad = true;
            }
        }
    }
    for(int k = 0;k < N;k++){
        for(int i = 0;i < N;i++) {
            for(int j = 0;j < N;j++) {
                if(d[i][k].value < MAXLL && d[k][j].bad) d[i][j].bad = true;
                if(d[k][j].value < MAXLL && d[i][k].bad) d[i][j].bad = true;
            }
        }
    }
}

bool solve() {
    int N,M,Q;
    std::cin >> N >> M >> Q;
    std::vector<std::vector<edge>> d(N,std::vector<edge>(N));
    if(N == 0 && M == 0 && Q == 0) return false;
    for(int q = 0;q < N;q++) d[q][q].value = 0;
    for(int q = 0;q < M;q++) {
        int u,v,w;
        std::cin >> u >> v >> w;
        if(d[u][v].value > w)
            d[u][v].value = w;
    }
    floyd(d);
    for(int q = 0;q < Q;q++) {
        int u,v;
        std::cin >> u >> v;
        if(d[u][v].bad) std::cout << "-Infinity\n";
        else if(d[u][v].value == MAXLL) std::cout << "Impossible\n";
        else std::cout << d[u][v].value << "\n";
    }
    return true;
}

int main () {
    while(solve()) {

    }
    return 0;
}
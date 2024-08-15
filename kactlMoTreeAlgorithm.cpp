#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <numeric>
#include <math.h>
#include <array>

#define vi vector<int>
#define pii pair<int,int>
#define sz(v) v.size()
#define all(v) v.begin(),v.end()
#define rep(i,s,t) for(int i = s;i < t;i++)

using namespace std;
using ll = long long;

int sum_ss = 0;

void add(int ind, int end) {
    sum_ss += ind;
}//add a[ind] (end=0 or 1)

void del(int ind, int end) {
    sum_ss -= ind;
}// remove a[ind]

int calc() {
    return sum_ss;    
}// compute current answer

vi moTree(vector<array<int,2>> Q,vector<vi>& ed, int root=0){
    int N=sz(ed),pos[2]={},blk=std::sqrt(N);// N/sqrt(Q)
    vi s(sz(Q)),res=s,I(N),L(N),R(N),in(N),par(N);
    add(0,0),in[0]=1;
    auto dfs=[&](int x, int p, int dep,auto&f)->void {
        par[x]=p;
        L[x]=N;
        if (dep) I[x]=N++;
        for(int y:ed[x]) if (y!=p)f(y,x,!dep,f);
        if (!dep) I[x]=N++;
        R[x]= N;
    };
    dfs(root,-1,0,dfs);

#define K(x) pii(I[x[0]] / blk, I[x[1]] ^ -(I[x[0]] / blk & 1))

    iota(all(s), 0);
    sort(all(s), [&](int s, int t){return K(Q[s]) < K(Q[t]);});
    for(int qi:s)rep(end,0,2){
        int& a=pos[end],b=Q[qi][end],i=0;
        #define step(c){if (in[c]){del(a,end);in[a]=0;}\
        else{add(c,end);in[c]=1;}a =c;}
        while (!(L[b]<=L[a]&&R[a]<=R[b]))
        I[i++]=b,b=par[b];
        while (a!=b)step(par[a]);
        while (i--)step(I[i]);
        if (end)res[qi]=calc();
    }
    return res;
}

int main() {
    int N = 8;
    std::vector<vi> ed(N);
    ed[0].push_back(1);
    ed[1].push_back(2);
    ed[0].push_back(3);
    ed[2].push_back(4);
    ed[2].push_back(5);
    ed[5].push_back(6);
    ed[3].push_back(7);
    std::vector<std::array<int,2>> Q;
    Q.push_back({0,3});
    Q.push_back({2,3});
    Q.push_back({4,5});
    Q.push_back({3,7});
    Q.push_back({5,7});
    std::vector<int> res = moTree(Q,ed,0);
    for(int q = 0;q < res.size();q++) {
        std::cout << res[q] << " ";
    }
    return 0;
}
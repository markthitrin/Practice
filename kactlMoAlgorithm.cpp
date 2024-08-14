#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <numeric>

#define vi vector<int>
#define pii pair<int,int>
#define sz(v) v.size()
#define all(v) v.begin(),v.end()
#define rep(i,s,t) for(int i = s;i < t;i++)

using namespace std;
using ll = long long;

const std::string ss = "Hello world I have been gone for a long time, Hope you are happy and wealthy";
int sum_ss;

void add(int ind, int end) {
    sum_ss += ss[ind];
}//add a[ind] (end=0 or 1)

void del(int ind, int end) {
    sum_ss -= ss[ind];
}// remove a[ind]

int calc() {
    return sum_ss;    
}// compute current answer

vi mo(vector<pii> Q){
    int L=0,R=0,blk=350;// N/sqrt(Q)
    vi s(sz(Q)),res=s;
    #define K(x) pii(x.first/blk,x.second^-(x.first/blk&1))
    iota(all(s),0);
    sort(all(s),[&](int s, int t){return K(Q[s])<K(Q[t]);});
    for(int qi:s){
        pii q=Q[qi];
        while (L > q.first) add(--L,0);
        while (R < q.second) add(R++,1);
        while (L < q.first) del(L++,0);
        while (R > q.second) del(--R,1);
        res[qi]=calc();
    }
    return res;
}statu

int main() {
    std::vector<std::pair<int,int>> Q;
    for(int q = 0;q < 100;q++) {
        int u = rand() % ss.size();
        int v = rand() % ss.size();
        if(u > v) std::swap(u,v);
        Q.push_back({u,v + 1});
    }
    auto res = mo(Q);
    for(int q = 0;q < res.size();q++) {
        int ans_sum_ss = 0;
        for(int w = Q[q].first ;w < Q[q].second;w++) {
            ans_sum_ss += ss[w];
        }
        if(res[q] != ans_sum_ss) {
            std::cout << "false : " << q << std::endl;
        }
    }
    std::cout << "finish\n";
}
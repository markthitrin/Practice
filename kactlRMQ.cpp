#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

#define rep(i,s,t) for(int i = s;i < t;i++)
#define sz(v) v.size()

using namespace std;
using ll = long long;

template<class T>
struct RMQ{
    vector<vector<T>>jmp;
    RMQ(const vector<T>&V):jmp(1,V){
        for(int pw=1,k=1;pw * 2<=sz(V);pw *=2,++k){
            jmp.emplace_back(sz(V)-pw * 2+1);
            rep(j,0,sz(jmp[k]))
            jmp[k][j]=min(jmp[k-1][j],jmp[k-1][j+pw]);
        }
    }
    T query(int a, int b){
        assert(a<b);// or return inf if a==b
        int dep=31-__builtin_clz(b-a);
        return min(jmp[dep][a],jmp[dep][b-(1<<dep)]);
    }
};

int main() {
    std::vector<int> a = {2,3,5,1,4,5,6,3,5,8,6,9};
    RMQ rmq(a);

    std::cout << rmq.query(0,1) << std::endl;
    std::cout << rmq.query(2,5) << std::endl;
    std::cout << rmq.query(9,10) << std::endl;
    std::cout << rmq.query(0,4) << std::endl;

    return 0;
}
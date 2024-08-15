#include <iostream>
#include <vector>

#define rep(i,s,t) for(int i = s;i < t;i++)
#define sz(v) v.size()

using namespace std;
using ll = long long;

const int mod = 1000000007;


typedef vector<ll> Poly;
ll linearRec(Poly S,Poly tr,ll k){
    int n=sz(tr);
    auto combine=[&](Poly a,Poly b){
        Poly res(n * 2+1);
        rep(i,0,n+1)rep(j,0,n+1)
        res[i+j]=(res[i+j]+a[i] * b[j])%mod;
        for(int i=2 * n;i>n;--i)rep(j,0,n)
        res[i-1-j]=(res[i-1-j]+res[i] * tr[j])%mod;
        res.resize(n+1);
        return res;
    };
    Poly pol(n+1),e(pol);
    pol[0]=e[1]=1;
    for(++k;k;k/=2){
        if (k%2)pol=combine(pol,e);
        e =combine(e,e);
    }
    ll res=0;
    rep(i,0,n)res=(res+pol[i+1] * S[i])%mod;
    return res;
}

int main() {
    for(int q = -11;q < 10;q++) {
        std::cout << linearRec({0,1},{1,1},q) << " ";
    }
}
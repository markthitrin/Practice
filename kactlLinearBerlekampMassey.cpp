#include <iostream>
#include <vector>

#define sz(v) v.size()
#define rep(i,s,t) for(int i = s;i < t;i++)

using namespace std;
using ll = long long;

const int mod = 1000000007;

ll modpow(ll a,ll p) {
    ll ans = 1;
    for(int q = 0;(1ll << q) <= p;q++) {
        if((1ll << q) & p) {
            ans *= a;
            ans %= mod;
        }
        a *= a;
        a %= mod;
    }
    return ans;
}

vector<ll> berlekampMassey(vector<ll> s) {
    int n=sz(s),L=0,m=0;
    vector<ll>C(n),B(n),T;
    C[0]=B[0]=1;
    ll b=1;
    rep(i,0,n){
        ++m;
        ll d=s[i]%mod;
        rep(j,1,L+1)d=(d+C[j] * s[i-j])%mod;
        if (!d)continue;
        T=C;ll coef=d * modpow(b,mod-2)%mod;
        rep(j,m,n)C[j]=(C[j]-coef * B[j-m])%mod;
        if (2 * L>i) continue;
        L=i+1-L;B=T;b=d;m=0;
    }
    C.resize(L+1);C.erase(C.begin());
    for(ll&x:C)x=(mod-x)%mod;
    return C;
}

int main() {
    std::vector<ll> res = berlekampMassey({1,1,2,5,9,18});
    for(int q = 0;q <res.size();q++) {
        std::cout << res[q] << " ";
    }
    std::cout << std::endl;
    return 0;
}
#include <iostream>
#include <vector>
#include <iostream>
#include <algorithm>

#define sz(v) v.size()
#define rep(i,s,t) for(int i = s;i < t;i++)

using ll = long long;
using namespace std;

const ll root = 62;
const ll MOD1 = (479 << 21) + 1;
const ll MOD2 = (483 << 21) + 1;
const ll REALMOD = 1000000007;

typedef vector<ll> vl;
typedef vector<int> vi;

ll modpow(ll a,ll p,ll mod) {
    ll ans = 1;
    for(int q = 0;q < 63;q++) {
        if((1ll << q) & p) {
            ans *= a;
            ans %= mod;
        }
        a*= a;
        a %= mod;
    }
    return ans;
}

void ntt1(vl &a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vl rt(2, 1);
    for(static int k = 2, s = 2;k < n;k *= 2,s ++){
        rt.resize(n);
        ll z[] = {1, modpow(root, MOD1 >> s,MOD1)};
        rep(i,k,2*k) rt[i] = rt[i / 2] * z[i & 1] % MOD1;
    }
    vi rev(n);
    rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i,0,n) if(i < rev[i]) swap(a[i],a[rev[i]]);
    for(int k = 1;k <n;k *= 2)
        for(int i = 0;i <n;i += 2 * k) rep(j,0,k) {
            ll z = rt[j + k] * a[i + j + k] % MOD1, &ai = a[i + j];
            a[i + j +k] = ai-z + (z > ai ? MOD1 : 0);
            ai += (ai + z >= MOD1 ? z - MOD1 : z);
        }
}

void ntt2(vl &a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vl rt(2, 1);
    for(static int k = 2, s = 2;k < n;k *= 2,s ++){
        rt.resize(n);
        ll z[] = {1, modpow(root, MOD2 >> s,MOD2)};
        rep(i,k,2*k) rt[i] = rt[i / 2] * z[i & 1] % MOD2;
    }
    vi rev(n);
    rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i,0,n) if(i < rev[i]) swap(a[i],a[rev[i]]);
    for(int k = 1;k <n;k *= 2)
        for(int i = 0;i <n;i += 2 * k) rep(j,0,k) {
            ll z = rt[j + k] * a[i + j + k] % MOD2, &ai = a[i + j];
            a[i + j +k] = ai-z + (z > ai ? MOD2 : 0);
            ai += (ai + z >= MOD2 ? z - MOD2 : z);
        }
}

vl conv1(const vl &a,const vl &b) {
    if(a.empty() || b.empty()) return {};
    int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    int inv = modpow(n, MOD1 - 2, MOD1);
    vl L(a),R(b), out(n);
    L.resize(n), R.resize(n);
    ntt1(L),ntt1(R);
    rep(i,0,n) out[-i & (n - 1)] = (ll)L[i] * R[i] % MOD1 * inv % MOD1;
    ntt1(out);
    return {out.begin(), out.begin() + s};    
}

vl conv2(const vl &a,const vl &b) {
    if(a.empty() || b.empty()) return {};
    int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    int inv = modpow(n, MOD2 - 2, MOD2);
    vl L(a),R(b), out(n);
    L.resize(n), R.resize(n);
    ntt2(L),ntt2(R);
    rep(i,0,n) out[-i & (n - 1)] = (ll)L[i] * R[i] % MOD2 * inv % MOD2;
    ntt2(out);
    return {out.begin(), out.begin() + s};
}

ll mulmod(ll a,ll b, ll mod) {
    ll ans = 0;
    for(int q = 0;q < 63;q++) {
        if((1ll << q) & b) {
            ans += a;
            ans %= mod;
        }
        a += a;
        a %= mod;
    }
    return ans;
}

ll get_res(ll a0,ll a1,ll m0,ll m1) {
    ll L0 = (m1 * modpow(m1, m0 - 2, m0)) % (m0 * m1);
    ll L1 = (m0 * modpow(m0, m1 - 2, m1)) % (m0 * m1);

    return (mulmod(L0,a0,m0 * m1) + mulmod(L1,a1,m0 * m1)) % (m0 * m1) % REALMOD;
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    vl a(N),b(M);
    for(int q = 0;q < N;q++) {
        std::cin >> a[q];
    }
    for(int q = 0;q < M;q++) {
        std::cin >> b[q];
    }
    vl c1 = conv1(a,b);
    vl c2 = conv2(a,b);
    for(int q = 0;q < c1.size();q++) {
        std::cout << get_res(c1[q],c2[q],MOD1,MOD2) << " ";
    }
}

int main() {
    solve();
}


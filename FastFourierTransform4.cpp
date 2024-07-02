#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <cmath>

#define rep(i,s,t) for(int i = s;i < t;i++)
#define sz(v) v.size()
#define all(v) v.begin(),v.end()

using namespace std;
using ll = long long;

const ll MOD = 1000000007;
const ll sqrt_MOD = 31623;

typedef complex<double> C;
typedef vector<long long> vd;
typedef vector<int> vi;

void fft(vector<C>& a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2,1);
    static vector<C> rt(3,1);
    for (static int k = 2;k < n;k *= 2) {
        R.resize(n); rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        rep(i,k,2*k) rt[i] = R[i] = i&1 ? R[i/2] * x : R[i / 2];
    }
    vi rev(n);
    rep(i,00,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i,0,n) if(i < rev[i]) swap(a[i],a[rev[i]]);
    for(int k = 1;k < n;k *= 2)
        for(int i = 0;i < n;i+=2 * k) rep(j,0,k) {
            C z = rt[j+k] * a[i + j+k];
            a[i+j+k] = a[i+j] - z;
            a[i + j] += z;
        }
}
vd conv(const vd& a,const vd& b) {
    if(a.empty() || b.empty()) return {};
    vd res(sz(a) + sz(b) - 1);
    int L = 32 - __builtin_clz(sz(res)), n = 1 << L;
    vector<C> in(n), out(n);
    copy(all(a),in.begin());
    rep(i,0,sz(b)) in[i].imag(b[i]);
    fft(in);
    for(C& x : in) x*= x;
    rep(i,0,n) out[i] = in[-i & (n- 1)] - conj(in[i]);
    fft(out);
    rep(i,0,sz(res)) res[i] = round(out[i].imag() / (4 * n));
    return res;
}

vd plusv(const vd& a,const vd& b) {
    vd c(std::max(sz(a),sz(b)));
    for(int q = 0;q < a.size() || q < b.size();q++) {
        if(q < a.size() && q < b.size()) {
            c[q] = a[q] + b[q];
        }
        else if(q < a.size()) {
            c[q] = a[q];
        }
        else {
            c[q] = b[q];
        }
        c[q] = c[q] % MOD;
    }
    return c;
}

vd mul(const vd& a,ll value) {
    vd c;
    for(int q = 0;q < a.size();q++) {
        c.push_back(a[q] * value);
        c.back() = (ll)c.back() % MOD;
    }
    return c;
}

vd sqrt_mod_conv(const vd& a,const vd& b) {
    vd a0,a1,b0,b1;
    for(int q = 0;q < a.size();q++){
        a0.push_back(a[q] % sqrt_MOD);
        a1.push_back(a[q] / sqrt_MOD);
    }
    for(int q = 0;q < b.size();q++) {
        b0.push_back(b[q] % sqrt_MOD);
        b1.push_back(b[q] / sqrt_MOD);
    }
    vd c;
    c = plusv(c,conv(a0,b0));
    c = plusv(c,mul(conv(a0,b1),sqrt_MOD));
    c = plusv(c,mul(conv(a1,b0),sqrt_MOD));
    c = plusv(c,mul(conv(a1,b1),sqrt_MOD * sqrt_MOD % MOD));
    return c;
}

int get_upper_2power(int n) {
    return 1 << (32 - __builtin_clz(n));
}

void solve() {
    int N,M;
    std::cin >> N >> M;
    vd a(N),b(M);
    for(int q = 0;q < N;q++) {
        std::cin >> a[q];
    }
    for(int q = 0;q < M;q++) {
        std::cin >> b[q];
    }
    vd c = sqrt_mod_conv(a,b);
    for(int q = 0;q < c.size();q++) {
        std::cout << c[q] << " ";
    }
}

int main() {
    solve();
}
#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const ll MOD = 1e9 + 7;

std::vector<ll> fac;
std::vector<ll> inv_fac;

ll power(ll a,ll p) {
    ll res = 1;
    for(int q = 0;q <= 60;q++) {
        if((1ll << q) & p) {
            res *= a;
            res %= MOD;
        }
        a *= a;
        a %= MOD;
    }
    return res;
}

void generate_fac(int N) {
    fac.push_back(1);
    for(int q = 1;q < N;q++) {
        fac.push_back(fac[q - 1] * q);
        fac.back() %= MOD;
    }
}

void generate_inv_fac(int N) {
    for(int q = 0;q < N;q++) {
        inv_fac.push_back(power(fac[q],MOD - 2));
    }
}

ll C(ll n,ll r) {
    ll _ = fac[n] * inv_fac[r] % MOD;
    return _ * inv_fac[n - r] % MOD;
}

int main() {
    int N;
    std::cin >> N;
    generate_fac(N + 1);
    generate_inv_fac(N + 1);
    for(int q = 0;q <= N;q++) {
        std::cout << C(N,q) << " ";
    }
}
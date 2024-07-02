// This code is tested by https://judge.yosupo.jp/problem/convolution_mod

#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MOD = 998244353l;
const int C = 119;
const ll G = 62;
const ll K = 23;
ll inverse_2;

int get_number_upper_2power(int a) {
    return 32 - __builtin_clz(a);
} 

int get_upper_2power(int a) {
    return (1 << get_number_upper_2power(a));
}

ll power(ll a,ll p) {
    ll ans = 1;
    for(int q = 0;q < 61;q++) {
        if((1ll << q) & p) {
            ans *= a;
            ans %= MOD;
        }
        a *= a;
        a %= MOD;
    }
    return ans;
}

std::vector<ll> fft(const std::vector<ll>& a,bool inverse = false) {
    int N = a.size();
    int n = get_number_upper_2power(N) - 1;
    ll g = power(G,C);
    g = power(g,(1ll << (K - n)));
    std::vector<ll> W(N);
    W[0] = 1;
    for(int q = 1;q < N;q++) {
        W[q] = g * W[q - 1];
        W[q] %= MOD;
    }
    if(inverse) {
        for(int q = 1;q < N - q;q++) {
            std::swap(W[q],W[N - q]);
        }
    }
    std::vector<ll> b(N);
    for(int q = 0;q < N;q++) {
        int pos = 0;
        for(int w = 0;(1 << w) < N;w++){
            if((1 << w) & q) pos |= (N >> (1 + w));
        }
        b[pos] = a[q];
    }
    for(int q = 2;q <= N;q*=2) {
        ll wn = W[N / q];
        for(int i = 0;i < N;i += q) {
            ll w = 1;
            for(int j = 0;j < q / 2;j++) {
                ll f = b[i + j] + w * b[i + j + q / 2];
                ll s = b[i + j] - w * b[i + j + q / 2] % MOD + MOD ;
                f %= MOD;
                s %= MOD;
                b[i + j] = f;
                b[i + j + q / 2] = s;
                w *= wn;
                w %= MOD;
            }
        }
    }

    if(inverse) {
        ll intvert_N = power(N, MOD - 2);
        for(int q = 0;q < N;q++) {
            b[q] *= intvert_N;
            b[q] %= MOD;
        }
    }
    return b;
}

void push_til(std::vector<ll>& a,int wanted_size) {
    while(a.size() < wanted_size) {
       a.push_back(0);
    }
}

std::vector<ll> mul(std::vector<ll> a,std::vector<ll> b) {
    int N = a.size();
    auto fft_a = fft(a);
    auto fft_b = fft(b);
    std::vector<ll> fft_c(N);
    for(int q = 0;q < N;q++) {
        fft_c[q] = fft_a[q] * fft_b[q];
        fft_c[q] %= MOD;
    }
    return fft(fft_c,true);
}

int main() {
    inverse_2 = power(2,MOD - 2);
    
    int N,M;
    std::cin >> N >> M;
    std::vector<ll> a(N),b(M);
    for(int q = 0;q < N;q++) {
        std::cin >> a[q];
    }
    for(int q= 0 ;q < M;q++) {
        std::cin >> b[q];
    }
    int LL = std::max(get_upper_2power(2*N),get_upper_2power(2*M));
    push_til(a,LL);
    push_til(b,LL);
    std::vector<ll> c = mul(a,b);
    for(int q = 0;q < N + M - 1;q++) {
        std::cout << c[q] << " ";
    }
    return 0;
}
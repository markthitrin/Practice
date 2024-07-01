#include <iostream>
#include <vector>
#include <queue>
#include <complex>

#define PI 3.14159265358979323846
#define vcd std::vector<std::complex<double>>

using ll = long long;
using ull = unsigned ll;

vcd fft(const vcd& a,bool inverse = false) {
    const int N = a.size();
    vcd b(N);
    for(int q = 0;q < N;q++) {
        int pos = 0;
        for(int w = 0;(1 << w) < N;w++) {
            if((1 << w) & q) pos |= (N >> (w + 1));
        }
        b[pos] = a[q];
    }
    for(int q = 2;q <= N;q *= 2) {
        double angle = 2 * PI / q * (inverse ? -1 : 1);
        std::complex<double> wn(std::cos(angle),std::sin(angle));
        for(int i = 0;i < N;i += q) {
            std::complex<double> w = 1;
            for(int j = 0;j < q / 2;j++) {
                std::complex<double> f = b[i + j] + b[i + q / 2 + j] * w;
                std::complex<double> s = b[i + j] - b[i + q / 2 + j] * w;
                if(inverse) f/=2,s/=2;
                b[i + j] = f;
                b[i + q / 2 + j] = s;
                w *= wn;
            }
        }
    }
    return b;
}

vcd mul(const vcd& a,const vcd& b){
    const int N = a.size();
    vcd fft_a = fft(a,false);
    vcd fft_b = fft(b,false);
    vcd fft_c(N);
    for(int q = 0;q < N;q++) {
        fft_c[q] = fft_a[q] * fft_b[q];
    }
    return fft(fft_c,true);
}

int get_upper_2power(int a) {
    return (1 << (32 - __builtin_clz(a)));
}

void push_til(vcd& a,int wanted_size) {
    while(a.size() < wanted_size) a.push_back(0);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    int N = (ull)get_rand_value() % 500 + 500;
    vcd a(N);
    vcd b(N);
    vcd c;
    for(int q = 0;q < N;q++) {
        a[q] = get_rand_value() % 100;
        b[q] = get_rand_value() % 100;
    }
    ll LL = get_upper_2power(2 * N);
    push_til(a,LL);
    push_til(b,LL);
    vcd ans(LL);
    for(int q =0;q < LL;q++) {
        for(int w = 0;w < LL;w++) {
            if(q + w < LL) {
                ans[q + w] += a[q] * b[w];
            }
        }
    }
    c = mul(a,b);
    for(int q = 0;q < LL;q++) {
        if(std::abs(c[q].real() - ans[q].real()) >= 1) {
            return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q =1;q<=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <complex>
#include <numbers>

const double PI = 3.14159265358979323846;

using ll = long long;
using ull = unsigned ll;

int get_upper_2power(int a) {
    return (1 << (32 - __builtin_clz(a)));
}

void push_til(std::vector<std::complex<double>>& v,int wanted_size) {
    while(v.size() < wanted_size) {
        v.push_back(0);
    }
}

std::vector<std::complex<double>> fft(const std::vector<std::complex<double>>& v,bool inevrse) {
    int N = v.size();
    if(N == 1) {
        return v;
    }
    std::vector<std::complex<double>> even, odd;
    for(int q = 0;q < N;q++) {
        if(q % 2) {
            odd.push_back(v[q]);
        }
        else even.push_back(v[q]);
    }
    auto a = fft(even,inevrse);
    auto b = fft(odd,inevrse);
    double angle = 2 * PI / N * (inevrse ? -1 : 1);
    std::complex<double> wn(std::cos(angle),std::sin(angle));
    std::complex<double> w = 1;
    std::vector<std::complex<double>> answer(N);
    for(int q = 0;q < N / 2;q++) {
        answer[q] = a[q] + w * b[q];
        answer[q + N / 2] = a[q] - w * b[q];
        w *= wn;
    }
    if(inevrse) {
        for(int q = 0;q < answer.size();q++) {
            answer[q] /= 2;
        }
    }
    return answer;
}

std::vector<std::complex<double>> mul(const std::vector<std::complex<double>>& a,const std::vector<std::complex<double>>& b) {
    std::vector<std::complex<double>> fft_a = fft(a,false);
    std::vector<std::complex<double>> fft_b = fft(b,false);
    std::vector<std::complex<double>> fft_c;
    for(int q = 0;q < fft_a.size();q++) {
        fft_c.push_back(fft_a[q] * fft_b[q]);
    }
    return fft(fft_c,true);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    const int N = 1000 + (ull)get_rand_value() % 1000;
    const int LL = get_upper_2power(N) * 2;
    std::vector<std::complex<double>> a;
    std::vector<std::complex<double>> b;
    for(int q = 0;q < N;q++) {
        a.push_back(std::complex<double>(get_rand_value() % 70));
        b.push_back(std::complex<double>(get_rand_value() % 70));
    }
    push_til(a,LL);
    push_til(b,LL);
    std::vector<std::complex<double>> c = mul(a,b);
    std::vector<std::complex<double>> ans(LL,0);
    for(int w = 0;w < a.size();w++) {
        for(int e = 0 ;e < b.size();e++) {
            if(w + e < ans.size()) {
                ans[w + e] += a[w] * b[e];
            }
        }
    }
    for(int q = 0;q < LL;q++) {
        if((c[q].real() - ans[q].real()) >= 1) 
            return false;
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <= _;q++) {
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
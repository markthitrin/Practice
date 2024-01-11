#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const ll N = 10000000;

std::vector<ll> prime;
bool composite_check[N];

void generate_prime() {
    for(ll q = 2;q < N;q++) {
        if(!composite_check[q]) {
            prime.push_back(q);
            for(ll w = q * q; w < N;w += q) {
                composite_check[w] = true;
            }
        }
    }
}

int main() {
    generate_prime();
    int start, end;
    std::cin >> start >> end;
    for(int q = start ;q < end;q++) {
        std::cout << prime[q] << " ";
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const ll MOD = 1e9 + 7;

std::vector<std::vector<ll>> matrix_mul(const std::vector<std::vector<ll>>& a,const std::vector<std::vector<ll>>& b) {
    std::vector<std::vector<ll>> res(a.size(),std::vector<ll>(b[0].size(),0));
    for(int k = 0;k < b[0].size();k++) {
        for(int i = 0;i < a.size();i++) {
            for(int j = 0;j < a[0].size();j++) {
                res[i][k] += a[i][j] * b[j][k];
                res[i][k] %= MOD;
            }
        }
    }
    return res;
}

std::vector<std::vector<ll>> matrix_exponential(std::vector<std::vector<ll>> M, ll power) {
    std::vector<std::vector<ll>> res(M.size(),std::vector<ll>(M[0].size(),0));
    for(int q = 0;q < res.size();q++) {
        res[q][q] = 1;
    }

    for(int q = 0;q <= 60;q++) {
        if((1ll << q) & power) {
            res = matrix_mul(res,M);
        } 
        M = matrix_mul(M,M);
    }
    return res;
}

std::vector<std::vector<ll>> matrix_exponential_brute_force(const std::vector<std::vector<ll>>& M,ll power) {
    std::vector<std::vector<ll>> res(M.size(),std::vector<ll>(M[0].size(),0));
    for(int q = 0;q < res.size();q++) {
        res[q][q] = 1;
    }

    for(int q = 0; q < power;q++) {
        res = matrix_mul(res,M);
    } 
    return res;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    ll dimention = (ull)get_rand_value() % 100;
    std::vector<std::vector<ll>> M(dimention, std::vector<ll>(dimention,0));
    for(int q = 0;q < M.size();q++) {
        for(int w = 0;w < M[0].size();w++) {
            M[q][w] = (ull)get_rand_value() % 1000;
        }
    }   
    ll power = (ull)get_rand_value() % 100;
    std::vector<std::vector<ll>> res1 = matrix_exponential(M,power);
    std::vector<std::vector<ll>> res2 = matrix_exponential_brute_force(M,power);
    for(int q = 0;q <res1.size();q++){
        for(int w = 0;w < res1[0].size();w++) {
            if(res1[q][w] != res2[q][w]) 
                return false;
        }
    }
    return true;
}

int main() {
    int _= 0;
    std::cin >> _;
    for(int q =1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <numeric>

using ll = long long;

ll get_ans(std::vector<ll>& a,int i,int j) {
    ll sum = 0 ;
    for(int q = 0;q < a.size();q++){
        if(i != q && q != j)
            sum += a[q];
    }
    sum += std::gcd(a[i],a[j]);
    return sum;
}

int main() {
    std::vector<ll> a = {(ll)std::pow(3,6),(ll)std::pow(3,8)-2,(ll)std::pow(3,8)-1,(ll)std::pow(3,8)};
    for(int i = 0;i < a.size() - 1;i++) {
        for(int j = i + 1;j < a.size();j++) {
            std::cout << i << " " << j << " : " << get_ans(a,i,j)<<std::endl;
        }
    }
}
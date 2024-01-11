#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

int main() {
    while(true) {
        ll a,b;
        std::cin >> a >> b;
        std::cout << "gcd(" <<a << "," << b << ") = " << std::__gcd(a,b) << std::endl; 
    }
    return 0;
}

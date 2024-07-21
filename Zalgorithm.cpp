// This code is tested by https://cses.fi/problemset/task/1753

#include <iostream>
#include <vector>
#include <algorithm>

using ll = long long;

void solve() {
    std::string s1,s2;
    std::cin >> s2 >> s1;
    std::string s = s1 + "#" + s2;
    std::vector<int> z(s.size(),0);
    int R = 0;
    int L = 0;
    for(int q = 1;q < s.size();q++) {
        auto reconstruct = [&]() {
            while(q + z[q] < s.size()) {
                if(s[z[q]] == s[q + z[q]]) {
                    ++z[q];
                }
                else break;
            }
            L = q;
            R = q + z[q];
        };
        if(q >= R) {
            z[q] = 0;
            reconstruct();
        }
        else {
            z[q] = std::min(z[q - L],R - q);
            if(q + z[q] == R) reconstruct();
        }
    }
    int ans = 0;
    for(int q = 1;q < z.size();q++) {
        if(z[q] == s1.size()) ans++;
    }
    std::cout << ans << std::endl;
}

int main() {
    solve();
    return 0;
}
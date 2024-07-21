// This code is tested by https://cses.fi/problemset/task/1753

#include <iostream>
#include <string>
#include <vector>

using ll = long long;

void solve() {
    std::string s,t;
    std::cin >> t >> s;
    std::vector<int> kmp(s.size());
    for(int q = 1;q < s.size();q++) {
        int c = kmp[q - 1];
        while(c != 0) {
            if(s[q] != s[c]) {
                c = kmp[c - 1];
            }
            else break;
        }
        if(c == 0) {
            kmp[q] = (s[q] == s[0]) ? 1 : 0;
        }
        else {
            kmp[q] = c + 1;
        }
    }
    int ans = 0;
    int i = 0;
    for(int q = 0;q < t.size();q++) {
        while(i != 0) {
            if(s[i] != t[q]) {
                i = kmp[i - 1];
            }         
            else break;
        }
        if(i == 0) {
            i = (t[q] == s[0]) ? 1 : 0;
        }
        else {
            i++;
        }
        if(i == s.size()) {
            ans++;
            i = kmp[i - 1];
        }
    }
    std::cout << ans << std::endl;
}

int main() {
    solve();
}
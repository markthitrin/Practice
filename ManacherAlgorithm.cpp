// This code is tested by https://cses.fi/problemset/task/1111

#include <iostream>
#include <string>
#include <vector>

using ll = long long;

void solve() {
    std::string s,_s;
    std::cin >> _s;
    for(int q = 0;q < _s.size();q++) {
        s.push_back('#');
        s.push_back(_s[q]);
    }
    s.push_back('#');

    std::vector<int> p(s.size(),0);
    int C = 0;
    for(int q = 1;q < p.size();q++) {
        int R = C + p[C];
        int L = C - p[C];
        auto reconstruct = [&]() {
            while(q + p[q] < p.size() && q - p[q] >= 0) {
                if(s[q - p[q] - 1] == s[q + p[q] + 1]) {
                    p[q]++;
                }
                else break;
            }
            C = q;
        };
        if(q > R) {
            reconstruct();
        }
        else {
            int _q = C - (q - C);
            if(_q - p[_q] > L) {
                p[q] = p[_q];
            }
            else if(_q - p[_q] == L) {
                p[q] = p[_q];
                reconstruct();
            }
            else {
                p[q] = R - q;
            }
        }
    }
    int best = 0;
    int best_pos = 0;
    for(int q = 1;q < p.size();q++) {
        if(p[q] > best) {
            best = p[q];
            best_pos = q;
        }
    }
    std::string ans;
    if(s[best_pos] == '#') {
        for(int q = 1;q <= best;q+=2) { 
            ans.push_back(s[best_pos + q]);
        }
        for(int q = ans.size() - 1;q>=0;q--) std::cout << ans[q];
        for(int q = 0;q < ans.size();q++) std::cout << ans[q];
    }
    else {
        //ans = s[best_pos];
        for(int q = 2;q<=best;q+=2) {
            ans.push_back(s[best_pos + q]);
        }
        for(int q = ans.size() - 1;q>=0;q--) std::cout << ans[q];
        std::cout << s[best_pos];
        for(int q = 0;q < ans.size();q++) std::cout << ans[q];
    }
    return;
}

int main() {
    solve();
    return 0;
}
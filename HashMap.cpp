#include <iostream>
#include <bits/extc++.h>
#include <string>

using ll = long long;
using namespace __gnu_pbds;

int main() {
    gp_hash_table<std::string,int> m;
    while(true) {
        int mode = 0;
        std::cin >> mode;
        if(mode == 0) {
            std::string s;
            std::cin >> s;
            int value;
            std::cin >> value;
            m[s] = value;
        }
        else if(mode == 1) {
            std::string s;
            std::cin >> s;
            m.erase(s);
        }
        else if(mode == 2) {
            std::string s;
            std::cin >> s;
            std::cout << m[s] << std::endl;
        }
    }
}
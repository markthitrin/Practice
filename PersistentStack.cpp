#include <iostream>
#include <vector>
#include <stack>

using ll = long long;
using ull = unsigned ll;

class node {
public:
    int root_version;
    ll value;
};

std::vector<node*> st_ver;

void add(int root_version,ll value) {
    node* new_node = new node();
    new_node->root_version = root_version;
    new_node->value = value;
    st_ver.push_back(new_node);
}

void pop(int root_version) {
    st_ver.push_back(st_ver[st_ver[root_version]->root_version]);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    st_ver.clear();
    st_ver.push_back(new node());
    std::vector<std::stack<ll>> vers_ans;
    vers_ans.push_back(std::stack<ll>());
    auto do_add = [&](int root_version, ll value) {
        vers_ans.push_back(vers_ans[root_version]);
        vers_ans.back().push(value);

        add(root_version,value);
    };
    auto do_pop = [&](int root_version) {
        vers_ans.push_back(vers_ans[root_version]);
        vers_ans.back().pop();

        pop(root_version);
    };
    auto do_check = [&](int version) {
        return vers_ans[version].top() == st_ver[version]->value;
    };
    for(int q = 0;q < 20000;q++) {
        int mode = rand() % 3;
        if(mode == 1) {
            int root_version = (ull)get_rand_value() % vers_ans.size();
            ll value = (ull)get_rand_value();
            do_add(root_version, value);
        }
        else if(mode == 2) {
            int version = (ull)get_rand_value() % vers_ans.size();
            if(vers_ans[version].size() > 0) {
                do_pop(version);
            }
        }
        else {
            int version = (ull)get_rand_value() % vers_ans.size();
            if(vers_ans[version].size() > 0) {
                if(!do_check(version)) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "fail\n";
        }
    }
}


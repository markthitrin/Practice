#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using ll = long long;
using ull = unsigned ll;

class slope_node{
public:
    int pos;
}put_in;

struct slope_node_compare_less{
    bool operator()(const slope_node& a,const slope_node& b) const {
        return a.pos < b.pos;
    }
};

struct slope_node_compare_greater{
    bool operator()(const slope_node& a,const slope_node& b) const {
        return a.pos > b.pos;
    }
};

void add_slope(std::priority_queue<slope_node,std::vector<slope_node>,slope_node_compare_less>& left_slope
            ,std::priority_queue<slope_node,std::vector<slope_node>,slope_node_compare_greater>& right_slope
            ,int pos) {
    put_in.pos = pos;
    if(!left_slope.empty() && pos < left_slope.top().pos) {
        left_slope.push(put_in);
        right_slope.push(left_slope.top());
        left_slope.pop();
    }
    else right_slope.push(put_in);
    while(left_slope.size() < right_slope.size()) {
        left_slope.push(right_slope.top());
        right_slope.pop();
    }
}

int get_min_pos(std::priority_queue<slope_node,std::vector<slope_node>,slope_node_compare_less>& left_slope
            ,std::priority_queue<slope_node,std::vector<slope_node>,slope_node_compare_greater>& right_slope) {
    return left_slope.top().pos;
}

ll get_rand_value() {
    #define UR (ll)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (4);
}

bool run_test() {
    int vector_size = (ull)std::rand() % 1000;
    int max_range = 1000;
    std::vector<slope_node> abs_function;
    std::priority_queue<slope_node,std::vector<slope_node>,slope_node_compare_less> left_slope;
    std::priority_queue<slope_node,std::vector<slope_node>,slope_node_compare_greater> right_slope;
    auto do_add_slope = [&](int pos) {
        put_in.pos = pos;
        add_slope(left_slope,right_slope,pos);
        abs_function.push_back(put_in);
    };
    auto do_get_min_value = [&]() {
        if(abs_function.size() == 0) return true;
        int ans = 2000000000;
        for(int q = -max_range;q <= max_range;q++) {
            int get_value = 0;
            for(int w = 0;w < abs_function.size();w++) {
                get_value += std::abs(q - abs_function[w].pos);
            }
            ans = std::min(ans,get_value);
        }

        int get_pos = get_min_pos(left_slope,right_slope);
        int get_ans_value = 0;
        for(int q = 0;q < abs_function.size();q++) {
            get_ans_value += std::abs(get_pos - abs_function[q].pos);
        }
        if(ans != get_ans_value) {
            std::cout << "sfds";
        }
        return ans == get_ans_value;
    };
    for (int q = 0;q < 1000;q++) {
        int mode = std::rand() % 2;
        if(mode == 0) {
            int pos = (ll)get_rand_value() % max_range;
            do_add_slope(pos);
        }
        else {
            if(!do_get_min_value()) return false;
        }
    }
    return true;
}

int main() {
    int _;
    std::cin >> _;
    for(int q =1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        } 
        else {
            std::cout << "failed\n";
        }
    }
}
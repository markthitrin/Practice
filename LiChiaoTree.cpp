#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const double MAXDOUBLE = 1e300;
const double MINDOUBLE = -1e150;
const double MINRANGE = -1e9;
const double MAXRANGE = 1e9;
const double EPS = 1e-3;

class line{
public:
    double m = 0;
    double b = MINDOUBLE;
    line() {}
    line(double _m,double _b) {
        m = _m;
        b = _b;
    }
    double operator()(const double& a) const {
        return a * m + b;
    }
};

const line LOWEST(0,MINDOUBLE);

class node {
public:
    line f;
    node* left = nullptr;
    node* right = nullptr;
};

double get_mid(double left,double right) {
    return (right + left) / 2;
}

int get_cross(double mid,line& root_line,line& new_line) {
    if(new_line(mid) > root_line(mid)) {
        std::swap(root_line, new_line);
    }
    if (root_line.m == new_line.m) return 0;
    else if(new_line.m > root_line.m) return 1;
    else return -1;
}

void add(node* root,line l,double left = MINRANGE,double right = MAXRANGE) {
    double mid = get_mid(left, right);
    int pos = get_cross(mid, root->f,l);
    if(std::abs(right - left) <= EPS) return;
    if(pos == 0) return;
    else if(pos == 1) {
        if(root->right == nullptr) root->right = new node();
        add(root->right,l,mid,right);
    }
    else {
        if(root->left == nullptr) root->left = new node();
        add(root->left,l,left,mid);
    }
}

double get_max(node* root,double x,double left = MINRANGE, double right = MAXRANGE) {
    if(root == nullptr) return LOWEST(x);
    if(std::abs(right - left) <= EPS) return root->f(x);
    double mid = get_mid(left,right);
    double cur_value = root->f(x);
    if(x == mid) return cur_value;
    else if(x < mid) return std::max(cur_value,get_max(root->left,x,left,mid));
    else return std::max(cur_value,get_max(root->right,x,mid,right));
}

double get_rand_double(const ll max_value) {
    #define UR (ull)std::rand()
    return double((UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15)) % (max_value << 10)) / (1ll << 10);
}

std::pair<double,double> get_rand_range(double min, double max) {
    std::pair<double,double> res(get_rand_double(ll(max - min + 1)) + min, get_rand_double(ll(max - min + 1)) + min);
    if(res.first > res.second) std::swap(res.first,res.second);
    return res;
}

bool run_test() {
    node* root = new node();
    std::vector<line> ls;
    auto do_add = [&](line a) {
        ls.push_back(a);
        add(root,a); 
    };
    auto do_get_max = [&](double x) {
        double ans = MINDOUBLE;
        for(int q = 0;q < ls.size();q++) {
            ans = std::max(ans,ls[q](x));
        }
        double try_ans = get_max(root,x);
        return try_ans == ans;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            double m = get_rand_double(100) - 50;
            double b = get_rand_double(10000) - 5000;
            line new_line(m,b);
            do_add(new_line);
        }
        else {
            double x = get_rand_double(1000000) - 1000000 / 2;
            if(!do_get_max(x)) {
                return false;
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
        if(run_test()){
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
    return 0;
}
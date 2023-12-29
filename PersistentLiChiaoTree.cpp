#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const double MINDOUBLE = -1e150;
const double MAXDOUBLE = 1e300;
const double MINRANGE = -1e9;
const double MAXRANGE = 1e9;
const double EPS = 1e-3;

class line {
public:
    double m = 0;
    double b = MINDOUBLE;
    double operator()(const double& x) const {
        return m * x + b;
    }
};

class node {
public:
    line f;
    node* left = nullptr;
    node* right = nullptr;
};

double get_mid(double left,double right) {
    return (right + left) / 2;
}

node* lc(node* head) {
    if(head == nullptr) return nullptr;
    return head->left;
}

node* rc(node* head) {
    if(head == nullptr) return nullptr;
    return head->right;
}

node* mirror(node* head) {
    node* new_node = new node();
    if(head == nullptr) return new_node;
    new_node->f = head->f;
    new_node->left = head->left;
    new_node->right = head->right;
    return new_node;
}

ll fix(line& root_line,line& new_line,double mid) {
    if(root_line(mid) < new_line(mid)) {
        std::swap(root_line,new_line);
    }
    if(new_line.m == root_line.m) return 0;
    if(new_line.m < root_line.m) return -1;
    else return 1;
}

void add(node* head, node* root, line new_line,double left = MINRANGE, double right = MAXRANGE) {
    if(std::abs(right - left) < EPS) {
        fix(root->f,new_line,left);
        return;
    }
    double mid = get_mid(left,right);
    ll pp = fix(root->f,new_line,mid);
    if(pp == 0) return;
    if(pp == -1) {
        root->left = mirror(lc(head));
        add(lc(head),root->left,new_line,left,mid);
    }
    else {
        root->right = mirror(rc(head));
        add(rc(head),root->right,new_line,mid,right);
    }
}

double get_max(node* root,double x,double left = MINRANGE, double right = MAXRANGE) {
    if(root == nullptr) return MINDOUBLE;
    double mid = get_mid(left,right);
    if(x == mid) return root->f(x);
    if(x < mid) return std::max(get_max(root->left,x,left,mid),root->f(x));
    else return std::max(get_max(root->right,x,mid,right),root->f(x));
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

double get_rand_double(ll min, ll max) {
    max *= 10000;
    min *= 10000;
    return ((ull)get_rand_value() % (max - min + 1) + min) / 10000;
}

bool run_test() {
    std::vector<std::vector<line>> ans_vers;
    ans_vers.push_back(std::vector<line>());

    std::vector<node*> tree_vers;
    tree_vers.push_back(new node());

    auto do_add = [&](ll root_version,line new_line) {
        ans_vers.push_back(std::vector<line>(ans_vers[root_version]));
        ans_vers.back().push_back(new_line);

        tree_vers.push_back(mirror(tree_vers[root_version]));
        add(tree_vers[root_version],tree_vers.back(),new_line);
    };
    auto do_get_max = [&](ll root_version,double x) {
        double get_ans = MINDOUBLE;
        for(int q = 0;q < ans_vers[root_version].size();q++) {
            get_ans = std::max(get_ans,ans_vers[root_version][q](x));
        }

        double get_max_tree = get_max(tree_vers[root_version],x);
        return get_ans == get_max_tree;
    };
    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 2;
        if(mode) {
            line new_line;
            new_line.m = get_rand_double(-1000,1000);
            new_line.b = get_rand_double(-100000,100000);
            ll root_version = (ull)get_rand_value() % ans_vers.size();
            do_add(root_version,new_line);
        }
        else {
            ll root_version = (ull)get_rand_value() % ans_vers.size();
            double x = get_rand_double(-1000000,1000000);
            if(!do_get_max(root_version,x)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int _  = 0;
    std::cin >> _;
    for(int q = 1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
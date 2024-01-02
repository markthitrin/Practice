// max value of mx + b - d while d is the distance to the node
#include <iostream>
#include <vector>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const double MINDOUBLE = -1e100;
const double MAXDOUBLE = 1e300;
const double MINRANGE = -1e6;
const double MAXRANGE = 1e6;
const double EPS = 1e-10;
const double ANSERROR = 1e-6;

class line{
public:
    double m;
    double b = MINDOUBLE;
    double operator()(const double& x) const {
        return m * x + b;
    }
};

class node {
public:
    line l;
    node* left = nullptr;
    node* right = nullptr;
};

class vertex_ref {
public:
    ll node_numnber;
    double distance;
};

class vertex {
public:
    node* lc_tree = new node();
    std::vector<vertex_ref> cen_parent;
    std::vector<line> l;
};

class edge{
public:
    ll to;
    ll sub_size = 0;
    ll inv;
    double distance;
};

std::vector<vertex> v;
std::vector<std::vector<edge>> f;
std::vector<bool> checked;

ll get_cross(line line1,line line2, double mid) {
    if(line1(mid) < line2(mid)) std::swap(line1,line2);
    if(line1.m == line2.m) return 0;
    else if(line2.m > line1.m) return 1;
    else return -1;
}

void fix(line& node_line,line& new_line, double mid) {
    if(new_line(mid) > node_line(mid)) 
        std::swap(new_line,node_line);
}

double get_mid(double left,double right) {
    return (left + right) / 2;
}

void add(node* root,line new_line,double left = MINRANGE,double right = MAXRANGE) {
    if(std::abs(right - left) < EPS) {
        fix(root->l,new_line,left);
        return;
    }
    if(root->left == nullptr) root->left = new node();
    if(root->right == nullptr) root->right = new node();
    double mid = get_mid(left,right);
    fix(root->l,new_line,mid);
    ll cross_pos = get_cross(root->l,new_line,mid);
    if(cross_pos == 0) return;
    if(cross_pos == -1) add(root->left,new_line,left,mid);
    if(cross_pos == 1) add(root->right,new_line,mid,right);
}   

double get_max(node* root,double x,double left = MININT, double right = MAXINT) {
    if(root == nullptr) return MINDOUBLE;
    if(std::abs(right - left) < EPS) {
        return root->l(x);
    }
    double mid = get_mid(left,right);
    double ans = root->l(x);
    return std::max(ans,std::max(get_max(root->left,x,left,mid),get_max(root->right,x,mid,right)));
}

void recur1(ll u,ll p,ll index) {
    f[u][index].sub_size = 1;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur1(f[u][q].to,u,f[u][q].inv);
            f[u][index].sub_size += f[f[u][q].to][f[u][q].inv].sub_size;
        }
    }
    f[p][f[u][index].inv].sub_size = v.size() - f[u][index].sub_size;
}

void build_size() {
    for(int q = 0;q < f[0].size();q++) {
        recur1(f[0][q].to,0,f[0][q].inv);
    }
}

void recur3(ll u,ll p,ll index,const ll& sub_size,const ll& cen,double distance) {
    f[p][f[u][index].inv].sub_size = sub_size - f[u][index].sub_size;
    if(checked[u]) return;
    v[u].cen_parent.push_back({cen,distance});
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur3(f[u][q].to,u,f[u][q].inv,sub_size,cen,distance + f[u][q].distance);
        }
    }
}

void recur2(ll u) {
    if(checked[u]) return;
    ll max_sub_tree = 0;
    ll max_sub_size = MININT;
    ll total_size = 1;
    for(int q = 0;q < f[u].size();q++) {
        edge& select = f[f[u][q].to][f[u][q].inv];
        if(select.sub_size > max_sub_size) {
            max_sub_size = select.sub_size;
            max_sub_tree = q;
        }
        total_size += select.sub_size;
    }
    if(max_sub_size * 2 > total_size) {
        recur2(f[u][max_sub_tree].to);
        return;
    }
    checked[u] = true;
    for(int q = 0;q < f[u].size();q++) {
        recur3(f[u][q].to,u,f[u][q].inv,f[f[u][q].to][f[u][q].inv].sub_size,u,f[u][q].distance);
    }
    for(int q = 0;q < f[u].size();q++) {
        recur2(f[u][q].to);
    }
}

void build_cen() {
    recur2(0);
}

void add_line(ll u,line l) {
    v[u].l.push_back(l);
    add(v[u].lc_tree,l);
    for(int q = 0;q < v[u].cen_parent.size();q++) {
        line new_l = l;
        new_l.b -= v[u].cen_parent[q].distance;
        add(v[v[u].cen_parent[q].node_numnber].lc_tree,new_l);
    }
}

double get_max_line(ll u,double x) {
    double ans = MINDOUBLE;
    ans = std::max(ans, get_max(v[u].lc_tree,x));
    for(int q = 0;q < v[u].cen_parent.size();q++) {
        ans = std::max(ans, get_max(v[v[u].cen_parent[q].node_numnber].lc_tree, x) - v[u].cen_parent[q].distance);
    }
    return ans;
}

double recur4(ll u,ll p,double x,double distance) {
    double ans = MINDOUBLE;
    for(int q = 0;q < v[u].l.size();q++) {
        if(v[u].l[q](x) - distance > ans) {
            ans = v[u].l[q](x) - distance;
        }
        ans = std::max(ans, v[u].l[q](x) - distance);
    }
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            ans = std::max(ans, recur4(f[u][q].to, u, x, distance + f[u][q].distance));
        }
    }
    return ans;
}

double get_max_line_brute_force(ll u, double x) {
    return recur4(u,-1,x,0);
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

double get_rand_double(ll min,ll max) {
    max *= 1000;
    min *= 1000;
    return double((ull)get_rand_value() % (max - min + 1) + min) / 1000; 
}

bool run_test() {
    v.clear();
    f.clear();
    checked.clear();

    ll vertex_number = (ull)get_rand_value() % (10000) + 50000;

    for(int q = 0;q < vertex_number;q++) v.push_back(vertex());
    for(int q = 0;q < vertex_number;q++) checked.push_back(false);
    for(int q = 0;q < vertex_number;q++) f.push_back(std::vector<edge>());
    for(int q = 1;q < vertex_number;q++) {
        ll parent = (ull)get_rand_value() % (q);
        double distance = get_rand_double(0,100000);
        f[parent].push_back(edge());
        f[q].push_back(edge());
        f[parent].back().to = q;
        f[q].back().to = parent;
        f[parent].back().inv = f[q].size() - 1;
        f[q].back().inv = f[parent].size() - 1;
        f[parent].back().distance = distance;
        f[q].back().distance = distance;
    }

    build_size();
    build_cen();

    for(int q = 0;q < 100;q++) {
        int mode = std::rand() % 2;
        if(q < 1) {
            ll vertex = (ull)get_rand_value() % (vertex_number);
            line new_line;
            new_line.m = get_rand_double(-10,10);
            new_line.b = get_rand_double(-10000,10000);
            add_line(vertex,new_line); 
        }
        else {
            ll vertex = (ull)get_rand_value() % (vertex_number);
            double x = get_rand_double(-1000000,1000000);
            double get_ans = get_max_line_brute_force(vertex,x);
            double get_max_ans = get_max_line(vertex,x);
            if(std::abs(get_ans - get_max_ans) > ANSERROR) {
                std::cout << get_ans << " " << get_max_ans << " " << q << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main() {
    int _ = 0;
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
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using ll = long long ;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class point{
public:
    double x;
    double y;
    point() {}
    point(const double& _x,const double& _y) : x(_x) , y(_y) {}
    point operator-(const point& other) const{
        point res;
        res.x = x - other.x;
        res.y = y - other.y;
        return res;
    }
    point operator+(const point& other) const{
        point res;
        res.x = x + other.x;
        res.y = y + other.y;
        return res;
    }
    void print() {
        std::cout << "(" << x << "," << y << ")\n";
    }
};

ll orientate(point a,point b,point c) {
    point ab = b - a;
    point bc = c - b;
    if(ab.x * bc.y == ab.y * bc.x) return 0;
    else if(ab.x * bc.y > ab.y * bc.x) return 1;
    return -1;
}

std::vector<point> get_convex_hull(std::vector<point> all_points) {
    std::vector<point> res;
    std::vector<bool> check(all_points.size(),false);
    std::vector<ll> pushed_point;
    std::sort(all_points.begin(),all_points.end(),[](point a,point b){
        if(a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    for(int q = 0;q < all_points.size();q++) {
        while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),all_points[q]) == 1) {
            res.pop_back();
            check[pushed_point.back()] = false;
            pushed_point.pop_back();
        }
        res.push_back(all_points[q]);
        pushed_point.push_back(q);
        check[pushed_point.back()] = true;
    }
    for(int q = all_points.size() - 1;q>=0;q--) {
        if(check[q]) continue;
        while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),all_points[q]) == 1) {
            res.pop_back();
            check[pushed_point.back()] = false;
            pushed_point.pop_back();
        }
        res.push_back(all_points[q]);
        pushed_point.push_back(q);
        check[pushed_point.back()] = true;
    }
    while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),res[0]) == 1) {
        res.pop_back();
    }
    res.push_back(res[0]);
    return res;
}

double get_distance(point a,point b) {
    return std::sqrt(std::pow(a.x - b.x,2) + std::pow(a.y - b.y,2));
}

double get_farest_point(const std::vector<point>& hull) {
    double res = 0;
    int j = 0;
    for(int q = 0;q < hull.size();q++){
        while(get_distance(hull[q],hull[(j + 1) % hull.size()]) >= get_distance(hull[q],hull[(j) % hull.size()])) {
            ++j;
        }
        res = std::max(res,get_distance(hull[q],hull[j]));
    }
    return res;
}

double get_farest_point_brute_force(const std::vector<point>& all_points) {
    double res = 0;
    for(int q = 0;q < all_points.size();q++) {
        for(int w = q + 1;w < all_points.size();w++) {
            res = std::max(res,get_distance(all_points[q],all_points[w]));
        }
    }
    return res;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15); 
}

double get_rand_double(ll min,ll max) {
    max *= 1000;
    min *= 1000;
    return double((ull)get_rand_value() % (max - min + 1000) + min) / 1000;
}

point get_rand_point(ll radius) {
    point new_point;
    double r = (ull)get_rand_double(0,radius * 5);
    r = radius * ((double)10 - std::sqrt(double(100) - (20 * r / radius))) / 10;
    double angle = get_rand_double(0,2 * M_PI);
    new_point.y = r * std::sin(angle);
    new_point.x = r * std::cos(angle);
    return new_point;
}

bool run_test() {
    ll point_number = (ull)get_rand_value() % 1000 + 1000;
    std::vector<point> all_points(point_number,point());
    for(int q = 0;q < all_points.size();q++) {
        all_points[q] = get_rand_point(1000);
    }
    std::vector<point> hull = get_convex_hull(all_points);
    double ans = get_farest_point_brute_force(all_points);
    double get_ans = get_farest_point(hull);
    return get_ans == ans;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++){
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
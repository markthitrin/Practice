#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const double MINDOUBLE = -1e100;
const double MAXDOUBLE = 1e300;

class point{
public:
    double x;
    double y;
    point() {}
    point(const double& _x,const double& _y) : x(_x) , y(_y) {}
    point operator+(const point& other) const{
        point res;
        res.x = x + other.x;
        res.y = y + other.y;
        return res;
    }
    point operator-(const point& other) const{
        point res;
        res.x = x - other.x;
        res.y = y - other.y;
        return res;
    }
    bool operator==(const point& other)const{
        return x == other.x && y == other.y;
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

std::vector<point> get_convex_hull(std::vector<point> points) {
    std::sort(points.begin(),points.end(),[](point a,point b) {
        if(a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    std::vector<bool> check(points.size(),false);
    std::vector<point> res;
    std::vector<ll> point_number;
    for(int q = 0;q < points.size();q++){
        while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),points[q]) == 1) {
            check[point_number.back()] = false;
            point_number.pop_back();
            res.pop_back();
        }
        res.push_back(points[q]);
        point_number.push_back(q);
        check[point_number.back()] = true;
    }
    for(int q = points.size() -1;q>=0;q--) {
        if(check[q]) continue; 
        while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),points[q]) == 1) {
            res.pop_back();
        }
        res.push_back(points[q]);
    }
    while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),res[0]) == 1) {
        res.pop_back();
    }
    res.push_back(res[0]);
    return res;
}

bool check_all_in(std::vector<point>& hull,std::vector<point>& all_points) {
    for(int q = 0;q < all_points.size();q++) {
        for(int w = 0;w < hull.size() - 1;w++) {
            if(all_points[q] == hull[w] || all_points[q] == hull[w]) continue;
            if(orientate(hull[w],all_points[q],hull[w + 1]) == -1) 
                return false;
        }
    }
    return true;
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
    ll point_number = (ull)get_rand_value() % 10000 + 10000;
    std::vector<point> all_points(point_number);
    for(int q = 0;q < all_points.size();q++) {
        all_points[q] = get_rand_point(100);
    } 
    std::vector<point> hull = get_convex_hull(all_points);
    return check_all_in(hull,all_points);
}

int main() {
    int  _ = 0;
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
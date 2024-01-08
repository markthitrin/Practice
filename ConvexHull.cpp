#include <iostream>
#include <vector>
#include <algorithm>
#include <Math.h>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;
const double MINDOUBLE = -1e100;
const double MAXDOUBLE = 1e300;
const double MINRANGE = -1e6;
const double MAXRANGE = 1e6;

class point{
public:
    double x;
    double y;
    point() {};
    point(const double& _x,const double& _y) : x(_x), y(_y) {}
    point operator-(const point& other) const {
        point res;
        res.x = x - other.x;
        res.y = y - other.y;
        return res;
    }
    point operator+(const point& other) const {
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
    if(ab.x * bc.y > ab.y * bc.x) return 1;
    return -1;
}

std::vector<point> convex_hull(const std::vector<point>& v) {
    point min_y(MAXDOUBLE,MAXDOUBLE);
    ll index = 0;
    std::vector<point> f;
    std::vector<point> res;
    for(int q = 0;q < v.size();q++) {
        if(v[q].y < min_y.y) {
            min_y = v[q];
            index = q;
        }
    }
    for(int q = 0;q < v.size();q++) {
        if(q != index) {
            f.push_back(v[q]);
        }
    }

    auto angle_compare = [&](point a,point b) {
        return orientate({0,0},a - min_y,b - min_y) > 0;
    };
    std::sort(f.begin(),f.end(),angle_compare);

    res.push_back(min_y);
    for(int q = 0;q < f.size();q++) {
        while(res.size() >= 2 && orientate(res[res.size() - 2],res[res.size() - 1],f[q]) == -1) {
            res.pop_back();
        }
        res.push_back(f[q]);
    }
    while(res.size() >= 3 && orientate(res[res.size() - 2],res[res.size() - 1],min_y) == -1) {
        res.pop_back();
    }
    return res;
}

bool check(std::vector<point>& convexhull, point a) {
    for(int q = 0;q < convexhull.size();q++) {
        if(orientate(convexhull[q],convexhull[(q + 1) % convexhull.size()],a) == -1) {
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
    ll point_number = (ull)get_rand_value() % (25) + 25;
    std::vector<point> points;
    std::vector<point> hull;

    for(int q = 0;q < point_number;q++) {
        point new_point;
        new_point = get_rand_point(500);
        points.push_back(new_point);
    }
    hull = convex_hull(points);
    for(int q = 0;q <100;q++) {
        point check_point = points[std::rand() % points.size()];
        if(!check(hull,check_point)) {
            return false;
        }
    }
    return true;
}

int main () {
    int _ = 0;
    std::cin >> _;
    for(int q =1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
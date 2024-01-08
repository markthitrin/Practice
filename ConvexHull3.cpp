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
    point(double _x,double _y) : x(_x) , y(_y) {}
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
    if(ab.x * bc.y == ab.y * bc. x) return 0;
    if(ab.x * bc.y > ab.y * bc.x) return 1;
    return -1;
}

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

std::vector<point> recur1(const std::vector<point>& points,ll left,ll right) {
    if(right == left) return std::vector<point>(1,points[left]);
    ll mid = get_mid(left,right);
    std::vector<point> left_convex = recur1(points,left,mid);
    std::vector<point> right_convex = recur1(points,mid + 1,right);
    std::vector<point> res;
    ll left_end = left_convex.size() - 1, right_start = 0;
    while(true) {
        if(right_start < right_convex.size() - 1 && 
            orientate(left_convex[left_end],right_convex[right_start],right_convex[right_start + 1]) == 1) {
                right_start = (right_start + 1) % right_convex.size();
            }
        else if(left_end >= 1 && 
            orientate(left_convex[left_end - 1],left_convex[left_end],right_convex[right_start]) == 1) {
                left_end = (left_end - 1);
            }
        else break;
    }
    ll left_start = 0,right_end = 0;
    while(true) {
        if(left_start < left_convex.size() - 1 &&
            orientate(right_convex[right_end],left_convex[left_start],left_convex[left_start + 1]) == 1) {
                left_start = (left_start + 1) % left_convex.size();
            }
    }
}

std::vector<point> get_convex_hull(const std::vector<point>& points) {
    std::vector<point> f(points.begin(),points.end());
    std::sort(f.begin(),f.end(),[](point a,point b) {
        return a.x < b.x;
    });
    return recur1(points,0,points.size() - 1);
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
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
    point(double _x, double _y) : x(_x) , y(_y) {}
    point operator+(const point& other) const {
        point res;
        res.x = x + other.x;
        res.y = y + other.y;
        return res;
    }
    point operator-(const point& other) const {
        point res;
        res.x = x - other.x;
        res.y = y - other.y;
        return res;
    }
    void print() const {
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

point min_y1;
struct compare1{
    bool operator()(const point& a,const point& b) const {
        if(a.x == min_y1.x && a.y == min_y1.y) return true;
        if(b.x == min_y1.x && b.y == min_y1.y) return false;
        return orientate(min_y1,a,b) == -1;
    }
};

std::set<point,compare1> get_convex_hull(const std::vector<point>& points) {
    min_y1 = point(MAXDOUBLE,MAXDOUBLE);
    ll min_y_index = 0;
    for(int q = 0;q < points.size();q++){
        if(points[q].y < min_y1.y) {
            min_y1 = points[q];
            min_y_index = q;
        }
    }

    std::vector<point> f(points.begin(),points.end());
    f.erase(f.begin() + min_y_index);
    std::sort(f.begin(),f.end(),[&](point a,point b) {
        return orientate(min_y1,a,b) == -1;
    });

    std::vector<point> res;
    res.push_back(min_y1);

    for(int q = 0;q < f.size();q++) {
        while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),f[q]) == 1) {
            res.pop_back();
        }
        res.push_back(f[q]);
    }
    while(res.size() >= 2 && orientate(res[res.size() - 2],res.back(),min_y1) == 1) {
        res.pop_back();
    }

    return std::set<point,compare1>(res.begin(),res.end());
}

std::set<point,compare1>::iterator back_interate(std::set<point,compare1>& s,std::set<point,compare1>::iterator it) {
    auto res = it;
    if(res == s.begin()) res = s.end();
    return --res;
}

std::set<point,compare1>::iterator front_interate(std::set<point,compare1>& s,std::set<point,compare1>::iterator it) {
    auto res = it;
    ++res;
    if(res == s.end()) return s.begin();
    return res;
}

bool check_is_in_convex(std::set<point,compare1>& hull,point point) {
    auto it = hull.upper_bound(point);
    if(it == hull.end()) it = hull.begin();

    auto prev_it = back_interate(hull,it);

    return (orientate(*prev_it,point,*it) == 1);
}

bool check_is_in_convex_brute_force(std::set<point,compare1>& hull,point point) {
    auto it1 = hull.begin();
    auto it2 = front_interate(hull, it1);
    while(it2 != hull.end()) {
        if(orientate(*it1,*it2,point) == 1) return false;
        ++it1,++it2;
    }
    if(orientate(*it1,*hull.begin(),point) == 1) return false;
    return true; 
}

void add_point_convex_hull(std::set<point,compare1>& hull, point new_point, point& min_y) {
    if(check_is_in_convex(hull, new_point)) return;

    auto clock_it = hull.upper_bound(new_point);
    if(clock_it == hull.end()) clock_it = hull.begin();
    auto counter_it = back_interate(hull, clock_it);

    while(hull.size() >= 2) {
        auto front = front_interate(hull, clock_it);
        if(orientate(new_point,*clock_it,*front) != 1) break;
        hull.erase(clock_it);
        clock_it = front;
    }

    while(hull.size() >= 2) {
        auto back = back_interate(hull, counter_it);
        if(orientate(*back,*counter_it,new_point) != 1) break;
        hull.erase(counter_it);
        counter_it = back;
    }

    auto reerere = hull.insert(new_point);

    min_y = *hull.begin();
}

bool check_all_in(std::set<point,compare1>& hull,std::vector<point>& all_points) {
    for(auto it = all_points.begin();it != all_points.end();it++) {
        if(!check_is_in_convex_brute_force(hull,*it)) return false;
    }
    return true;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

double get_rand_double(ll min,ll max) {
    min *= 10000;
    max *= 10000;
    return double((ull)get_rand_value() % (max - min + 10000) + min) / 10000;
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
    std::vector<point> init_point(std::rand() % 10 + 10);
    for(int q = 0;q < init_point.size();q++) {
        init_point[q] = get_rand_point(100);
    }
    std::vector<point> all_point(init_point.begin(),init_point.end());

    std::set<point,compare1> hull = get_convex_hull(init_point);

    auto do_add = [&](point new_point) {
        all_point.push_back(new_point);
        add_point_convex_hull(hull,new_point,min_y1);
    }; 
    auto do_check = [&](point point) {
        return check_is_in_convex(hull,point) == check_is_in_convex_brute_force(hull, point);
    };
    auto do_checK_all = [&](){
        return check_all_in(hull,all_point);
    };

    

    for(int q = 0;q < 10000;q++) {
        int mode = std::rand() % 3;
        if(mode == 0) {
            point new_point = get_rand_point(500);
            do_add(new_point);
        }
        else if(mode == 1) {
            point p = get_rand_point(500);
            if(!do_check(p)) {
                return false;
            }
        }
        else {
            if(!do_checK_all()) {
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
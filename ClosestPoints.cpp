#include <iostream>
#include <vector>
#include <Math.h>
#include <algorithm>

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
};

const auto compare_x = [](point a,point b) {
    return a.x < b.x;
};

const auto compare_y = [](point a,point b) {
    return a.y < b.y;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

double get_distance(point a,point b) {
    return std::sqrt(std::pow((b.y - a.y),2) + std::pow(b.x - a.x,2));
}

double recur1(std::vector<point>& points, ll left,ll right) {
    if(left == right) 
        return MAXDOUBLE;

    double min_local = MAXDOUBLE;
    ll mid = get_mid(left,right);
    min_local = std::min(min_local,recur1(points,left,mid));
    min_local = std::min(min_local,recur1(points,mid + 1,right));
    double min_ans = min_local;

    double x_mid = points[mid].x;
    point left_border; left_border.x = x_mid - min_local;
    point right_border; right_border.x = x_mid + min_local;
    ll start = std::distance(points.begin(),std::lower_bound(points.begin() + left, points.begin() + right + 1,left_border,compare_x));
    ll end = std::distance(points.begin(),std::upper_bound(points.begin() + left,points.begin() + right + 1,right_border,compare_x));
    std::vector<point> left_point(points.begin() + start, points.begin() + mid + 1);
    std::vector<point> right_point(points.begin() + mid + 1,points.begin() + end); 
    std::sort(left_point.begin(),left_point.end(), compare_y);
    std::sort(right_point.begin(),right_point.end(), compare_y);
    ll startr = 0;
    for(int q = 0;q < left_point.size();q++) {
        while(startr < right_point.size() && left_point[q].y - right_point[startr].y > min_local) 
            startr++;
        for(int w = startr; w < right_point.size() && right_point[w].y - left_point[q].y < min_local;w++) {
            min_ans = std::min(min_ans,get_distance(left_point[q],right_point[w]));
        }
    }
    return min_ans;
}

double get_closst_distance(std::vector<point> points) {
    std::sort(points.begin(),points.end(),[](point a,point b) {
        return a.x < b.x;
    });
    double get_min_distance = recur1(points,0,points.size() - 1);
    return get_min_distance;
}

double brute_force_get_closest_point(std::vector<point>& points) {
    double ans = MAXDOUBLE;
    for(int q = 0;q < points.size();q++) {
        for(int w = q + 1;w < points.size();w++){
            ans = std::min(ans,get_distance(points[q],points[w]));
        }
    }
    return ans;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

double get_rand_double(ll min,ll max) {
    return double((ull)get_rand_value() % ((max - min + 1) * 10000)) / 10000 + min;
}

bool run_test() {
    std::vector<point> points;
    ll point_number = (ull)get_rand_value() % (10000);
    for(int q = 0;q < point_number;q++) {
        point put_in;
        put_in.x = get_rand_double(-1000,1000);
        put_in.y = get_rand_double(-1000,1000);
        points.push_back(put_in);
    }
    double get_ans = brute_force_get_closest_point(points);
    double try_ans = get_closst_distance(points);
    return get_ans == try_ans;
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

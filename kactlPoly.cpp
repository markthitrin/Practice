#include <iostream>
#include <vector>

#define rep(i,s,t) for(int i = s;i < t;i++)
#define sz(v) v.size()

using namespace std;

struct Poly{
    vector<double>a;
    double operator()(double x) const {
        double val=0;
        for(int i=sz(a);i--;)(val *= x)+=a[i];
        return val;
    }
    void diff() {
        rep(i,1,sz(a))a[i-1]=i*a[i];
        a.pop_back();
    }
    void divroot(double x0){
        double b=a.back(),c;a.back()=0;
        for(int i=sz(a)-1;i--;)c=a[i],a[i]=a[i+1]*x0+b,b=c;
        a.pop_back();
    }
};

int main() {
    Poly a({{2,3,4,5,5}});
    a.diff();
    auto print_poly = [&](Poly& a) {
        for(int q = 0;q < a.a.size();q++) std::cout << a.a[q] << " ";
        std::cout << std::endl;
    };
    print_poly(a);

    Poly b({{1,2,1}});
    b.divroot(-1);
    print_poly(b);
}
#include <iostream>
#include <vector>

using ll = long long;

typedef double d;
#define S(a,b)(f(a)+4*f((a+b)/2)+f(b)) * (b-a) /6
template <class F>
d rec(F&f,d a,d b,d eps,d S){
    d c=(a+b)/2;
    d S1=S(a,c),S2=S(c,b),T=S1+S2;
    if (abs(T-S)<=15 * eps||b-a<1e-10)
    return T+(T-S)/15;
    return rec(f,a,c,eps/2,S1)+rec(f,c,b,eps/2,S2);
}
template<class F>
d quad(d a,d b,F f,d eps=1e-8) {
    return rec(f,a,b,eps,S(a,b));
}

int main() {
    double sphereVolumn = quad(-10, 10, [](double x)->double {
        return quad(-10, 10, [&](double y)->double {
            if(x*x + y*y < 100) return 1; else return 0;
            return quad(-1, 1, [&](double z)->double{
                return (x*x + y*y + z*z) < 1 ? 1 : 0;
            });
        });
    });
    std::cout << "res : " << sphereVolumn << std::endl;
}
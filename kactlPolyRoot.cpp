#include <iostream>
#include <vector>
#include <algorithm>

#define sz(v) v.size()
#define rep(i,s,t) for(int i = s;i < t;i++)
#define all(v) v.begin(),v.end()

using namespace std;

using ll = long long;

struct Poly{
    vector<double> a;
    double operator()(double x) const{
        double val=0;
        for(int i=sz(a);i--;)(val *=x)+=a[i];
            return val;
    }
    void diff(){
        rep(i,1,sz(a))  a[i-1]=i*a[i];
        a.pop_back();
    }
    void divroot(double x0){
        double b=a.back(),c;a.back()=0;
        for(int i=sz(a)-1;i--;)c=a[i],a[i]=a[i+1]*x0+b,b=c;
        a.pop_back();
    }
};

vector<double>polyRoots(Poly p, double xmin, double xmax){
    if (sz(p.a)==2){return{-p.a[0]/p.a[1]};}
    vector<double>ret;
    Poly der=p;
    der.diff();
    auto dr=polyRoots(der,xmin,xmax);
    dr.push_back(xmin-1);
    dr.push_back(xmax+1);
    sort(all(dr));
    rep(i,0,sz(dr)-1) {
        double l=dr[i],h=dr[i+1];
        bool sign=p(l)>0;
        if (sign^(p(h)>0)){
            rep(it,0,60) {//while (h l>1e 8)
                double m=(l+h)/2,f=p(m);
                if ((f<=0)^sign)l=m;
                    else h=m;
            }
            ret.push_back((l+h)/2);
        }
    }
    return ret;
}

int main() {
    Poly a({{1,3,3,1}});
    vector<double> res = polyRoots(a,-10e9,10e9);
    for(int q = 0;q < res.size();q++) {
        std::cout << res[q] << " ";
    }
    std::cout << std::endl;
    for(int q = 0;q < res.size();q++) {
        std::cout << a(res[q]) << " ";
    }
}



#include <iostream>
#include <vector>

#define rep(i,s,t) for(int i = s;i < t;i++)

using namespace std;
using ll = long long;

typedef vector<double> vd;
vd interpolate(vd x,vd y, int n){
    vd res(n),temp(n);
    rep(k,0,n-1) rep(i,k+1,n)
    y[i]=(y[i]-y[k])/(x[i]-x[k]);
    double last=0;temp[0]=1;
    rep(k,0,n) rep(i,0,n){
        res[i]+=y[k] * temp[i];
        swap(last,temp[i]);
        temp[i]-=last * x[k];
    }
    return res;
}

int main() {
    vd res = interpolate({-1,0,1},{1,0,2},3);
    for(int q = 0;q < res.size();q++) {
        std::cout << res[q] << " ";
    }
}
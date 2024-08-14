#include <iostream>
#include <vector>

using namespace std;

#define sz(x) x.size()
typedef vector<int> vi;
typedef pair<int,int> pii;

struct RollbackUF{
    vi e;vector<pii>st;
    RollbackUF(int n):e(n,-1){}
    int size(int x){return-e[find(x)];}
    int find(int x){return e[x]<0?x:find(e[x]);}
    int time(){return sz(st);}
    void rollback(int t){
        for(int i=time();i-->t;)
            e[st[i].first]=st[i].second;
        st.resize(t);
    }
    bool join(int a, int b){
        a=find(a),b=find(b);
        if (a==b) return false;
        if (e[a]>e[b]) swap(a,b);
        st.push_back({a,e[a]});
        st.push_back({b,e[b]});
        e[a]+=e[b];e[b]=a;
        return true;
    }
};


int main() {
    const int N = 100;
    RollbackUF D(N);

    D.join(1,2);
    D.join(3,4);
    std::cout << (D.find(1) == D.find(4)) << std::endl;
    std::cout << (D.find(2) == D.find(1)) << std::endl;
    D.join(4,5);
    std::cout << (D.find(4) == D.find(5)) << std::endl;
    int t = D.time();
    D.join(1,5);
    std::cout << (D.find(1) == D.find(4)) << std::endl;
    D.rollback(t);
    std::cout << (D.find(1) == D.find(4)) << std::endl;
}

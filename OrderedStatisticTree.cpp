#include <bits/extc++.h>

using namespace __gnu_pbds;
using namespace std;

template<class T>
using Tree = tree<T, null_type, less<T>,rb_tree_tag, tree_order_statistics_node_update>;

class name{
public:
    int x;
    int y;
    name() {}
    name(int _x,int _y) : x(_x), y(_y) {;}
    bool operator<(const name& other) const{
        return x != other.x ? x < other.x : y < other.y;
    }
    void print() const {
        std::cout << "(" << x << "," << y << ")\n";
    }
};

int main() {
    Tree<name> t1,t2;
    t1.insert(name(1,2));
    t1.insert(name(2,2));
    t1.insert(name(3,3));
    t1.insert(name(3,2));
    t1.insert(name(5,1));
    
    std::cout << "print lower bound" << std::endl;
    t1.lower_bound(name(0,0))->print();
    t1.lower_bound(name(2,3))->print();
    std::cout << std::endl;

    std::cout << "print upper bound" << std::endl;
    t1.lower_bound(name(1,3))->print();
    t1.upper_bound(name(2,2))->print();
    std::cout << std::endl;

    std::cout << "order of key" << std::endl;
    std::cout << t1.order_of_key(name(0,0)) << std::endl;
    std::cout << t1.order_of_key(name(1,2)) << std::endl;
    std::cout << t1.order_of_key(name(3,2)) << std::endl;
    std::cout << t1.order_of_key(name(3,1)) << std::endl;
    std::cout << std::endl;

    std::cout << "find by order" << std::endl;
    for(int q = 0;q < t1.size();q++) {
        t1.find_by_order(q)->print();
    }
    t1.find_by_order(t1.size())->print();
    std::cout << std::endl;

    std::cout << "print iterating" << std::endl;
    for(auto it = t1.begin();it != t1.end();it++) {
        it->print();
    }
    std::cout << std::endl;

    std::cout << "iterting lower bound" << std::endl;
    auto it = t1.find_by_order(3);
    ++it;
    it->print();
    std::cout << std::endl;

    std::cout << "joining tree";
    t2.insert(name(11,2));
    t2.insert(name(12,3));
    t2.insert(name(12,6));
    t2.insert(name(12,0));
    t2.insert(name(19,4));
    t2.insert(name(10,-1));
    t1.join(t2);
    for(auto it = t1.begin();it != t1.end();it++) {
        it->print();
    }
    std::cout << std::endl;

    return 0;
}
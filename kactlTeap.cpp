#include <iostream>
#include <vector>

using ll = long long;
using namespace std;

struct Node{
    Node *l=0, *r=0;
    int val,y,c=1;
    Node(int val):val(val),y(rand()){}
    void recalc();
};

int cnt(Node* n) {
    return n?n->c:0;
}

void Node::recalc() {
    c=cnt(l)+cnt(r)+1;
}

template<class F> void each(Node* n,F f) {
    if (n){each(n->l,f);f(n->val);each(n->r,f);}
}
pair<Node*,Node*>split(Node* n, int k) {
    if (!n) return{};
    if (cnt(n->l)>=k) {// n>val>=k for lowerbound(k)
        auto pa=split(n->l,k);
        n->l=pa.second;
        n->recalc();
        return{pa.first,n};
    } else {
        auto pa=split(n->r,k-cnt(n->l)-1);//and just k
        n->r=pa.first;
        n->recalc();
        return{n,pa.second};
    }
}
Node* merge(Node* l,Node* r){
    if (!l) return r;
    if (!r) return l;
    if (l->y>r->y){
        l->r=merge(l->r,r);
        l->recalc();
        return l;
    }
    else {
        r->l=merge(l,r->l);
        r->recalc();
        return r;
    }
}
Node* ins(Node* t,Node* n, int pos){
    auto pa=split(t,pos);
    return merge(merge(pa.first,n),pa.second);
}
//Example application: move the range [l, r) to index k
void move(Node*&t, int l, int r, int k){
    Node *a, *b, *c;
    tie(a,b)=split(t,l);tie(b,c)=split(b,r-l);
    if (k<=l)t=merge(ins(a,b,k),c);
    else t=merge(a,ins(c,b,k-r));
}

int main() {
    const std::string ss = "Hello world welcome to the earth";
    Node* root = nullptr;
    for(int q = 0;q < ss.size();q++) {
        Node* new_node = new Node(q);
        root = ins(root,new_node,q);
    }
    auto print_node = [&](int a) {
        std::cout << ss[a];
    };
    each(root,print_node); std::cout << std::endl;

    Node *left,*right;
    tie(left,right) = split(root,15);

    each(left,print_node); std::cout << std::endl;
    each(right,print_node); std::cout << std::endl;

    return 0;
}
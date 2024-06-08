// This code is tested by https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

#include <iostream>
#include <vector>
using ll = long long;

class SplayNode {
public:
    SplayNode* left = nullptr;
    SplayNode* right = nullptr;
    SplayNode* parent = nullptr;
    bool flip = false;
    ll sum = 0;
    bool is_root() {
        return parent == nullptr || (parent->right != this && parent->left != this);
    }
};

bool get_child_side(SplayNode* child) {
    return (child->parent->right) == child;   
}

SplayNode*& get_child_pointer(SplayNode* child) {
    return get_child_side(child) ? child->parent->right : child->parent->left;
}

void fix(SplayNode* child) {
    if(child->flip) {
        child->flip = false;
        std::swap(child->left,child->right);
        if(child->left) child->left->flip ^= true;
        if(child->right) child->right->flip ^= true;
    }
}

ll get_sum(SplayNode* node) {
    if(node) return node->sum;
    return 0;
}

void rotate(SplayNode* child) {
    SplayNode* parent = child->parent;
    SplayNode* root = parent->parent;

    if(!parent->is_root()) get_child_pointer(parent) = child;

    fix(parent);
    fix(child);

    ll old_parent_value = parent->sum;

    if(get_child_side(child)) {
        parent->sum -= get_sum(parent->right);
        parent->sum += get_sum(child->left);

        parent->right = child->left;
        child->left = parent;
        if(parent->right) parent->right->parent = parent;
    }
    else {
        parent->sum -= get_sum(parent->left);
        parent->sum += get_sum(child->right);

        parent->left = child->right;
        child->right = parent;
        if(parent->left) parent->left->parent = parent;
    }

    child->sum = old_parent_value;

    parent->parent = child;
    child->parent = root;
}

void splay(SplayNode* node) {
    while(!node->is_root()) {
        auto parent = node->parent;
        if(!parent->is_root()) {
            fix(parent->parent);
            fix(parent);
            if(get_child_side(parent) == get_child_side(node))
                rotate(parent);
            else rotate(node);
        }
        fix(node);
        rotate(node);
    }
    fix(node);
}

SplayNode* access(SplayNode* node) {
    SplayNode* last = nullptr;
    for(SplayNode* p = node;p;p= p->parent) {
        splay(p);
        p->sum -= get_sum(p->right);
        p->sum += get_sum(last);
        p->right = last;
        last = p;
    }
    splay(node);
    return last;
}

void set_root(SplayNode* node) {
    access(node);
    if(node->left) {
        node->left->flip ^= true;
        node->sum -= get_sum(node->left);
        node->left = nullptr;
    }
}

void link(SplayNode* node1,SplayNode* node2) {
    access(node1);
    set_root(node2);
    node2->parent = node1;
}

void cut(SplayNode* node1,SplayNode* node2) {
    set_root(node1);
    access(node2);
    node2->left->parent = nullptr;
    node2->sum -= get_sum(node2->left);
    node2->left = nullptr;
}

ll get_path_sum(SplayNode* node1,SplayNode* node2) {
    set_root(node1);
    access(node2);
    return get_sum(node2);
}

void add_value(SplayNode* node,ll value) {
    access(node);
    node->sum += value;
}

int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<SplayNode*> nodes(N);
    for(int q = 0;q < N;q++) {
        nodes[q] = new SplayNode();
        std::cin >> nodes[q]->sum;
    }
    for(int q = 0;q < N - 1;q++) {
        int u,v;
        std::cin >> u >> v;
        link(nodes[u],nodes[v]);
    }
    for(int q = 0;q < M;q++) {
        int mode = 0;
        std::cin >> mode;
        if(mode == 0) {
            int u,v,w,x;
            std::cin >> u >> v >> w >> x;
            cut(nodes[u],nodes[v]);
            link(nodes[w],nodes[x]);
        }
        else if(mode == 1) {
            int  p;
            ll x;
            std::cin >> p >> x;
            add_value(nodes[p],x);
        }
        else {
            int u,v;
            std::cin >> u >> v;
            std::cout << get_path_sum(nodes[u],nodes[v]) << std::endl;
        }
    }
}
// This code is tested by https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

#include <iostream>
#include <vector>

using ll = long long;

class SplayNode {
public:
    SplayNode* left = nullptr;
    SplayNode* right = nullptr;
    SplayNode* parent = nullptr;
    ll sum = 0;
    bool flip = false;
    bool is_root() {
        return parent == nullptr || (parent->right != this && parent->left != this);
    }
};

bool get_child_side(SplayNode* child) {
    return child->parent->right == child;
}

SplayNode*& get_child_pointer(SplayNode* child) {
    return get_child_side(child) ? child->parent->right : child->parent->left;
}

ll get_sum(SplayNode* node) {
    if(node) return node->sum;
    return 0;
}

void fix(SplayNode* node) {
    if(node->flip) {
        node->flip = false;
        std::swap(node->left,node->right);
        if(node->left) node->left->flip ^= true;
        if(node->right) node->right->flip ^= true;
    }
}

ll get_real_sum(SplayNode* node) {
    if(!node) return 0;
    if(!node->is_root()) {
        get_real_sum(node->parent);
    }
    fix(node);
    return node->sum;
}

void rotate(SplayNode* child) {
    auto parent = child->parent;
    auto root = parent->parent;

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

    child->parent = root;
    parent->parent = child;
}

void splay(SplayNode* node) {
    while(!node->is_root()) {
        auto parent = node->parent;
        auto root = parent->parent;
        if(!parent->is_root()) {
            fix(root);
            fix(parent);
            if(get_child_side(parent) == get_child_side(node)) {
                rotate(parent);
            }
            else {
                rotate(node);
            }
        }
        rotate(node);
    }
    fix(node);
}

SplayNode* access(SplayNode* node) {
    SplayNode* last = nullptr;
    for(SplayNode* p = node;p;p=p->parent) {
        splay(p);
        get_real_sum(p);
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
        node->left = nullptr;
    }
}

void link(SplayNode* node1,SplayNode* node2) {
    access(node1);
    set_root(node2);
    node2->parent = node1;
    node1->sum += get_sum(node2);
}

void cut(SplayNode* node1,SplayNode* node2) {
    set_root(node1);
    access(node2);
    node2->sum -= get_sum(node1);
    node1->parent = nullptr;
    node2->left = nullptr;
}

ll get_subtree_sum(SplayNode* parent,SplayNode* child) {
    set_root(parent);
    access(child);
    access(parent);
    return get_sum(child);
}

void add_value(SplayNode* node,ll value) {
    set_root(node);
    node->sum += value;
}

int main() {
    int N,M;
    std::cin >> N >> M;
    std::vector<SplayNode*> nodes;
    for(int q = 0;q <N;q++) {
        nodes.push_back(new SplayNode());
        std::cin >> nodes[q]->sum;
    }
    for(int q = 0;q < N-1;q++) {
        int u,v;
        std::cin>> u >> v;
        link(nodes[u],nodes[v]);
    }
    for(int q = 0;q < M;q++) {
        int mode;
        std::cin >> mode;
        if(mode == 0) {
            int u,v,w,x;
            std::cin >> u >> v >> w >> x;
            cut(nodes[u],nodes[v]);
            link(nodes[w],nodes[x]);
        }
        else if(mode == 1) {
            int p,x;
            std::cin>> p >> x;
            add_value(nodes[p],x);
        }
        else {
            int v,p;
            std::cin >> v >> p;
            std::cout << get_subtree_sum(nodes[p],nodes[v]) << std::endl;
        }
    }
}


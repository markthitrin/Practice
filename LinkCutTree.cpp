// This code is tested by https://www.spoj.com/problems/DYNACON1/

#include <iostream>
#include <vector>

class SplayNode {
public:
    SplayNode* left = nullptr;
    SplayNode* right = nullptr;
    SplayNode* parent = nullptr;
    bool flip = false;
    bool is_root() {
        return parent == 0 || (parent->left != this && parent->right != this);
    }
};

void fix(SplayNode* node) {
    if(node->flip) {
        node->flip = false;
        std::swap(node->left,node->right);
        if(node->left) node->left->flip ^= true;
        if(node->right) node->right->flip ^= true;
    }
}

bool get_child_side(SplayNode* child) {
    if(child->parent->right == child) return true;
    else return false;
}

SplayNode*& get_child_pointer(SplayNode* child) {
    if(get_child_side(child)) return child->parent->right;
    else return child->parent->left;
}

void rotate(SplayNode* child) {
    SplayNode* parent = child->parent;
    SplayNode* root = parent->parent;
    if(!parent->is_root()) get_child_pointer(parent) = child;

    fix(parent);
    fix(child);

    if(get_child_side(child)) {
        if(child->left) child->left->parent = parent;
        parent->right = child->left;
        child->left = parent;
    }
    else {
        if(child->right) child->right->parent = parent;
        parent->left = child->right;
        child->right = parent;
    }
    
    parent->parent = child;
    child->parent = root;
}

void splay(SplayNode* root) {
    if(root->is_root()) return fix(root);
    if(root->parent->is_root()) {
        rotate(root);
    }
    else {
        if(get_child_side(root->parent) == get_child_side(root)) {
            rotate(root->parent);
        }
        else {
            rotate(root);
        }
        rotate(root);
        splay(root);
    }
}

SplayNode* get_min_node(SplayNode* node) {
    while(!node->is_root()) node = node->parent;
    fix(node);
    while(node->left) {
        node = node->left;
        fix(node);
    }
    return node; 
}

SplayNode* access(SplayNode* node) {
    SplayNode* lca_child = nullptr;
    for(SplayNode* p = node;p;p = p->parent) {
        splay(p);
        p->right = lca_child;
        lca_child = p;
    }
    splay(node);
    return lca_child;
}

void set_root(SplayNode* node) {
    access(node);
    if(node->left) node->left->flip ^= true;
    node->left = nullptr;
}

void link(SplayNode* node1,SplayNode* node2) {
    set_root(node2);
    node2->parent = node1;
}

void cut(SplayNode* node1,SplayNode* node2) {
    set_root(node1);
    access(node2);
    if(node2->left) {
        node2->left->parent = nullptr;
        node2->left = nullptr;
    }
}

bool is_connected(SplayNode* node1,SplayNode* node2) {
    access(node1);
    access(node2);
    return node1->parent;
}

int main() {
    int N,Q;
    std::cin >> N >> Q;
    std::vector<SplayNode*> nodes(N + 1);
    std::vector<std::string> modes(Q);
    std::vector<int> AA(Q),BB(Q);
    for(int q = 0;q <= N;q++) {
        nodes[q] = new SplayNode();
    }
    for(int q = 0;q < Q;q++) {
        std::cin >> modes[q];
        std::cin >> AA[q] >> BB[q];
    }
    for(int q = 0;q < Q;q++) {
        std::string mode;
        int A,B;
        mode = modes[q];
        A = AA[q];
        B = BB[q];
        if(mode == "add") {
            link(nodes[A],nodes[B]);
        }
        else if(mode == "rem") {
            cut(nodes[A],nodes[B]);
        }
        else {
            if(is_connected(nodes[A],nodes[B])) {
                std::cout << "YES" << std::endl;
            }
            else {
                std::cout << "NO" << std::endl;
            }
        }
    }
    return 0;
}
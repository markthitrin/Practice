// This code is tested by https://www.spoj.com/problems/DYNALCA/

#include <iostream>
#include <vector>

class SplayNode {
public:
    int node_number;
    SplayNode* left = nullptr;
    SplayNode* right = nullptr;
    SplayNode* parent = nullptr;
    bool is_root() {
        return parent == 0 || (parent->left != this && parent->right != this);
    }
};

bool get_child_side(SplayNode* child) {
    if(child->parent->right == child) return true;
    else return false;
}

SplayNode*& get_child_pointer(SplayNode* child) {
    if(get_child_side(child)) return child->parent->right;
    return child->parent->left;
}

void rotate(SplayNode* child) {
    SplayNode* parent = child->parent;
    SplayNode* root = parent->parent;
    
    if(!parent->is_root()) get_child_pointer(parent) = child;
    
    if(get_child_side(child)) {
        parent->right = child->left;
        child->left = parent;
        if(parent->right) parent->right->parent = parent;
    }
    else {
        parent->left = child->right;
        child->right = parent;
        if(parent->left) parent->left->parent = parent;
    }

    child->parent = root;
    parent->parent = child;
}

void splay(SplayNode* child) {
    while (!child->is_root()) {
        auto parent = child->parent;
        auto root = parent->parent;
        if (!parent->is_root()) rotate((root->right == parent) == (parent->right == child) ? parent : child);
        rotate(child);
    }
}

SplayNode* access(SplayNode* node) {
    SplayNode* last = 0;
    for(SplayNode* p = node;p;p = p->parent) {
        splay(p);
        p->right = last;
        last = p;
    }
    splay(node);
    return last;
}

void link(SplayNode* node1,SplayNode* node2) {
    access(node1);
    node1->parent = node2;
}

void cut(SplayNode* node) {
    access(node);
    node->left->parent = nullptr;
    node->left = nullptr;
}

SplayNode* get_lca(SplayNode* node1,SplayNode* node2) {
    access(node1);
    SplayNode* lca = access(node2);
    return lca;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int N,M;
    std::cin >> N >> M;
    std::vector<SplayNode*> nodes(N + 1);
    for(int q = 0;q < nodes.size();q++){
        nodes[q] = new SplayNode();
        nodes[q]->node_number = q;
    }
    for(int q = 0;q < M;q++) {
        std::string mode;
        std::cin >> mode;
        if(mode == "link") {
            int A,B;
            std::cin >> A >> B;
            link(nodes[A],nodes[B]);
        }
        else if(mode == "cut") {
            int A;
            std::cin >> A;
            cut(nodes[A]);
        }
        else {
            int A,B;
            std::cin >> A >> B;
            SplayNode* lca_node = get_lca(nodes[A],nodes[B]);
            std::cout << lca_node->node_number << "\n";
        }
    }
    return 0;
}
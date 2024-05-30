#include <iostream>
#include <vector>
#include <string>
#include <map>

using ll = long long;
using ull = unsigned ll;

class SplayNode{ 
public:
    int value = 0;
    std::string s = "";
    SplayNode* left = nullptr;
    SplayNode* right = nullptr;
    SplayNode* parent = nullptr;
};

SplayNode*& get_child_pointer(SplayNode* root,bool side) {
    if(side) return root->right;
    else return root->left;
}

bool get_child_side(SplayNode* root,SplayNode* child) {
    if(root->right == child) return true;
    else return false;
}

void rotate_right(SplayNode* root) {
    SplayNode* parent = root->parent;
    SplayNode* child = root->right;
    SplayNode* sub_child = child->left;

    child->parent = parent;
    root->parent = child;
    if(sub_child)
        sub_child->parent = root;

    if(parent)
        get_child_pointer(parent,get_child_side(parent,root)) = child;
    root->right = sub_child;
    child->left = root;        
}

void rotate_left(SplayNode* root) {
    SplayNode* parent = root->parent;
    SplayNode* child = root->left;
    SplayNode* sub_child = child->right;

    child->parent = parent;
    root->parent = child;
    if(sub_child)
        sub_child->parent = root;

    if(parent)
        get_child_pointer(parent,get_child_side(parent,root)) = child;
    root->left = child->right;
    child->right = root;
}

void rotate(SplayNode* root,bool side) {
    if(side) rotate_right(root);
    else rotate_left(root);
}

void splay(SplayNode* root) {
    if(root->parent == nullptr) return;
    else if(root->parent->parent == nullptr) {
        bool pc = get_child_side(root->parent,root);
        rotate(root->parent,pc);
    }
    else {
        bool pcc = get_child_side(root->parent->parent,root->parent);
        bool pc = get_child_side(root->parent,root);
        if(pcc == pc) {
            rotate(root->parent->parent,pcc);
            rotate(root->parent,pc);
        }
        else {
            rotate(root->parent,pc);
            rotate(root->parent,pcc);
        }
        splay(root);
    }
}

SplayNode* get_node(SplayNode* root,int value) {
    if(value == root->value) { 
        return root;
    }
    if(value < root->value) {
        if(root->left) return get_node(root->left,value);
    }
    else if(value > root->value) {
        if(root->right) return get_node(root->right,value);
    }
    return root;
}

void insert(SplayNode*& root,SplayNode* new_node) {
    SplayNode* parent = get_node(root,new_node->value);
    if(new_node->value < parent->value) {
        parent->left = new_node;
        new_node->parent = parent;
    }
    else {
        parent->right = new_node;
        new_node->parent = parent;
    }
    splay(new_node);
    root = new_node;
}

std::string get_string(SplayNode*& root,int value) {
    SplayNode* node = get_node(root,value);
    splay(node);
    root = node;
    if(root->value == value) return root->s;
    return "";
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & 15;
}

std::string get_rand_string(int size) {
    std::string res = "";
    for(int q = 0;q < size;q++) {
        res += 'a' + rand() % 26;
    }
    return res;
}

bool run_test() {
    int op_number = 100000 + get_rand_value() % 100000;
    std::vector<int> value_vector;
    std::map<int,std::string> mm;
    SplayNode* root = new SplayNode();
    
    for(int q = 0;q < op_number;q++) {
        int mode = (ull)get_rand_value() % 2;
        if(mode == 0) {
            SplayNode* new_node = new SplayNode();
            int value = (ll)get_rand_value() % 2000000000;
            std::string s = get_rand_string(10);

            if(mm.find(value) != mm.end()) continue;

            mm[value] = s;
            value_vector.push_back(value);
            new_node->s = s;
            new_node->value = value;
            insert(root,new_node);
        }
        else {
            if(value_vector.size() < 1) continue;

            int value_pos = (ull)get_rand_value() % value_vector.size();
            int value = value_vector[value_pos];

            std::string ans = mm[value];
            std::string get_ans = get_string(root, value);

            if(ans != get_ans)
                return false;
        }
    }
    return true;
}

int main() {
    int _ = 0;
    std::cin >> _;
    for(int q = 1;q <= _;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed" << std::endl;
        }
        else {
            std::cout << "failed" << std::endl;
        }
    }
    return 0;
}
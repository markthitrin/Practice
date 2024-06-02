#include <iostream>
#include <vector>
#include <map>
#include <string>

using ll = long long;
using ull = unsigned ll;

class SplayNode {
public:
    int value = -2000000000;
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

    root->parent = child;
    child->parent = parent;
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

    root->parent = child;
    child->parent = parent;
    if(sub_child)
        sub_child->parent = root;
    
    if(parent)
        get_child_pointer(parent,get_child_side(parent,root)) = child;
    root->left = sub_child;
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
    if(value == root->value) return root;
    if(value < root->value) {
        if(root->left) return get_node(root->left,value);
    }
    else {
        if(root->right) return get_node(root->right,value);
    }
    return root;
}

void insert_splay(SplayNode*& root,SplayNode* new_node) {
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
    if(node->value == value) return node->s;
    return "";
}

void split(SplayNode* root,int value, SplayNode*& left,SplayNode*& right) {
    SplayNode* node = get_node(root,value);
    splay(node);
    if(node->value <= value) {
        left = node;
        right = node->right;
        if(left)
            left->right = nullptr;
        if(right)
            right->parent = nullptr;
    }   
    else {
        left = node->left;
        right = node;
        if(left)
            left->parent = nullptr;
        if(right)
            right->left = nullptr;
    }
}

SplayNode* join(SplayNode* left,SplayNode* right) {
    if(!left) return right;
    if(!right) return left;
    SplayNode* root = get_node(left,2000000000);
    splay(root);
    root->right = right;
    root->right->parent = root;
    return root;
}

void delete_splay(SplayNode*& root,int value) {
    SplayNode* delete_node = get_node(root,value);
    splay(delete_node);
    root = delete_node;
    if(delete_node->value != value) return;
    SplayNode* left_tree = delete_node->left;
    if(left_tree)
        left_tree->parent = nullptr;
    SplayNode* right_tree = delete_node->right;
    if(right_tree)
        right_tree->parent = nullptr;
    delete delete_node;
    root = join(left_tree,right_tree);
}

int get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & 15;
}

std::string get_rand_string(int size) {
    std::string res = "";
    for(int q = 0;q < size;q++) {
        res += 'a' + std::rand() % 26;
    }
    return res;
}

bool run_test() {
    int op_num = 50000 + (ull)get_rand_value() % 50000;
    std::vector<std::vector<int>> value_vector(1);
    std::vector<std::map<int,std::string>> mm(1);
    std::vector<SplayNode*> root;
    root.push_back(new SplayNode());

    auto do_insert = [&](int pos,int value,std::string s) {
        value_vector[pos].push_back(value);
        mm[pos][value] = s;
        SplayNode* new_node = new SplayNode();
        new_node->value = value;
        new_node->s = s;
        insert_splay(root[pos],new_node);
    };
    auto do_delete = [&](int pos,int value) {
        for(int q = 0;q < value_vector[pos].size();q++) {
            if(value_vector[pos][q] == value) {
                value_vector[pos].erase(value_vector[pos].begin() + q);
                break;
            }
        }
        mm[pos].erase(value);
        delete_splay(root[pos],value);
        
        if(value_vector[pos].size() == 0) {
            value_vector.erase(value_vector.begin() + pos);
            mm.erase(mm.begin() + pos);
            root.erase(root.begin() + pos);
        }
    };
    auto do_split = [&](int pos,int value) {
        std::vector<int> select_value_vector = value_vector[pos];
        std::map<int,std::string> select_mm = mm[pos];
        SplayNode* select_root = root[pos];

        value_vector.erase(value_vector.begin() + pos);
        mm.erase(mm.begin() + pos);
        root.erase(root.begin() + pos);

        std::vector<int> left_value_vector;
        std::vector<int> right_value_vector;
        std::map<int,std::string> left_mm;
        std::map<int,std::string> right_mm;
        SplayNode* left_root = nullptr;
        SplayNode* right_root = nullptr;
        for(int q = 0;q < select_value_vector.size();q++) {
            if(select_value_vector[q] <= value) {
                left_value_vector.push_back(select_value_vector[q]);
                left_mm[select_value_vector[q]] = select_mm[select_value_vector[q]];
            }
            else {
                right_value_vector.push_back(select_value_vector[q]);
                right_mm[select_value_vector[q]] = select_mm[select_value_vector[q]];
            }
        }
        split(select_root,value,left_root,right_root);

        if(left_value_vector.size()) {
            value_vector.push_back(left_value_vector);
            mm.push_back(left_mm);
            root.push_back(left_root);
        }   
        if(right_value_vector.size()) {
            value_vector.push_back(right_value_vector);
            mm.push_back(right_mm);
            root.push_back(right_root);
        }
    };
    auto do_check = [&](int pos,int value) {
        std::string ans = mm[pos][value];
        std::string get_ans = get_string(root[pos],value);
        return ans == get_ans;
    };

    for(int q = 0;q < op_num;q++) {
        int mode = (ull)get_rand_value() % 10;
        if(mode <= 5) {
            int pos = (ull)get_rand_value() % value_vector.size();
            int value;
            while(mm[pos].find(value) != mm[pos].end()) {
                value = (ull)get_rand_value() % 1000000000;
            }
            std::string s = get_rand_string(15);
            do_insert(pos,value,s);
        }
        else if(mode == 6) {
            int pos = (ull)get_rand_value() % value_vector.size();
            if(value_vector[pos].size() == 0) continue;
            int value_pos = (ull)get_rand_value() % value_vector[pos].size();
            int value = value_vector[pos][value_pos];
            do_split(pos,value);
        }
        else if(mode == 7 && q > 200) {
            int pos = (ull)get_rand_value() % value_vector.size();
            int value_pos = (ull)get_rand_value() % value_vector[pos].size();
            int value = value_vector[pos][value_pos];
            do_delete(pos,value);
        }
        else {
            int pos = (ull)get_rand_value() % value_vector.size();
            if(value_vector[pos].size() == 0) continue;
            int value_pos = (ull)get_rand_value() % value_vector[pos].size();
            int value = value_vector[pos][value_pos];
            if(!do_check(pos,value)) {
                return false;
            }
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
}
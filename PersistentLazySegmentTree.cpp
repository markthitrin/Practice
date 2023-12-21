#include <iostream>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class node {
public:
    ll value = 0;
    ll to_add = 0;
    node* left = nullptr;
    node* right = nullptr;
};

ll get_mid(ll left,ll right) {
    return (right - left) / 2 + left;
}

node* get_mirror(node* head) {
    if(head == nullptr) return new node();
    node* new_node = new node(*head);
    return new_node;
}

void fix(node* head,node* root,ll left,ll right) {
    ll mid = get_mid(left,right);
    if(head == nullptr) {
        root->left = new node();
        root->right = new node();
    }
    else {
        root->left = get_mirror(head->left);
        root->right = get_mirror(head->right);
    }
    root->left->value += (root->to_add) * (mid - left + 1);
    root->left->to_add += (root->to_add);
    root->right->value += (root->to_add) * (right - mid + 1);
    root->right->to_add += (root->to_add);
    root->to_add = 0;
}

void add(node* head,node* root,ll s,ll e,ll x,ll left = MININT,ll right = MAXINT) {
    if(s <= left && right <= e) {
        root->value += (right - left + 1) * x;
        root->to_add +=  
    }
}
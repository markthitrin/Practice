#include <iostream>

int main() {
    int* a = new int;
    (*a) = 0;
    int* b = a;
    delete a;
    int* c = new int;
    std::cout << *c;
}
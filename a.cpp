#include <iostream>
#include <vector>
#include <algorithm>


int main() {
    std::vector<int> a = {6,5,3,2,0,5,4,9,2,1};
    std::vector<int> y(15,0);
    for(int q = 0;q < y.size();q++) {
        if(q < a.size()) y[q] += 3 * a[q];
        if(q  - 5 >= 0 && q - 5 < a.size()) y[q] += a[q - 5]  * 3;
        if(q -3 >= 0 && q-3 < a.size()) y[q] += a[q - 3] * (-1);
    }
    for(int q = 0;q < y.size();q++) {
        std::cout << y[q] << " ";
    }
}
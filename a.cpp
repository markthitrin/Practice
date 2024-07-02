#include <iostream>
#include <set>

int main() {
   for(int q = 60;q>=0;q--) {
      if((1ll << q) & 998244353) std::cout << 1;
      else std::cout << 0;
   }
   int count = 0;
   int m = 998244353 - 1;
   while(m % 2 == 0) {
      ++count;
      m >>= 1;
   }
   std::cout << std::endl << count ;
   return 0;
}
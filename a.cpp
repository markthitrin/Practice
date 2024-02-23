#include <iostream>
#include <set>

int main() {
   std::set<int> s = {1,2,3,4,5,6};
   auto it = s.end();
   --it;
   std::cout << *it;
   while(true) {
      
   }
}
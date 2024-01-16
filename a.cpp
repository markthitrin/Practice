#include<iostream>
#include<map>
#include<vector>

using namespace std;

void locate_target(map<int,vector<int>> &target,map<int,int> monster,int range) {
   for (auto &tow: target) {
      int start = max(0,tow.first-range);
      auto aim = monster.lower_bound(start);
      while (aim != monster.end()) {
         if ((*aim).first > tow.first + range) {
            continue;
         }
         tow.second.push_back((*aim).first);
         aim++;
      }
   }
}

int main() {
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);
   int n,m,k,w;
   cin >> n  >> m >> k >> w;
   int sumhealth = 0;
   vector<int> tower(k);
   map<int,vector<int>> target;
   map<int,int> monster;
   vector<int> monster_place(m);
   for (int i = 0;i < m;i++) {
      int a;
      cin >> a;
      monster_place[i] = a;
   }
   for (int i = 0;i < m;i++) {
      int h;
      cin >> h;
      if (monster.find(i) == monster.end()) {
         monster[i] = 0;
      }
      monster[i] += h;
      sumhealth += h;
   }
   for (int i = 0;i < k;i++) {
      int t;
      cin >> t;
      target[t] = {};
      tower[i] = t;
   }
   locate_target(target,monster,w);
   for (auto t : target) {
      for (auto m : t.second) {
         cout << m << " ";
      }
      cout << "\n";
   }
}
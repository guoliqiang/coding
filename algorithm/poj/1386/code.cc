// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-12 21:37:38
// File  : code.cc
// Brief :
// abc efg  hml omj
// 图为：a --> c --> e --> g -- >h -->l -->o -- >j
// 可能有环
// poj用ｃ

#include "base/public/common_head.h"

namespace algorithm {

bool visited[26] = {false};
int income[26] = {0};
int outcome[26] = {0};
int uset[26] = {};

void Init() {
  for (int i = 0; i < 26; i++) {
    uset[i] = i;
    income[i] = 0;
    outcome[i] = 0;
    visited[i] = false;
  }
}

int Find(int i) {
  return uset[i] = uset[i] == i ? i : Find(uset[i]);
}

void Merge(int x, int y) {
  int p_x = Find(x);
  int p_y = Find(y);
  if (p_x != p_y) uset[p_x] = p_y;
}

void ReadFromConsole() {
  int group = 0;
  std::cin >> group;
  while (group--) {
    Init();
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
      std::string t;
      std::cin >> t;
      int b = t[0] - 'a';
      int e = t[t.size() - 1] - 'a';
      income[b]++;
      outcome[e]++;
      visited[b] = true;
      visited[e] = true;
      Merge(b, e);
    }
    int pre = -1;
    int i = 0;
    for (i = 0; i < 26; i++) {
      if (visited[i] && pre == -1) {
        pre = i;
        continue;
      }
      if (visited[i] && Find(i) != Find(pre)) break;
    }
    if (i != 26)
      std::cout << "The door cannot be opened." << std::endl;
    else {
      int bad = 0;
      for (i = 0; i < 26; i++) {
        if (visited[i] && abs(income[i] - outcome[i]) == 0) continue;
        if (visited[i] && abs(income[i] - outcome[i]) == 1) bad++;
        else if (visited[i]) break;
      }
      if (i != 26 || bad == 1 || bad > 2)
        std::cout << "The door cannot be opened." << std::endl;
      else
        std::cout << "Ordering is possible." << std::endl;
    }
  }
}

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  ReadFromConsole();
  return 0;
}

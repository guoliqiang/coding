// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-11 01:34:50
// File  : code.cc
// Brief :

// poj 1521
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 10000;
int code[27];
int cnt[27];
struct Node {
  int val;
  std::vector<char> chars;
};
char data[MAXN];

struct Cmp {
 bool operator ()(const Node & x, const Node & y) const {
   return x.val > y.val;
 }
};

void Solve() {
  memset(cnt, 0, sizeof(cnt));
  memset(code, 0, sizeof(code));
  int k = 0;
  while (data[k] != '\0') {
    if (data[k] == '_') cnt[26]++;
    else cnt[data[k] - 'A']++;
    k++;
  }
  std::priority_queue<Node, std::vector<Node>, Cmp> queue;
  for (int i = 0; i < 27; i++) {
    if (cnt[i] == 0) continue;
    Node t;
    t.val = cnt[i];
    if (i == 26) {
      t.chars.push_back('_');
    } else {
      t.chars.push_back(i + 'A');
    }
    queue.push(t);
  }
  if (queue.size() == 1) {
    k = 0;
    while (data[k] != 0) {
      if (data[k] == '_') code[26] = 1;
      else code[data[k] - 'A'] = 1;
      k++;
    }
  } else {
    while (queue.size() > 1) {
      Node t1 = queue.top();
      queue.pop();
      Node t2 = queue.top();
      queue.pop();
      for (int i = 0; i < t1.chars.size(); i++) {
        if (t1.chars[i] == '_') code[26]++;
        else code[t1.chars[i] - 'A']++;
      }
      for (int i = 0; i < t2.chars.size(); i++) {
        if (t2.chars[i] == '_') code[26]++;
        else code[t2.chars[i] - 'A']++;
      }
      t2.val += t1.val;
      t2.chars.insert(t2.chars.end(), t1.chars.begin(), t1.chars.end());
      queue.push(t2);
    }
  }
  int len = 0;
  int clen = 0;
  k = 0;
  while (data[k] != '\0') {
    len++;
    if (data[k] == '_') clen += code[26];
    else clen += code[data[k] - 'A'];
    k++;
  }
  printf("%d %d %.1lf\n", len * 8, clen, double(len * 8) / double(clen));
}


void Read() {
  while (scanf("%s", data) != EOF) {
    getchar();
    if (strcmp(data, "END") == 0) break;
    Solve();
  }
}


}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}

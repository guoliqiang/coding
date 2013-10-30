// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-29 22:58:45
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

struct Node {
  std::vector<std::vector<int> > v;
  void Push(int n) {
    std::vector<int> foo;
    for (int i = 0; i < n; i++) foo.push_back(i + 1);
    v.push_back(foo);
  }
  void PushEach(int n) {
    for (int i = 0; i < v.size(); i++) {
      v[i].push_back(n);
    }
  }
  void PushVec(int n) {
    v.push_back(std::vector<int>(1, n));
  }
  void Out() {
    LOG(INFO) << JoinMatrix(&v);
  }
  void PushAt(int i, int n) {
    v[i].push_back(n);
  }
};

// 第二类stirling数
// S(n, k) = S(n - 1, k - 1) + k * S(n - 1, k)
void SetSplit(int n, int k, std::vector<Node> & v) {
  if (k > n) return;
  
  if (k == n) {
    Node node;
    for (int i = 0; i < n; i++) {
      node.PushVec(i + 1);
    }
    v.push_back(node);
    return;
  }
  
  if (k == 1) {
    Node node;
    node.Push(n);
    v.push_back(node);
    return;
  }

  std::vector<Node> tmp;
  SetSplit(n - 1, k - 1, tmp);
  for (int i = 0; i < tmp.size(); i++) {
    tmp[i].PushVec(n);
  }
  for (int i = 0; i < tmp.size(); i++) {
    v.push_back(tmp[i]);
  }
  tmp.clear();
  SetSplit(n - 1, k, tmp);
  for (int i = 0; i < tmp.size(); i++) {
    Node t = tmp[i];
    for (int j = 0; j < t.v.size(); j++) {
      t.PushAt(j, n);
      v.push_back(t);
      t.v[j].pop_back();
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<Node> v;
  SetSplit(4, 2, v);
  for (int i = 0; i < v.size(); i++) {
    v[i].Out();
  }
  return 0;
}

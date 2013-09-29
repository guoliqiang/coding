// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 19:56:58
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 11/11 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 34/34 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

std::vector<std::vector<int> > LevelOrderBottom(TreeNode * root) {
  std::vector<std::vector<int> > rs;
  if (root == NULL) return rs;
  std::queue<TreeNode *> queue;
  queue.push(root);
  queue.push(NULL);
  std::vector<int> foo;
  while (!queue.empty()) {
    TreeNode * t = queue.front();
    queue.pop();
    if (t == NULL) {
      rs.push_back(foo);
      foo.clear();
      if (queue.empty()) break;
      queue.push(NULL);
    } else {
      foo.push_back(t->val);
      if (t->left != NULL) queue.push(t->left);
      if (t->right != NULL) queue.push(t->right);
    }
  }
  for (int i = 0; i< rs.size() / 2; i++) {
    std::swap(rs[i], rs[rs.size() - 1 - i]);
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

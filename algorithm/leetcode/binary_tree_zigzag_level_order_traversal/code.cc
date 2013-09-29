// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 17:51:14
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 11/11 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 32 milli secs
 * Progress: 33/33 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

std::vector<std::vector<int> > ZigzagLevelOrder(TreeNode * root) {
  std::vector<std::vector<int> > rs;
  if (root == NULL) return rs;

  bool flag = false;  // true from left to right
  std::queue<TreeNode *> queue;
  queue.push(root);
  queue.push(NULL);
  std::vector<int> foo;
  while (!queue.empty()) {
    TreeNode * t = queue.front();
    queue.pop();
    if (t == NULL) {
      if (flag) {
        std::vector<int> tmp(foo.rbegin(), foo.rend());
        foo = tmp;
      }
      rs.push_back(foo);
      foo.clear();
      flag = !flag;
      if (queue.empty()) break;
      queue.push(NULL);
    } else {
      foo.push_back(t->val);
      if (t->left != NULL) queue.push(t->left);
      if (t->right != NULL) queue.push(t->right);
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

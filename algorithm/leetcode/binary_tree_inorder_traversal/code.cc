// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 17:16:40
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 67/67 test cases passed.
 *
 * TODO preorder postorder
 * */

#include "base/public/common_head.h"

namespace algorithm {

std::vector<int> Inorder(TreeNode * root) {
  std::vector<int> rs;
  std::stack<TreeNode*> stack;
  TreeNode * tmp = root;
  while (tmp != NULL) {
    stack.push(tmp);
    tmp = tmp->left;
  }
  while (!stack.empty()) {
    tmp = stack.top();
    stack.pop();
    rs.push_back(tmp->val);
    tmp = tmp->right;
    while (tmp != NULL) {
      stack.push(tmp);
      tmp = tmp->left;
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

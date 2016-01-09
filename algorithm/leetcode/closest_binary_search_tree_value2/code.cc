// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 23:35:44
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

std::vector<int> ClosestKValues(TreeNode * root, int target, int k) {
  std::queue<int> ans;
  std::queue<TreeNode *> queue;
  while (root != NULL) {
    queue.push(root);
    root = root->left;
  }

  while (queue.size()) {
    TreeNode * cur = queue.front();
    queue.pop();
    if (ans.size() < k) ans.push(cur->val);
    else {
      if (fabs(cur->val - target) < fabs(ans.front() - target)) {
        ans.pop();
        ans.push(cur->val);
      }
    }

    cur = cur->right;
    while (cur != NULL) {
      queue.push(cur);
      cur = cur->left;
    }
  }

  std::vector<int> rs;
  while (ans.size()) {
    rs.push_back(ans.front());
    ans.pop();
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace NB {
}  // namespace NB

int main(int argc, char** argv) {
  return 0;
}

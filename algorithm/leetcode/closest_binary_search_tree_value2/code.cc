// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 23:35:44
// File  : code.cc
// Brief :

/*
Given a non-empty binary search tree and a target value, find k values in the BST that are closest to the target.

Note: Given target value is a floating point. You may assume k is always valid, that is: k ≤ total nodes. You are guaranteed to have only one unique set of k values in the BST that are closest to the target. Follow up: Assume that the BST is balanced, could you solve it in less than O(n) runtime (where n = total nodes)?

Hint:

Consider implement these two helper functions: getPredecessor(N), which returns the next smaller node to N. getSuccessor(N), which returns the next larger node to N.
*/

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

namespace algorithm {
std::vector<int> ClosestKValues(TreeNode * root, int target, int k) {
  std::queue<int> ans;
  std::stack<TreeNode *> queue;
  while (root != NULL) {
    queue.push(root);
    root = root->left;
  }

  while (queue.size()) {
    TreeNode * cur = queue.top();
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

// 从小到大的访问，然后把小于的加入到栈中
void InOrder(TreeNode * root, int t, std::stack<int> & s) {
  if (root == NULL) return;
  InOrder(root->left, t, s);
  if (root->val <= t) {
    s.push(root->val);
    InOrder(root->right, t, s);
  }
}
// 其实是从大到小的访问，然后把大于的加入到栈中
void InOrder2(TreeNode * root, int t, std::stack<int> & s) {
  if (root == NULL) return;
  InOrder2(root->right, t, s);
  if (root->val > t) {
    s.push(root->val);
    InOrder2(root->left, t, s);
  }
}

std::vector<int> ClosestKValues(TreeNode * root, int target, int k) {
  std::stack<int> pre;
  std::stack<int> suc;
  InOrder(root, target, pre);
  InOrder2(root, target, suc);

  std::vector<int> ans;
  while (ans.size() < k && pre.size() + suc.size() > 0) {
    if (pre.size() == 0) {
      ans.push_back(suc.top());
      suc.pop();
    } else if (suc.size() == 0) {
      ans.push_back(pre.top());
      pre.pop();
    } else if (fabs(pre.top() - target) < fabs(suc.top() - target)) {
      ans.push_back(pre.top());
      pre.pop();
    } else {
      ans.push_back(suc.top());
      suc.pop();
    }
  }

  return ans;
}


}  // namespace NB

int main(int argc, char** argv) {
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 22:03:53
// File  : code.cc
// Brief :

/*
Given a binary tree and a sum, find all root-to-leaf paths where each path's sum equals the given sum.

For example:
Given the below binary tree and sum = 22,
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
return
[
   [5,4,11,2],
   [5,8,4,5]
]
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 43/43 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 114/114 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void PathSum(TreeNode * root, int sum, int * tsum,
             std::vector<int> & path, std::vector<std::vector<int> > & rs) {
  if (root == NULL) return;
  *tsum += root->val;
  path.push_back(root->val);
  if (root->left == NULL && root->right == NULL && *tsum == sum) {
    rs.push_back(path);
  }  // 测试数据中有负数
  PathSum(root->left, sum, tsum, path, rs);
  PathSum(root->right, sum, tsum, path, rs);
  *tsum -= root->val;
  path.pop_back();
}

std::vector<std::vector<int> > PathSum(TreeNode * root, int sum) {
  std::vector<std::vector<int> >  rs;
  if (root == NULL) return rs;
  int tsum = 0;
  std::vector<int> path;
  PathSum(root, sum, &tsum, path, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

// 非递归
// TODO
namespace NB {
struct Node {
  TreeNode * v;
  std::vector<int> path;
  Node (TreeNode * t) : v(t) {}
  void Add(Node & t, int val) {
    path = t.path;
    path.push_back(val);
  }
  int Sum() {
    int rs = 0;
    for (int i = 0; i < path.size(); i++) {
      rs += path[i];
    }
    return rs;
  }
}

std::vector<std::vector<int> > Path(TreeNode * root, int sum) {
  srd::vector<std::vector<int> > rs;
  std::vector<int> path;
  int cur = 0;
  std::stack<TreeNode *> stack;
  while (root != NULL) {
    stack.push(root);
    cur += root->val;
    path.push(root->val);
    root = root->left;
  }
  while (!stack.empty()) {
    TreeNode * t = stack.top();
    if (t->right == NULL && cur == sum) {
      rs.push_back(path);
      cur -= t->val;
      path.pop_back();
    }
    TreeNode * tmp = t->right;
    while (tmp != NULL) {
      stack.push(tmp);
      cur += tmp->val;
      tmp = tmp->left;
    }
  }
  return rs;
}

}

int main(int argc, char** argv) {
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-21 04:14:58
// File  : code.cc
// Brief :

/*
Given a binary tree, return the postorder traversal of its nodes' values.
For example:
Given binary tree {1,#,2,3},
   1
    \
     2
    /
   3
return [3,2,1].
Note: Recursive solution is trivial, could you do it iteratively?

*/

/*
 * 67 / 67 test cases passed.
 * Status: Accepted
 * Runtime: 12 ms
 * Submitted: 1 minute ago
 *
 * */

#include "base/public/common_ojhead.h"
#include "../ojbst/ojbst.h"

namespace algorithm {
struct Node {
  TreeNode * ptr;
  int flag;
  Node(TreeNode * p = NULL, int f = 0) : ptr(p), flag(f) {}
};

std::vector<int> PostOrderTraversal(TreeNode * root) {
  std::vector<int> rs;
  std::stack<Node> stack;
  while (root != NULL) {
    stack.push(Node(root, 1));
    root = root->left;
  }
  while (!stack.empty()) {
    Node & t = stack.top();
    if (t.flag == 2) {
      rs.push_back(t.ptr->val);
      stack.pop();  // 要continue
    } else {  // bug fixed, 之前要pop出去的Node，的left子树被重复加如到了stack中了
      t.flag += 1;
      TreeNode * foo = t.ptr->right;
      while (foo != NULL)  {
        stack.push(Node(foo, 1));
        foo = foo->left;
      }
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "1,#,2,3";
  TreeNode * root = MakeFromOjString(str);
  PostOrder(root);
  std::vector<int> rs = PostOrderTraversal(root);
  LOG(INFO) << JoinVector(rs);
  return 0;
}

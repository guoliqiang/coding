// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 10:50:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// 如果预处理每个节点下有多少个节点的话可以二分查找
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

TreeNode * Kth(TreeNode * root, int & cnt, int k) {
  if (root == NULL) return NULL;
  TreeNode * t = Kth(root->left, cnt, k);
  if (t != NULL) return t;
  cnt += 1;
  if (cnt == k) return root;
  return Kth(root->left, cnt, k);
}

TreeNode * Kth(TreeNode * root, int k) {
  int cnt = 0;
  return Kth(root, cnt, k);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}

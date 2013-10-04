// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 21:33:10
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

struct TreeNode {
  TreeNode(int v) : val(v), left(NULL), right(NULL) {}
  int val;
  TreeNode * left;
  TreeNode * right;
};

void Array2Tree(std::vector<int> & v, int b ,int e, TreeNode ** p) {
  if (b > e) return;
  if (b == e) {
    *p = new TreeNode(v[b]);
  } else {
    int mid = b + (e - b) / 2;
    *p = new TreeNode(v[mid]);
    Array2Tree(v, b, mid - 1, &((*p)->left));
    Array2Tree(v, mid + 1, e, &((*p)->right));
  }
}

TreeNode * Array2Tree(std::vector<int> & v) {
  TreeNode * root = NULL;
  Array2Tree(v, 0, v.size() - 1, &root);
  return root;
}

void InOrder(const TreeNode * root) {
  if (root == NULL) return;
  InOrder(root->left);
  LOG(INFO) << root->val;
  InOrder(root->right);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  InOrder(Array2Tree(foo));
  return 0;
}

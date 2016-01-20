// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 10:59:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

int DepthLeft(TreeNode * n) {
  int ans = 0;
  while (n != NULL) {
    ans++;
    n = n->left;
  }
  return ans;
}

int DepthRight(TreeNode * n) {
  int ans = 0;
  while (n != NULL) {
    ans++;
    n = n->right;
  }
  return ans;
}


int Count(TreeNode * n) {
  int dl = DepthLeft(n);
  int dr = DepthRight(n);

  if (dl == dr) {
    return pow(2, dl) - 1;
  } else {
    return 1 + Count(n->left) + Count(n->right);
  }
}

int Count2(TreeNode * root) {
  if (root == NULL) return 0;
  int ans = 0;

  while (root != NULL) {
    int l = DepthLeft(root->left);
    int r = DepthLeft(root->right);
    LOG(INFO) << r << "~" << l;
    if (r == l) {
      ans += pow(2, l);
      root = root->right;
    } else {
      ans += pow(2, r);
      root = root->left;
    }
  }
  return ans;
}


}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  TreeNode * root = new TreeNode(2);
  root->left = new TreeNode(1);
  LOG(INFO) << Count2(root);
  return 0;
}

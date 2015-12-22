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


}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 22:12:28
// File  : code.cc
// Brief :
//
// 思路一:直接中序遍历输出指定节点的下一个即可
//

#include "base/public/common_head.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode * para;
};

TreeNode * Next(TreeNode * n) {
  if (n->right != NULL) {
    TreeNode * foo = n->right;
    while (foo->left != NULL) foo = foo->left;
    return foo;
  } else {
    TreeNode * para = n->para;
    while (para != NULL) {
      if (para->left == n) return para;
      else {
        n = para;
        para = para->para;
      }
    }
    return NULL;
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

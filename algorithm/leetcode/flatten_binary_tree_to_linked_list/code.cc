// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 23:56:06
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 27/27 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 225/225 test cases passed.
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void Flatten(TreeNode * root, TreeNode * & pre, TreeNode ** v) {
  if (root == NULL) return;
  pre = root;
  TreeNode * tright = root->right;
  Flatten(root->left, pre, &(root->right));
  *v = root;
  Flatten(tright, pre, &(pre->right));
  root->left = NULL;  // delete this line can not be AC
}

TreeNode * Flatten(TreeNode * root) {
  TreeNode dummy(0);
  dummy.left = root;
  TreeNode * pre = NULL;
  Flatten(root, pre, &(dummy.right));
  return dummy.right;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "1,#,2,4,3";
  // std::string str = "1,#,2";
  TreeNode * root = Make(str);
  PreOrder(root);
  Flatten(root);
  while (root != NULL) {
    LOG(INFO) << root->val;
    root = root->right;
  }
  return 0;
}

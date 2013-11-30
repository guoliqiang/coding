// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 18:41:47
// File  : code.cc
// Brief :

/*
Given preorder and inorder traversal of a tree, construct the binary tree.
Note:
You may assume that duplicates do not exist in the tree.


*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 124 milli secs
 * Progress: 202/202 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void Build(std::vector<int> & preorder,
           std::vector<int> & inorder,
           int p_b, int p_e,
           int i_b, int i_e,
           TreeNode ** p) {

  if (p_b > p_e || i_b > i_e) return;
  int t = preorder[p_b];
  *p = new TreeNode(t);
  
  int i = i_b;
  while (inorder[i] != t) i++;
  
  int interval = i - i_b;

  Build(preorder, inorder,
        p_b + 1, p_b + interval, i_b, i_b + interval - 1, &((*p)->left));
  Build(preorder, inorder,
        p_b + interval + 1, p_e, i_b + interval + 1, i_e, &((*p)->right));
}

TreeNode * BuildTree(std::vector<int> & preorder, std::vector<int> & inorder) {
  if (preorder.size() != inorder.size()) return NULL;

  TreeNode * root = NULL;
  Build(preorder, inorder, 0, preorder.size() - 1, 0, inorder.size() - 1, &root);
  return root;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

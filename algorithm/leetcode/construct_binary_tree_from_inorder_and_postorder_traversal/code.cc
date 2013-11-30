// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 19:07:45
// File  : code.cc
// Brief :

/*
Given inorder and postorder traversal of a tree, construct the binary tree.

Note:
You may assume that duplicates do not exist in the tree.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 120 milli secs
 * Progress: 202/202 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void Build(std::vector<int> & inorder, std::vector<int> & postorder,
           int i_b, int i_e, int p_b, int p_e, TreeNode ** p) {
  if (i_b > i_e || p_b > p_e) return;
  int t = postorder[p_e];
  *p = new TreeNode(t);

  int i = i_b;
  while (inorder[i] != t) i++;
  int interval = i - i_b;

  Build(inorder, postorder,
        i_b, i_b + interval - 1, p_b, p_b + interval - 1, &((*p)->left));
  Build(inorder, postorder,
        i_b + interval + 1, i_e, p_b + interval, p_e - 1, &((*p)->right));
}

TreeNode * BuildTree(std::vector<int> & inorder, std::vector<int> & postorder) {
  TreeNode * root = NULL;
  if (inorder.size() != postorder.size()) return root;
  Build(inorder, postorder, 0, inorder.size() - 1, 0, postorder.size() - 1, &root);
  return root;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

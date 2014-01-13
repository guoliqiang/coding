// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-11 21:58:55
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode * parent;
  TreeNode(int vi) : val(vi), left(NULL), right(NULL), parent(NULL) {}
};

TreeNode * LCA(TreeNode * n1, TreeNode * n2) {
  TreeNode * cp_n1 = n1;
  int len1 = 0;
  while (cp_n1 != NULL) {
    len1 += 1;
    cp_n1 = cp_n1->parent;
  }
  TreeNode * cp_n2 = n2;
  int len2 = 0;
  while (cp_n2 != NULL) {
    len2 += 1;
    cp_n2 = cp_n2->parent;
  }
  while (len1 > len2) {
    n1 = n1->parent;
    len1--;
  }
  while (len2 > len1) {
   n2 = n2->parent;
   len2--;
  }
  while (n1 != n2) {
    n1 = n1->parent;
    n2 = n2->parent;
  }
  return n1;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}

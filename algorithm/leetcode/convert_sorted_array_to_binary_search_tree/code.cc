// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 20:07:29
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 12/12 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 100 milli secs
 * Progress: 32/32 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void SortedArrayToBST(std::vector<int> & v, int b, int e, TreeNode ** p) {
  if (b > e) return;
  int mid = b + (e - b) / 2;
  *p = new TreeNode(v[mid]);
  SortedArrayToBST(v, b, mid - 1, &((*p)->left));
  SortedArrayToBST(v, mid + 1, e, &((*p)->right));
}

TreeNode * SortedArrayToBST(std::vector<int> & v) {
  TreeNode * root = NULL;
  SortedArrayToBST(v, 0, v.size() - 1, &root);
  return root;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

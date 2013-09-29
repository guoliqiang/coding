// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 21:21:42
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 64 milli secs
 * Progress: 226/226 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

bool IsBalance(TreeNode * root, int * deepth) {
  if (root == NULL) {
    deepth = 0;
    return true;
  } else {
    int left_deep = 0;
    int right_deep = 0;
    if (!IsBalance(root->left, &left_deep)) return false;
    if (!IsBalance(root->right, &right_deep)) return false;

    *deepth = left_deep > right_deep ? left_deep + 1 : right_deep + 1;
    return abs(left_deep - right_deep ) <= 1;
  }
}

bool IsBalance(TreeNode * root) {
  int deepth = 0;
  return IsBalance(root, &deepth);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

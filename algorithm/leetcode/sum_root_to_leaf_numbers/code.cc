// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 22:12:13
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 44 milli secs
 * Progress: 109/109 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void PostOrder(TreeNode * root, std::vector<std::vector<int> > & v) {
  if (root == NULL) {
    return;
  }
  std::vector<std::vector<int> > left;
  std::vector<std::vector<int> > right;
  PostOrder(root->left, left);
  PostOrder(root->right, right);
  for (int i = 0; i < left.size(); i++) {
    left[i].insert(left[i].end(), root->val);
    v.push_back(left[i]);
  }

  for (int i = 0; i < right.size(); i++) {
    right[i].insert(right[i].end(), root->val);
    v.push_back(right[i]);
  }

  if (root->left == NULL && root->right == NULL) {
    v.push_back(std::vector<int>(1, root->val));
  }
}

int SumNumber(TreeNode * root) {
  int rs = 0;
  std::vector<std::vector<int> > v;
  PostOrder(root, v);
  for (int i = 0; i < v.size(); i++) {
    std::vector<int> & t = v[i];
    int one = 0;
    for (int i = t.size() - 1; i >= 0; i--) {
      one += t[i] * pow(10, i);
    }
    rs += one;
  }
  return rs;
}

}  // namespce algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "5,3,2,7,0,6,#,#,#,0";
  TreeNode * t = Make(str);
  InOrder(t);
  HERE(INFO);
  PreOrder(t);
  LOG(INFO) << SumNumber(t);
  LOG(INFO) << 'A' - 'a';
  return 0;
}

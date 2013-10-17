// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 22:03:53
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 43/43 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 114/114 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

void PathSum(TreeNode * root, int sum, int * tsum,
             std::vector<int> & path, std::vector<std::vector<int> > & rs) {
  if (root == NULL) return;
  *tsum += root->val;
  path.push_back(root->val);
  if (root->left == NULL && root->right == NULL && *tsum == sum) {
    rs.push_back(path);
  }  // 测试数据中有负数
  PathSum(root->left, sum, tsum, path, rs);
  PathSum(root->right, sum, tsum, path, rs);
  *tsum -= root->val;
  path.pop_back();
}

std::vector<std::vector<int> > PathSum(TreeNode * root, int sum) {
  std::vector<std::vector<int> >  rs;
  if (root == NULL) return rs;
  int tsum = 0;
  std::vector<int> path;
  PathSum(root, sum, &tsum, path, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 21:44:09
// File  : code.cc
// Brief :
//
/*Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 136 milli secs
 * Progress: 92/92 test cases passed.
 *
 *
 * 这是不是就是树形DP，树形DP由于要使用到子树的状态所以
 * 用后根遍历?
 *
 * 树形DP学习:
 * http://blog.csdn.net/woshi250hua/article/details/7644959
 * */

#include "base/public/common_head.h"
#include "../ojbst/ojbst.h"

namespace algorithm {

void PostOrder(TreeNode * root, int * max, int * rs) {
  if (root == NULL) {
    *max  = 0;
    return;
  }
  int left_max = 0;
  int right_max = 0;
  PostOrder(root->left, &left_max, rs);
  PostOrder(root->right, &right_max, rs);
  
  int tmax = std::max(left_max, right_max);
  tmax = std::max(tmax, left_max + right_max);
  tmax = std::max(tmax, 0) + root->val;
  if (tmax > *rs) *rs = tmax;

  *max = std::max(left_max, right_max);
  *max = std::max(0, *max) + root->val;
}

int MaxPathSum(TreeNode * root) {
  int rs = 0x80000000;
  int tmax = 0;
  PostOrder(root, &tmax, &rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "1,2,3";
  LOG(INFO) << MaxPathSum(Make(str));
  return 0;
}

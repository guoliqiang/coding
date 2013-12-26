// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 10:33:37
// File  : code.cc
// Brief :

#include "ojbst.h"

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "2,1,7,#,#,4,#,#,6";
  TreeNode * root = Make(str);
  
  LOG(INFO) << "in order:";
  InOrder(root);
  LOG(INFO) << "pre order:";
  PreOrder(root);
  HERE(INFO);
  TreeNode * rs = FindGE(root, 3);
  if (rs != NULL) LOG(INFO) << rs->val;
  return 0;
}

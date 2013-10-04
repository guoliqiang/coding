// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 21:57:14
// File  : code.cc
// Brief :
// 其实是bfs但没有用queue

#include "base/public/common_head.h"

namespace algorithm {
struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

void ToList(TreeNode * root, std::vector<std::list<TreeNode *> > & rs) {
  if (!root) return;
  std::list<TreeNode *> foo;
  foo.push_back(root);
  rs.push_back(foo);
  bool flag = true;
  while (flag) {
    flag = false;
    std::list<TreeNode*> t = rs.back();
    foo.clear();
    for (std::list<TreeNode *>::iterator i = t.begin(); i != t.end(); i++) {
      if ((*i)->left != NULL) foo.push_back((*i)->left);
      if ((*i)->right != NULL) foo.push_back((*i)->right);
    }
    if (foo.size() > 0) {
      flag = true;
      rs.push_back(foo);
    }
  }
}

std::vector<std::list<TreeNode *> > ToList(TreeNode * root) {
  std::vector<std::list<TreeNode *> >rs;
  ToList(root, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

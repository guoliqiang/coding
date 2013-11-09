// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 00:37:54
// File  : code.cc
// Brief :
/*
 * You are given a binary tree in which each node contains a value.
 * Design an algorithm to print all paths which sum up to that value.
 * Note that it can be any path in the tree – it does not have to start at the root.
 * */
#include "base/public/common_head.h"

namespace algorithm {

struct TreeNode {
 int val;
 TreeNode * left;
 TreeNode * right;
 TreeNode (int v = 0) : val(v), left(NULL), right(NULL) {} 
};

void DFS(TreeNode * root, int k, std::vector<std::vector<int> > & paths,
         std::vector<std::vector<int> > & rs) {
  if (root == NULL) return;
  else {
    for (int i = 0; i < paths.size(); i++) {
      int sum = root->val;
      for (int j = 0; j < paths[i].size(); j++) {
        sum += paths[i][j];
      }
      paths[i].push_back(root->val);
      if (sum == k) rs.push_back(paths[i]);
    }
    paths.push_back(std::vector<int>(1, root->val));
    if (root->val == k) rs.push_back(paths.back());
  }
  DFS(root->left, k, paths, rs);
  DFS(root->right, k, paths, rs);
  paths.pop_back();
  for (int i = 0; i < paths.size(); i++) {
     paths[i].pop_back();
  }
}

std::vector<std::vector<int> > Find(TreeNode * root, int k) {
  std::vector<std::vector<int> > rs;
  std::vector<std::vector<int> > paths;
  DFS(root, k, paths, rs);
  return rs;
}

void Build(std::vector<int> & v ,int b , int e, TreeNode ** p) {
  if (b > e) return;
  if (b == e) {
    *p = new TreeNode(v[b]);
  } else {
    int mid = b + (e - b) / 2;
    *p = new TreeNode(v[mid]);
    Build(v, b , mid - 1, &((*p)->left));
    Build(v, mid + 1, e, &((*p)->right));
  }
}

TreeNode * Make(std::vector<int> & v) {
  TreeNode * root = NULL;
  Build(v, 0, v.size() - 1, &root);
  return root;
}

}// namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  std::vector<std::vector<int> > rs = Find(Make(foo), 3);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}

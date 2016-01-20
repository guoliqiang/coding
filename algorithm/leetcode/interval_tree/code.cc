// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-14 23:23:02
// File  : code.cc
// Brief :
// http://www.geeksforgeeks.org/interval-tree/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  int b;
  int e;
  int max;
  TreeNode * left;
  TreeNode * right;
  TreeNode () : b(0), e(0), max(0), left(NULL), right(NULL) {}
};

int Insert(std::pair<int, int> v, TreeNode * & root) {
  if (root == NULL) {
    root = new TreeNode();
    root->b = v.first;
    root->e = v.second;
    root->max = v.second;
  } else {
    if (v.first <= root->b) {
      root->max = std::max(root->max, Insert(v, root->left));
    } else {
      root->max = std::max(root->max, Insert(v, root->right));
    }
  }
  return root->max;
}

void Query(int k, TreeNode * root, std::vector<TreeNode *> & ans) {
  if (root == NULL) return;
  if (k >= root->b && k <= root->e) {
    ans.push_back(root);
  }
  if (root->left != NULL && root->left->max >= k) {
    Query(k, root->left, ans);
  }
  if (k > root->b && root->right != NULL && k <= root->right->max) {
    Query(k, root->right, ans);
  }
}

std::vector<std::pair<int, int> > Merge(TreeNode * root) {
  std::vector<std::pair<int, int> > ans;
  std::stack<TreeNode *> stack;
  while (root != NULL) {
    stack.push(root);
    root = root->left;
  }
  while (stack.size()) {
    TreeNode * cur = stack.top();
    stack.pop();
    if (ans.size() > 0 && cur->b <= ans.back().second) {
      ans.back().second = cur->e;
    } else {
      ans.push_back(std::make_pair(cur->b, cur->e));
    }
    cur = cur->right;
    while (cur != NULL) {
      stack.push(cur);
      cur = cur->left;
    }
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  TreeNode * root = NULL;
  std::vector<TreeNode *> vec;
  Insert(std::make_pair(1, 3), root);
  Query(2, root, vec);
  for (int i = 0; i < vec.size(); i++) LOG(INFO) << "i = " << i << " " << vec[i]->b << " " << vec[i]->e;
  vec.clear();
  Insert(std::make_pair(1, 3), root);
  Query(2, root, vec);
  for (int i = 0; i < vec.size(); i++) LOG(INFO) << "i = " << i << " " << vec[i]->b << " " << vec[i]->e;
  vec.clear();
  Insert(std::make_pair(2, 3), root);
  Query(2, root, vec);
  for (int i = 0; i < vec.size(); i++) LOG(INFO) << "i = " << i << " " << vec[i]->b << " " << vec[i]->e;
  Insert(std::make_pair(3, 5), root);
  Insert(std::make_pair(10, 20), root);
  std::vector<std::pair<int, int> > merge = Merge(root);
  for (int i = 0; i < merge.size(); i++) {
    LOG(INFO) << merge[i].first << "~" << merge[i].second;
  }
  return 0;
}

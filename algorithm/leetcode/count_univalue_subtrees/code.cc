// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 14:28:42
// File  : code.cc
// Brief :

/*
Given a binary tree, count the number of uni-value subtrees.

A Uni-value subtree means all nodes of the subtree have the same value.

For example:
Given binary tree,
              5
             / \
            1   5
           / \   \
          5   5   5
return 4.

*/

#include "base/public/common_ojhead.h"

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
};

namespace algorithm {
std::pair<bool, int> Get(TreeNode * root, int & ans) {
  if (root == NULL) return std::make_pair(true, 0);

  if (root->left == NULL && root->right == NULL) {
    ans++;
    return std::make_pair(true, root->val);
  } else if (root->left == NULL || root->right == NULL) {
    std::pair<bool, int> t = Get(root->right == NULL ? root->left : root->right, ans);
    if (t.first == false) return t;
    else {
      if (t.second != root->val) {
        return std::make_pair(false, 0);
      } else {
        ans++;
        return t;
      }
    }
  } else {
    std::pair<bool, int> l = Get(root->left, ans);
    std::pair<bool, int> r = Get(root->right, ans);
    if (l.first == true && r.first == true &&
        l.second == r.second && l.second == root->val) {
      ans++;
    }

    if (l.first == false || r.second == false ||
        l.second != root->val || r.second != root->val) {
      return std::make_pair(false, 0);
    } else {
      return l;
    }
  }
}

int CountUnivalSubtrees(TreeNode * root) {
  int ans = 0;
  Get(root, ans);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;
namespace NB {
bool Get(TreeNode * root, int & ans) {
  if (root == NULL) return true;
  if (root->left == NULL && root->right == NULL) {
    ans++;
    return true;
  } else if (root->left == NULL || root->right == NULL){
    TreeNode * t = root->left == NULL ? root->right : root->left;
    if (Get(t, ans) && root->val == t->val) {
      ans++;
      return true;
    }
  } else {
    if (Get(root->left, ans) && Get(root->right, ans) &&
        root->left->val == root->val && root->right->val == root->val) {
      ans++;
      return true;
    }
  }
  return false;
}

int CountUnivalSubtrees(TreeNode * root) {
  int ans = 0;
  Get(root, ans);
  return ans;
}
}  // namespace NB

namespace NB2 {
bool Get(TreeNode * root, int val, int & ans) {
  if (root == NULL) return true;
  if (!Get(root->left, root->val, ans) || Get(root->right, root->val, ans)) {
    return false;
  }
  ans++;
  return root->val == val;
}
}  // namespace NB2
int main(int argc, char** argv) {
  return 0;
}


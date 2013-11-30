// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 03:35:24
// File  : code.cc
// Brief :

/*
Given a binary tree, determine if it is a valid binary search tree (BST).

Assume a BST is defined as follows:

The left subtree of a node contains only nodes with keys less than the node's key.
The right subtree of a node contains only nodes with keys greater than the node's key.
Both the left and right subtrees must also be binary search trees.
confused what "{1,#,2,3}" means? > read more on how binary tree is serialized on OJ.


OJ's Binary Tree Serialization:
The serialization of a binary tree follows a level order traversal, where '#' signifies a path terminator where no node exists below.

Here's an example:
   1
  / \
 2   3
    /
   4
    \
     5
The above binary tree is serialized as "{1,2,3,#,#,4,#,#,5}".

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 54/54 test cases passed.
 *
 * */

#include "base/public/common_head.h"

// 此题写的不熟

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

namespace algorithm {
bool IsValidBST(TreeNode * root, int * & max, int * & min) { // bug fix note use reference
  if (root == NULL) {
    max = NULL;
    min = NULL;
    return true;
  } else {
    int left_max = 0;
    int * p_left_max = &(left_max);
        
    int right_max = 0;
    int * p_right_max = &(right_max);
        
    int left_min = 0;
    int * p_left_min = &(left_min);
        
    int right_min = 0;
    int * p_right_min = &(right_min);
        
    if (!IsValidBST(root->left, p_left_max, p_left_min)) return false;
    if (!(IsValidBST(root->right, p_right_max, p_right_min))) return false;
        
    if (p_left_max != NULL && *p_left_max >= root->val) return false;
    if (p_right_min != NULL && *p_right_min <= root->val) return false;
        
    *max = (p_right_max != NULL) ? *p_right_max : root->val;
    *min = (p_left_min != NULL) ? *p_left_min : root->val;
    return true;
 }
}

bool IsValidBST(TreeNode * root) {
    int max;
    int *p_max = &max;
    int min;
    int *p_min = &min;
    return IsValidBST(root, p_max, p_min);
}

class Solution {
 public:
  bool isValidBST(TreeNode *root) {
    return IsValidBST(root);
  }
};
}  // namespace algorithm

namespace twice {
// 这种方法是错误的
bool IsValidBST(TreeNode * root) {
  if (root == NULL) return true;
  if (!IsValidBST(root->left)) return false;
  if (!IsValidBST(root->right)) return false;
  if (root->left != NULL && root->left->val >= root->val) return false;
  if (root->right != NULL && root->right->val <= root->val) return false;
  return true;
}

// 基于线索二叉树，注意最后返回，因为中间返回时那颗二叉树
// 还没有被还原
class Solution {
 public:
  bool isValidBST(TreeNode *root) {
    if (root == NULL) return true;
    TreeNode * pre = NULL;
    bool rs = true;
    while (root != NULL) {
      if (root->left == NULL) {
        if (pre != NULL && pre->val >= root->val) {
          rs = false;
          // 不能return false
        }
        pre = root;
        root = root->right;
      } else {
        TreeNode * t = root->left;
        while (t->right != NULL && t->right != root) t = t->right;
          if (t->right == NULL) {
            t->right = root;
            root = root->left;
          } else {
            if (pre != NULL && pre->val >= root->val) {
              rs = false;
            }
            pre = root;
            t->right = NULL;
            root = root->right;
          }
        }
      }
      return rs;
  }
};
}  // namespace twice

namespace third {
bool Validate(TreeNode * root, int & max, int & min) {
  int rmax, rmin;
  int lmax, lmin;
  if (root->left != NULL) {
    if (!Validate(root->left, lmax, lmin)) return false;
    if (lmax >= root->val) return false;
    else min = lmin;
  } else min = root->val;

  if (root->right != NULL) {
    if (!Validate(root->right, rmax, rmin)) return false;
    if (rmin <= root->val) return false;
    else max = rmax;
  } else max = root->val;

  return true;
}

bool Validate(TreeNode * root) {
  if (root == NULL) return true;
  int max, min;
  return Validate(root, max, min);
}
}  // namespace third


int main(int argc, char** argv) {
  return 0;
}

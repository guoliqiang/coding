// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 03:35:24
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 54/54 test cases passed.
 *
 * */

// 此题写的不熟

/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
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
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        return IsValidBST(root);
    }
};


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
        // Note: The Solution object is instantiated only once and is reused by each test case.
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

}

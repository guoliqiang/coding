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

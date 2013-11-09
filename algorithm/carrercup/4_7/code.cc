// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 13:10:21
// File  : code.cc
// Brief :

/*
 * You have two very large binary trees: T1, with millions of nodes, and T2, with hundreds of nodes.
 * Create an algorithm to decide if T2 is a subtree of T1
 * 
 * http://hawstein.com/posts/4.7.html
 * 答案是暴力搜索，但我认为可以利用数的高度缩小以下搜索范围
 * 因为树多一层，节点最多可以增加上一层节点数的两倍，缩小一层
 * 可以省去好多操作
 *
 * 评论中有网友提出用hash来做。
 *
 * 另外一种思考方式：
 * 分别输出两个树的前序和中序，然后用KMP或BM，在字符串中查找字符串
 * 时间复杂度 O(m + n)
 * 但需要额外的存储空间
 * 此题目不可以，因为题目中明确说节点数量很大，额外空间消耗会很多.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode * left;
  TreeNode * right;
  TreeNode(int v = 0) : val(v), left(NULL), right(NULL) {}
};

int Deepth(TreeNode * root) {
  if (root == NULL) return 0;
  int l = Deepth(root->left);
  int r = Deepth(root->right);
  return std::max(l, r) + 1;
}

bool IsMatch(TreeNode * root1, TreeNode * root2) {
  if (root1 == NULL && root2 == NULL) return true;
  if (root1 == NULL || root2 == NULL) return false;
  if (root1->val != root2->val) return false;
  if (!IsMatch(root1->left, root2->left)) return false;
  if (!IsMatch(root1->right, root2->right)) return false;
  return true;
}

bool Contain(TreeNode * root, TreeNode * c, int cdeep, int * deep) {
  if (root == NULL) {
    *deep = 0;
    if (cdeep == 0) return IsMatch(root, c);
    else return false;
  } else {
    int tldeep = 0;
    int trdeep = 0;
    if (Contain(root->left, c, cdeep, &tldeep)) return true;
    if (Contain(root->right, c, cdeep, &trdeep)) return true;
    *deep = std::max(tldeep, trdeep) + 1;
    if (*deep >= cdeep && IsMatch(root, c)) return true;
    return false;
  }
}

bool Contain(TreeNode * root, TreeNode * c) {
  int cdeep = Deepth(c);
  int deep = 0;
  return Contain(root, c, cdeep, &deep);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 21:35:52
// File  : code.cc
// Brief :

/*
Given a binary tree, find its minimum depth.
The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 60 milli secs
 * Progress: 41/41 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

int MinDeep(TreeNode * root) {
  if (root == NULL) return 0;
  if (root->left == NULL && root->right == NULL ) return 1;

  int min = -1;
  if (root->left != NULL) {
    min = MinDeep(root->left);
  }
  if (root->right != NULL) {
    int t = MinDeep(root->right);
    min = (min == -1) ? t : std::min(t, min);
  }

  return min + 1;
}

}  // namespace algorithm


namespace twice {
int MinDeep2(TreeNode * root) {
  if (root == NULL) return 0;
  int min = 0;
  if (root->left != NULL) {
    min = MinDeep2(root->left);
  }
  if (root->right != NULL) {
    int t = MinDeep2(root->right);
    min = (min == 0) ? t : std::min(t, min);
  }
  return min + 1;
}
}  // namespace twice


// 相对于递归方式效率更高，因为遇到第一个leaf节点就返回了
// http://www.careercup.com/question?id=4476686
namespace third {
int MinDepth(TreeNode * root) {
  if (root == NULL) return 0;
  std::queue<TreeNode *> queue;
  queue.push(root);
  queue.push(NULL);
  int rs = 1;
  while (!queue.empty()) {
    TreeNode * t = queue.front();
    queue.pop();
    if (t == NULL) {
      rs++;
      queue.push(NULL);
    } else {
      if (t->left == NULL && t->right == NULL) break;
      if (t->left != NULL) queue.push(t->left);
      if (t->right != NULL) queue.push(t->right);
    }
  }
  return rs;
}
}  // namespace third


using namespace algorithm;

int main(int argc, char** argv) {

  return 0;
}

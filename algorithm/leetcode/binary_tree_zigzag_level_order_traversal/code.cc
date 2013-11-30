// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 17:51:14
// File  : code.cc
// Brief :

/*
Given a binary tree, return the zigzag level order traversal of its nodes' values. 
(ie, from left to right, then right to left for the next level and alternate between).

For example:
Given binary tree {3,9,20,#,#,15,7},
    3
   / \
  9  20
    /  \
   15   7
return its zigzag level order traversal as:
[
  [3],
  [20,9],
  [15,7]
]
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
 * Program Runtime: 8 milli secs
 * Progress: 11/11 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 32 milli secs
 * Progress: 33/33 test cases passed.
 *
 * */

#include "../ojbst/ojbst.h"

namespace algorithm {

std::vector<std::vector<int> > ZigzagLevelOrder(TreeNode * root) {
  std::vector<std::vector<int> > rs;
  if (root == NULL) return rs;

  bool flag = false;  // true from left to right
  std::queue<TreeNode *> queue;
  queue.push(root);
  queue.push(NULL);
  std::vector<int> foo;
  while (!queue.empty()) {
    TreeNode * t = queue.front();
    queue.pop();
    if (t == NULL) {
      if (flag) {
        std::vector<int> tmp(foo.rbegin(), foo.rend());
        foo = tmp;
      }
      rs.push_back(foo);
      foo.clear();
      flag = !flag;
      if (queue.empty()) break;
      queue.push(NULL);
    } else {
      foo.push_back(t->val);
      if (t->left != NULL) queue.push(t->left);
      if (t->right != NULL) queue.push(t->right);
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

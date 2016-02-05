// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 23:56:06
// File  : code.cc
// Brief :

/*
Given a binary tree, flatten it to a linked list in-place.

For example,
Given

         1
        / \
       2   5
      / \   \
     3   4   6
The flattened tree should look like:
   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6
click to show hints.

Hints:
If you notice carefully in the flattened tree, each node's right child points to the next node of a pre-order traversal.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 27/27 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 225/225 test cases passed.
 * */

#include "../ojbst/ojbst.h"

// 这个方法当初是如何想到的？？
namespace algorithm {

// pre 保存前序遍历的最后一个节点
void Flatten(TreeNode * root, TreeNode * & pre, TreeNode ** v) {
  if (root == NULL) return;
  pre = root;
  TreeNode * tright = root->right;
  Flatten(root->left, pre, &(root->right));
  *v = root;
  Flatten(tright, pre, &(pre->right));
  root->left = NULL;  // delete this line can not be AC
}

TreeNode * Flatten(TreeNode * root) {
  TreeNode dummy(0);
  dummy.left = root;
  TreeNode * pre = NULL;
  Flatten(root, pre, &(dummy.right));
  return dummy.right;
}

}  // namespace algorithm

// 转换成中序链表形式
namespace algorithm {
void ToSortedList(TreeNode * root, TreeNode * & pre, TreeNode * & rs) {
  if (root == NULL) return;
  ToSortedList(root->left, pre, rs);
  if (pre != NULL) {
    pre->left = NULL;
    pre->right = root;
  }
  pre = root;
  if (rs == NULL) rs = pre;
  ToSortedList(root->right, pre, rs);
}

TreeNode * ToSortedList(TreeNode * root) {
  TreeNode * pre = NULL;
  TreeNode * rs = NULL;
  ToSortedList(root, pre, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

// 这个更精简
namespace twice {
void Flatten(TreeNode * root, TreeNode * & pre) {  // pre must be reference
  if (root == NULL) return;
  if (pre != NULL) {
    pre->right = root;
    pre->left = NULL;
  }
  pre = root;
  TreeNode * right = root->right; // right 必须备份，否在起值会在left递归中被覆盖
  Flatten(root->left, pre);
  Flatten(right, pre);
}

TreeNode * Flatten(TreeNode * root) {
  TreeNode * pre = NULL;
  Flatten(root, pre);
  return root;
}
}  // namespace twice

namespace itera {
TreeNode * Flatten(TreeNode * root) {
  while (root != NULL) {
    if (root->left) {
      TreeNode * cur = root->left;
      while (cur->right != NULL) cur = cur->right;
      cur->right = root->right;
      root->right = root->left;
      root->left = NULL;
    }
    root = root->right;
  }
}
}  // namespace itera

namespace iter_clear1 {
void flatten(TreeNode* root) {
  std::stack<std::pair<TreeNode *, TreeNode *> > stack;
  TreeNode * pre = NULL;
  while (root != NULL) {
      if (pre != NULL) {
          pre->left = NULL;
          pre->right = root;
      }
      stack.push(std::make_pair(root, root->right));
      pre = root;
      root = root->left;
  }

  while (stack.size()) {
      std::pair<TreeNode *, TreeNode *> cur = stack.top();
      stack.pop();

      root = cur.second;
      while (root != NULL) {
          if (pre != NULL) {
              pre->right = root;
              pre->left = NULL;
          }
          stack.push(std::make_pair(root, root->right));
          pre = root;
          root = root->left;
      }
  }
}
}  // namespace iter_clear1

namespace iter_clear2 {
void flatten(TreeNode* root) {
  std::stack<TreeNode *> stack;
  TreeNode * pre = NULL;
  while (root != NULL) {
      if (pre != NULL) {
          pre->left = NULL;
          pre->right = root;
      }
      stack.push(root->right);
      pre = root;
      root = root->left;
  }

  while (stack.size()) {
      root = stack.top();
      stack.pop();

      while (root != NULL) {
          if (pre != NULL) {
              pre->right = root;
              pre->left = NULL;
          }
          stack.push(root->right);
          pre = root;
          root = root->left;
      }
  }
}
}  // namespace iter_clear2


int main(int argc, char** argv) {
  std::string str = "1,#,2,4,3,5,6";
  // std::string str = "1,#,2";
  TreeNode * root = Make(str);
  InOrder(root);
  // Flatten(root);
  root = ToSortedList(root);
  while (root != NULL) {
    LOG(INFO) << root->val;
    root = root->right;
  }
  return 0;
}

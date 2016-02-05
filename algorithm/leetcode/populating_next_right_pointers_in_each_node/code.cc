// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 11:14:18
// File  : code.cc
// Brief :

/*
Given a binary tree

    struct TreeLinkNode {
      TreeLinkNode *left;
      TreeLinkNode *right;
      TreeLinkNode *next;
    }
Populate each next pointer to point to its next right node. If there is no next right node, the next pointer should be set to NULL.

Initially, all next pointers are set to NULL.

Note:

You may only use constant extra space.
You may assume that it is a perfect binary tree (ie, all leaves are at the same level, and every parent has two children).
For example,
Given the following perfect binary tree,
         1
       /  \
      2    3
     / \  / \
    4  5  6  7
After calling your function, the tree should look like:
         1 -> NULL
       /  \
      2 -> 3 -> NULL
     / \  / \
    4->5->6->7 -> NULL

*/

/* Connect
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 124 milli secs
 * Progress: 14/14 test cases passed.
 *
 * */

/* ConnectNew
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 108 milli secs
 * Progress: 14/14 test cases passed.
 * */

#include "base/public/common_head.h"
#include "../ojbst/ojbst.h"


struct TreeLinkNode {
  int val;
  TreeLinkNode *left, *right, *next;
  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
};


namespace NB {

using namespace algorithm;

// http://discuss.leetcode.com/questions/7/populating-next-right-pointers-in-each-node

void Connect(TreeLinkNode * root) {
  if (root == NULL || (root->left == NULL && root->right == NULL)) return;
  root->left->next = root->right->next;
  if (root->next != NULL) {
    root->right->next = root->next->left;
  }
  Connect(root->left);
  Connect(root->right);
}

// 一行一行处理

void Connect2(TreeLinkNode * root) {
  while (root != NULL) {
    if (root->left == NULL && root->right == NULL) break;
    TreeLinkNode * foo = root;
    while (foo != NULL) {
      LOG(INFO) << "here " << foo->val;
      foo->left->next = foo->right;
      if (foo->next != NULL) foo->right->next = foo->next->left;
      foo = foo->next;
    }
    root = root->left;
  }
}

void ConnectBest(TreeLinkNode * root) {
  while (root && root->left) {
    TreeLinkNode * pre = NULL;
    TreeLinkNode * foo = root;
    while (foo) {
      foo->left->next = foo->right;
      if (pre != NULL) pre->right->next = foo->left;
      pre = foo;
      foo = foo->next;
    }
    root = root->left;
  }
}
}  // namespace NB

namespace algorithm {

/*  
 *  pre始终指向中根遍历中节点的right
 *  中序遍历没有使用到栈,O(1)的空间复杂度
 * */

void InOrder(TreeLinkNode * root, TreeLinkNode * & pre) {
  if (root == NULL) return;
  while (root != NULL) {
    if (root->right == NULL) {
      root->next = pre;
      pre = root->right;
      root = root->left;
    } else {
      TreeLinkNode * foo = root->right;
      while (foo->left != NULL && foo->left != root) {
        foo = foo->left;
      }
      if (foo->left == NULL) {
        foo->left = root;
        root->next = pre;
        root = root->right;
        if (pre != NULL) pre = pre->left;
      } else {
        pre = root->right;
        foo->left = NULL;
        root = root->left;
      }
    }
  }
}



void ConnectNew(TreeLinkNode * root) {
  TreeLinkNode * pre = NULL;
  InOrder(root, pre);
}

/*
 * 使用到了queue，O(n)的空间复杂度
 * */
void Connect(TreeLinkNode * root) {
  if (root == NULL) return;
  std::queue<TreeLinkNode *> queue;
  queue.push(root);
  queue.push(NULL);
  TreeLinkNode * pre = NULL;
  while (!queue.empty()) {
    TreeLinkNode * t = queue.front();
    queue.pop();
    if (t == NULL) {
      if (queue.empty()) break;
      else {
        pre = NULL;
        queue.push(NULL);
      }
    } else {
      t->next = pre;
      pre = t;
      if (t->right != NULL) queue.push(t->right);
      if (t->left != NULL) queue.push(t->left);
    }
  }
}

void SpecialInOrder(TreeLinkNode * root) {
  if (root == NULL) return;
  SpecialInOrder(root->left);
  if (root->next != NULL) {
    LOG(INFO) << root->val << " " << root->next << " " << root->next->val;
  } else {
    LOG(INFO) << root->val << " " << root->next;
  }
  SpecialInOrder(root->right);
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
void Connect(TreeLinkNode * root) {
  while (root != NULL) {
    TreeLinkNode * pre = NULL;
    TreeLinkNode * next = NULL;
    TreeLinkNode * foo = root;
    while (foo != NULL) {
      if (foo->left != NULL) {
        if (pre != NULL) pre->next = foo->left;
        pre = foo->left;
        if (next == NULL) next = pre;
      }
      if (foo->right != NULL) {
        if (pre != NULL) pre->next = foo->right;
        pre = foo->right;
        if (next == NULL) next = pre;
      }
      foo = foo->next;
    }
    root = next;
  }
}
}  // namespace twice

namespace clear {
void connect(TreeLinkNode *root) {
    while (root != NULL && root->left != NULL) {
      TreeLinkNode * next = root->left;
      TreeLinkNode * pre = NULL;

      while (root != NULL) {
          root->left->next = root->right;
          if (pre != NULL) {
              pre->right->next = root->left;
          }
          pre = root;
          root = root->next;
      }
      root = next;
    }
}
}  // namespace clear


int main(int argc, char** argv) {
  std::string str = "0,1,2,3,4,#,#,#,#,5,6";
  // std::string str = "0,1,2,3,4,5,6";
  TreeLinkNode * t = Make<TreeLinkNode>(str);
  LOG(INFO) << t->val;
  InOrder(t);
  HERE(INFO);
  PreOrder(t);
  // ConnectNew(t);
  NB::Connect2(t);
  HERE(INFO);
  SpecialInOrder(t);
  return 0;
}

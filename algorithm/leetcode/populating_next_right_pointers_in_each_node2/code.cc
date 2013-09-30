// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 11:14:18
// File  : code.cc
// Brief :

#include "base/public/common_head.h"
#include "../ojbst/ojbst.h"

namespace algorithm {

struct TreeLinkNode {
  int val;
  TreeLinkNode *left, *right, *next;
  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
};


/*
 * 调不通了 bug
 * oj对树的定义不明确
 * {1,2,3,4,5,#,6,7,#,#,#,#,8}  {1,#,2,3,#,4,5,6,#,7,#} {1,#,2,3,#,4,5,6,#,7,8,#}
 * */

void Connect2(TreeLinkNode * root) {
  while (root != NULL) {
    // LOG(INFO) << "root " << root->val;
    if (root->left == NULL && root->right == NULL) return;
    TreeLinkNode * foo = root;
    TreeLinkNode * next = NULL;
    while (foo != NULL) {
      if (foo->left == NULL && foo->right == NULL) {
        foo = foo->next;
        continue;
      }
      if (foo->left != NULL && foo->right != NULL) {
        if ((foo->left->left != NULL || foo->left->right != NULL) && next == NULL)
          next = foo->left;
        if ((foo->right->left != NULL || foo->right->right != NULL) && next == NULL)
          next = foo->right;
        
        foo->left->next = foo->right;
        if (foo->next != NULL)
          foo->right->next = foo->next->left == NULL ? foo->next->right : foo->next->left;
      }

      if (foo->left == NULL) {
        if ((foo->right->left != NULL || foo->right->right != NULL) && next == NULL)
          next = foo->right;
        
        if (foo->next != NULL)
          foo->right->next = foo->next->left == NULL ? foo->next->right : foo->next->left;
      }

      if (foo->right == NULL) {
        if ((foo->left->left != NULL || foo->left->right != NULL) && next == NULL)
          next = foo->left;
        
        if (foo->next != NULL)
          foo->left->next = foo->next->left == NULL ? foo->next->right : foo->next->left;
      }

      foo = foo->next;
    }
    root = next;
  }
}

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 19/19 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 156 milli secs
 * Progress: 61/61 test cases passed.
 * */

void ConnectNB(TreeLinkNode * root) {
  while (root!= NULL) {
    TreeLinkNode * pre = NULL;
    TreeLinkNode * foo = root;
    TreeLinkNode * next = NULL;
    while (foo) {
      if (next == NULL) next = foo->left != NULL ? foo->left : foo->right;
      if (foo->left != NULL) {
        if (pre != NULL) pre->next = foo->left;
        pre = foo->left;
      }
      if (foo->right != NULL) {
        if (pre != NULL) pre->next = foo->right;
        pre = foo->right;
      }
      foo = foo->next;
    }
    root = next;
  }
}


void SpecialInOrder(TreeLinkNode * root) {
  if (root == NULL) return;
  SpecialInOrder(root->left);
  if (root->next != NULL) {
    LOG(INFO) << root << " " << root->val << " " << root->next << " " << root->next->val;
  } else {
    LOG(INFO) << root << " " << root->val << " " << root->next;
  }
  SpecialInOrder(root->right);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  // std::string str = "1,2,3,4,#,#,#,#,5";  // 先根遍历,#代表NULL
  std::string str = "0,2,4,1,#,3,9,5,1,#,6,#,8";  // 先根遍历,#代表NULL
  TreeLinkNode * t = Make<TreeLinkNode>(str);
  LOG(INFO) << t->val;
  InOrder(t);
  HERE(INFO);
  PreOrder(t);
  ConnectNB(t);
  HERE(INFO);
  SpecialInOrder(t);
  return 0;
}

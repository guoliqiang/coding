// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 17:16:40
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 67/67 test cases passed.
 *
 * TODO preorder postorder
 * */

#include "base/public/common_head.h"
#include "../ojbst/ojbst.h"

namespace algorithm {

std::vector<int> InorderIterator(TreeNode * root) {
  std::vector<int> rs;
  std::stack<TreeNode*> stack;
  TreeNode * tmp = root;
  while (tmp != NULL) {
    stack.push(tmp);
    tmp = tmp->left;
  }
  while (!stack.empty()) {
    tmp = stack.top();
    stack.pop();
    rs.push_back(tmp->val);
    tmp = tmp->right;
    while (tmp != NULL) {
      stack.push(tmp);
      tmp = tmp->left;
    }
  }
  return rs;
}

std::vector<int> PreOrderIterator(TreeNode * root) {
  std::vector<int> rs;
  std::stack<TreeNode*> stack;
  while (root != NULL) {
    rs.push_back(root->val);
    stack.push(root);
    root = root->left;
  }
  while (!stack.empty()) {
    TreeNode * t = stack.top();
    stack.pop();
    t = t->right;
    while (t != NULL) {
      rs.push_back(t->val);
      stack.push(t);
      t = t->left;
    }
  }
  return rs;
}

std::vector<int> PostOrderIterator(TreeNode * root) {
  std::vector<int> rs;
  std::stack<TreeNode*> stack;
  std::set<TreeNode *> visited;
  while (root != NULL) {
    stack.push(root);
    root = root->left;
  }
  while (!stack.empty()) {
    TreeNode * t = stack.top();
    stack.pop();
    if (visited.count(t)) {
      rs.push_back(t->val);
    } else {
      visited.insert(t);
      stack.push(t);
      t = t->right;
      while (t != NULL) {
        stack.push(t);
        t = t->left;
      }
    }
  }
  return rs;
}

}  // namespace algorithm


// http://www.cnblogs.com/AnnieKim/archive/2013/06/15/MorrisTraversal.html
// no recursive and without stack!!!  NB
// threading binary tree (线索二叉树)

namespace morris_traversal {
using namespace algorithm;

// 遍历左子树前，先让左子树中序遍历的最后
// 一个节点（一定是叶子节点）的右指针指向自己
// 这样在遍历结束左子树后可以回到当前节点

void InOrder(TreeNode * root) {
  if (root == NULL) return;
  
  while (root != NULL) {
    if (root->left == NULL) {
      LOG(INFO) << root->val;
      root = root->right;
    } else {
      TreeNode * foo = root->left;
      while (foo->right != NULL && foo->right != root) {
        foo = foo->right;
      }
      if (foo->right == NULL) {
        foo->right = root;
        root = root->left;
      } else {
        foo->right = NULL;
        LOG(INFO) << root->val;
        root = root->right;
      }
    }
  }
}

void PreOrder(TreeNode * root) {
  if (root == NULL) return;
  
  while (root != NULL) {
    if (root->left == NULL) {
      LOG(INFO) << root->val;
      root = root->right;
    } else {
      TreeNode * foo = root->left;
      while (foo->right != NULL && foo->right != root) {
        foo = foo->right;
      }
      if (foo->right == NULL) {
        foo->right = root;
        LOG(INFO) << root->val;
        root = root->left;
      } else {
        foo->right = NULL;
        root = root->right;
      }
    }
  }
}

/*

     dummy
     a
   b   c

先输出b
进而输出c a

*/

void PostOrderSub(TreeNode * from, TreeNode * to) {
  if (from == to) {
    LOG(INFO) << from->val;
  } else {
    TreeNode * bak = to->right;

    //将最右侧的treenode的right都指向其parent
    TreeNode * pre = from;
    TreeNode * foo = from->right;
    while (foo != NULL) {
      if (foo == to) {
        foo->right = pre;
        break;
      }
      TreeNode * tmp = foo->right;
      foo->right = pre;
      pre = foo;
      foo = tmp;
    }

    // visit
    foo = to;
    while (foo != NULL) {
      if (foo == from) {
        LOG(INFO) << foo->val;
        break;
      }
      LOG(INFO) << foo->val;
      foo = foo->right;
    }

    // 修改回去
    pre = to;
    foo = to->right;
    while (foo != NULL) {
      if (foo == from) {
        foo->right = pre;
        break;
      }
      TreeNode * tmp = foo->right;
      foo->right = pre;
      pre = foo;
      foo = tmp;
    }

    to->right = bak;

  }
}

void PostOrder(TreeNode * root) {
  if (root == NULL) return;
  // 因为算法只输出左子树，所以需要将整个二叉树
  // 作为一个虚拟节点的左子树
  TreeNode dummy;
  dummy.left = root;
  root = &dummy;

  while (root != NULL) {
    if (root ->left == NULL) {
      root = root->right;
    } else {
      TreeNode * foo = root->left;
      while (foo->right != NULL && foo->right != root) {
        foo = foo->right;
      }
      if (foo->right == NULL) {
        foo->right = root;
        root = root->left;
      } else {
        PostOrderSub(root->left, foo);
        foo->right = NULL;
        root = root->right;
      }
    }
  }
}

}  // namespace morris_trversal

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "1,2,3,#,#,4,5";
  morris_traversal::InOrder(Make(str));
  HERE(INFO);
  morris_traversal::PreOrder(Make(str));
  HERE(INFO);
  morris_traversal::PostOrder(Make(str));
  InOrder(Make(str));
  HERE(INFO);
  PostOrder(Make(str));
  HERE(INFO);
  std::vector<int> rs = PostOrderIterator(Make(str));
  LOG(INFO) << JoinVector(rs);
  return 0;
}

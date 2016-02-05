// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 11:26:20
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"


namespace algorithm {
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class BSTIterator {
 public:
  BSTIterator(TreeNode *root) {
    while (root != NULL) {
      stack_.push(root);
      root = root->left;
    }
  }

  // @return whether we have a next smallest number
  bool hasNext() {
    return stack_.size() > 0;
  }

  // @return the next smallest number
  int next() {
    TreeNode * cur = stack_.top();
    stack_.pop();

    TreeNode * cur_right = cur->right;
    while (cur_right != NULL) {
      stack_.push(cur_right);
      cur_right = cur_right->left;
    }
    return cur->val;
  }

 private:
  std::stack<TreeNode *> stack_;
};

}  // namespace algorithm


namespace extend {
struct TreeNode {
  int type;
  std::string val;
  std::vector<TreeNode *> child;
};

void PreOrder(TreeNode * root, std::string & ans) {
  if (root == NULL) return;
  if (root->type == 0) {
    ans.append(root->val);
    return;
  }
  for (int i = 0; i < root->child.size(); i++) {
    PreOrder(root->child[i], ans);
  }
}

std::string PreOrder(TreeNode * root) {
  std::string ans;
  std::stack<std::pair<TreeNode *, int> > stack;
  while (true && root != NULL) {
    stack.push(std::make_pair(root, 1));
    if (root->child.size() == 0) break;
    root = root->child[0];
  }

  while (stack.size()) {
    std::pair<TreeNode *, int> cur = stack.top();
    stack.pop();
    if (cur.first->type == 0) ans.append(cur.first->val);
    if (cur.second >= cur.first->child.size()) continue;

    root = cur.first->child[cur.second];
    cur.second += 1;
    stack.push(cur);

    while (true) {
      stack.push(std::make_pair(root, 1));
      if (root->child.size() == 0) break;
      root = root->child[0];
    }
  }
  return ans;
}

class DOMIterator {
 public:
  DOMIterator(TreeNode *root) {
    while (true && root != NULL) {
      stack.push(std::make_pair(root, 1));
      if (root->child.size() == 0) break;
      root = root->child[0];
    }
  }

  // @return the next smallest number
  TreeNode * next() {
    while (stack.size()) {
      std::pair<TreeNode *, int> cur = stack.top();
      stack.pop();
      if (cur.first->type == 0) return cur.first;
      if (cur.second >= cur.first->child.size()) continue;

      TreeNode * root = cur.first->child[cur.second];
      cur.second += 1;
      stack.push(cur);

      while (true) {
        stack.push(std::make_pair(root, 1));
        if (root->child.size() == 0) break;
        root = root->child[0];
      }
    }
    return NULL;
  }

 private:
  std::stack<std::pair<TreeNode *, int> > stack;
};
}  // namespace extend


using namespace extend;
int main(int argc, char** argv) {
  TreeNode * root = new TreeNode();
  root->type = 1;
  root->child.push_back(new TreeNode());
  TreeNode * foo = root->child[0];
  foo->type = 1;

  root->child.push_back(new TreeNode());
  root->child.back()->type = 0;
  root->child.back()->val = "hello";

  root->child.push_back(new TreeNode());
  root->child.back()->type = 0;
  root->child.back()->val = "word";
  for (int i = 0; i < 5; i++) {
    foo->child.push_back(new TreeNode());
    foo->child.back()->type = 0;
    foo->child.back()->val = IntToString(i);
  }
  std::string ans;
  PreOrder(root, ans);
  LOG(INFO) << ans;

  LOG(INFO) << PreOrder(root);

  DOMIterator bar(root);
  while (true) {
    TreeNode * t = bar.next();
    if (t == NULL) break;
    LOG(INFO) << "in iter:" << t->val;
  }
  return 0;
}

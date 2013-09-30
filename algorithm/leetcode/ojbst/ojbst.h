// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 10:08:12
// File  : ojbst.h
// Brief :

#ifndef  __OJBST_H_
#define  __OJBST_H_

#include "base/public/common_head.h"

namespace algorithm {

struct TreeNode {
 int val;
 TreeNode * left;
 TreeNode * right;
 TreeNode(int x = 0): val(x), left(NULL), right(NULL) {}
};

//{1,2,3,#,#,4,#,#,5}
void MakeFromOjString(std::string & str, TreeNode * p) {
  if (str.size() == 0) {
    return;
  }
  std::string left = (str.find(',') != std::string::npos) ? str.substr(0, str.find_first_of(',')) : str;
  str = (left == str) ? "" : str.substr(str.find_first_of(',') + 1);
  std::string right = (str.find(',') != std::string::npos) ?  str.substr(0, str.find_first_of(',')) : str;
  str = (right == str) ? "" : str.substr(str.find_first_of(',') + 1);
  LOG(INFO) << "left:" << left << " right:" << right << " root:" << p->val;
  if (left != "#" && !left.empty()) {
    p->left = new TreeNode(left[0] - '0');
    MakeFromOjString(str, p->left);
  }
  if (right != "#" && !right.empty()) {
    p->right = new TreeNode(right[0] - '0');
    MakeFromOjString(str, p->right);
  }
}

//{1,2,3,#,#,4,#,#,5}
template <typename T>
void MakeFromOjString(std::string & str, T * p) {
  if (str.size() == 0) {
    return;
  }
  std::string left = (str.find(',') != std::string::npos) ? str.substr(0, str.find_first_of(',')) : str;
  str = (left == str) ? "" : str.substr(str.find_first_of(',') + 1);
  std::string right = (str.find(',') != std::string::npos) ?  str.substr(0, str.find_first_of(',')) : str;
  str = (right == str) ? "" : str.substr(str.find_first_of(',') + 1);
  LOG(INFO) << "left:" << left << " right:" << right << " root:" << p->val;
  if (left != "#" && !left.empty()) {
    p->left = new T(left[0] - '0');
    MakeFromOjString<T>(str, p->left);
  }
  if (right != "#" && !right.empty()) {
    p->right = new T(right[0] - '0');
    MakeFromOjString<T>(str, p->right);
  }
}

TreeNode * MakeFromOjString(std::string str) {
  std::string foo = str.substr(0, str.find_first_of(','));
  str = str.substr(str.find_first_of(',') + 1);
  TreeNode * root = new TreeNode(foo[0] - '0');
  MakeFromOjString(str, root);
  return root;
};

template <typename T>
T * MakeFromOjString(std::string str) {
  std::string foo = str.substr(0, str.find_first_of(','));
  str = str.substr(str.find_first_of(',') + 1);
  T * root = new T(foo[0] - '0');
  MakeFromOjString<T>(str, root);
  return root;
};

void InOrder(TreeNode * root) {
  if (root == NULL) return;
  InOrder(root->left);
  std::cout << root->val << std::endl;
  InOrder(root->right);
}

template <typename T>
void InOrder(T * root) {
  if (root == NULL) return;
  InOrder(root->left);
  std::cout << root->val << std::endl;
  InOrder(root->right);
}

void PreOrder(TreeNode * root) {
  if (root == NULL) return;
  std::cout << root->val << std::endl;
  PreOrder(root->left);
  PreOrder(root->right);
}

template <typename T>
void PreOrder(T * root) {
  if (root == NULL) return;
  std::cout << root->val << std::endl;
  PreOrder(root->left);
  PreOrder(root->right);
}

TreeNode * Make(std::string & str) {
  return MakeFromOjString(str);
}

template <typename T>
T * Make(std::string & str) {
  return MakeFromOjString<T>(str);
}

}  // namespace algorithm

#endif  //  __OJBST_H_

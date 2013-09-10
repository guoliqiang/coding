// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 14:41:21
// File  : bst_to_dq.h
// Brief :

#ifndef BST_H
#define BST_H

#include <string>
#include <vector>
#include <algorithm>
#include "stdlib.h"
#include "base/public/logging.h"

namespace algorithm {

template <typename type>
struct Node {
  Node() {
    left = NULL;
    right = NULL;
  }

  type value;
  Node<type> * left;
  Node<type> * right;
};

template <typename type>
Node<type> * BuildBST(std::vector<type> & v, int i, int j) {
  if (i >= j) return NULL;
  int foo = i + (j - i) / 2;
  Node<type> * ptr = new Node<type>();
  ptr->value = v[foo];
  ptr->left = BuildBST(v, i, foo);
  ptr->right = BuildBST(v, foo + 1, j);
  return ptr;
}

template <typename type>
Node<type> * BuildBST(std::vector<type> & v) {
  return BuildBST(v, 0, v.size());
}

template <typename type>
void MiddleRootIterate(Node<type> * root) {
  if (root == NULL) return;
  MiddleRootIterate(root->left);
  LOG(INFO) << root->value;
  MiddleRootIterate(root->right);
}

template <typename type>
void DeleteBST(Node<type> * root) {
  if (root == NULL) return;
  DeleteBST(root->left);
  DeleteBST(root->right);
  delete root;
}

template <typename type>
void DeleteDQ(Node<type> * head) {
  while(head != NULL) {
    Node<type> * foo = head;
    head = head->right;
    delete foo;
  }
}

template <typename type>
std::pair<Node<type>*, Node<type> *> BST2DQ(Node<type> * root) {
  std::pair<Node<type> *, Node<type>*> rs;
  rs.first = NULL;
  rs.second = NULL;
  if (root == NULL) {
    return rs;
  } else {
    std::pair<Node<type>*, Node<type>*> a = BST2DQ(root->left);
    std::pair<Node<type>*, Node<type>*> b = BST2DQ(root->right);
    root->left = a.second;
    if (a.second != NULL) a.second->right = root;
    root->right = b.first;
    if (b.first != NULL) b.first->left = root;
    if (a.first != NULL) rs.first = a.first;
    else rs.first = root;
    if (b.second != NULL) rs.second = b.second;
    else rs.second = root;
    return rs;
  }
}

template <typename type>
void IterateDQ(Node<type> * head) {
  while (head != NULL) {
    LOG(INFO) << head->value;
    head = head->right;
  }
}

template <typename type>
void ReverseIterateDQ(Node<type> * tail) {
  while (tail != NULL) {
    LOG(INFO) << tail->value;
    tail = tail->left;
  }
}

}  // namespace algorithem

#endif  // BST_H

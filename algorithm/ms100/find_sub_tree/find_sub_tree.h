// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 16:43:47
// File  : find_sub_tree.h
// Brief :


#ifndef  __FIND_SUB_TREE_H_
#define  __FIND_SUB_TREE_H_

#include "../bst_to_dq/bst.h"
#include "base/public/string_util.h"

namespace algorithm {

template<typename type>
void FindPathSub(Node<type> * root, type * v, std::vector<type> * path, type * f) {
  if (root == NULL) return;
  *v += root->value;
  // LOG(INFO) << *v;
  path->push_back(root->value);
  if (*v == *f) {
    LOG(INFO) << "find " << *f << " at "
              << path->size() << ":" << JoinVector(*path);
  }
  FindPathSub(root->left, v, path, f);
  FindPathSub(root->right, v, path, f);
  *v -= root->value;
  path->pop_back();
}

template<typename type>
void FindPath(Node<type> * root, type f) {
  std::vector<type> foo;
  type v = 0;
  FindPathSub(root, &v, &foo, &f);
}

}  // namespace algorithm

#endif  //__FIND_SUB_TREE_H_

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 23:44:30
// File  : LCA.h
// Brief :

#ifndef  __LCA_H_
#define  __LCA_H_

// if tree is bst, this is a simple question
//
// 可以转换成求：链表的交点问题
// 路径1： 1 10 4 3
// 路径2： 1 10 6 8 7
// 交点为 10
//
// 求距离要使用节点的深度 d[i] + d[j] - 2 * d[ancestor]

// http://comzyh.tk/blog/archives/492/

#include "../bst_to_dq/bst.h"
#include "base/public/logging.h"
#include "util/unionfind_set/public/unionfind_set.h"
#include <map>
#include <set>
#include <vector>

using namespace util;  // NOLINT


// 大众解法, 时间复杂度高，多次遍历二叉树
namespace algorithm {
template <typename type>
bool Find(Node<type> * root, Node<type> * a) {
  if (root == NULL) return false;
  if (root == a) return true;
  if(Find(root->left, a)) return true;
  return Find(root->right, a);
}

template <typename type>
Node<type> * LCABadVersion(Node<type> * root, Node<type> * a, Node<type> * b) {
  if (root == a && root == b) return root;
  bool l_a = Find(root->left, a);
  bool l_b = Find(root->left, b);
  bool r_a = Find(root->right, a);
  bool r_b = Find(root->right, b);

  if (l_a && r_b || l_b && r_a) return root;
  if (l_a && l_b) return LCABadVersion(root->left, a, b);
  if (r_a && r_b) return LCABadVersion(root->right, a, b);
  return NULL;
}

}  // namespace algorithm

namespace algorithm {

template <typename type>
Node<type> * LCASub(Node<type> * root,
                    Node<type> * a,
                    Node<type> * b,
                    int * flag_a,
                    int * flag_b) {
  if (root == NULL) return NULL;
  if (root == a && root == b) {
    *flag_a = 1;
    *flag_b = 1;
    return root;
  }
  if (root == a) {
    *flag_a = 1;
    if (*flag_b) return NULL;
    else {
      LCASub(root->left, a, b, flag_a, flag_b);
      if (*flag_a + *flag_b == 2) return root;
      LCASub(root->right, a, b, flag_a, flag_b);
      if (*flag_a + *flag_b == 2) return root;
      return NULL;
    }
  }
  if (root == b) {
    *flag_b = 1;
    if (*flag_a) return NULL;
    else {
      LCASub(root->left, a, b, flag_a, flag_b);
      if (*flag_a + *flag_b == 2) return root;
      LCASub(root->right, a, b, flag_a, flag_b);
      if (*flag_a + *flag_b == 2) return root;
      return NULL;
    }
  }
  CHECK(*flag_a + *flag_b <= 1);
  Node<type> * foo = LCASub(root->left, a, b, flag_a, flag_b);
  if (*flag_a + *flag_b == 2) {
    return foo;
  }
  if (*flag_a + *flag_b == 0) {
    return LCASub(root->right, a, b, flag_a, flag_b);
  }
  if (*flag_a + *flag_b == 1) {
    LCASub(root->right, a, b, flag_a, flag_b);
    if (*flag_a + *flag_b == 2) return root;
    else return NULL;
  }
  CHECK(false);
  return NULL; // for compiler
}

template <typename type>
Node<type> * LCA(Node<type> * root, Node<type> * a, Node<type> * b) {
  int flag_a = 0;
  int flag_b = 0;
  return LCASub(root, a, b, &flag_a, &flag_b);
}


// 单对的LCA问题用这种解法比较好
// 由于记录了深度，可以直接计算出节点间的距离
template <typename type>
void LCA2Sub(Node<type> * root, Node<type> * a, Node<type> * b,
             int * flag_a, int * flag_b, std::map<int, Node<type> *> &m,
             int l) {
  if (root == NULL) return;
  if (*flag_a + *flag_b == 2) return;
  l++;
  if (a == root && b == root) {
    *flag_a = 1;
    *flag_b = 1;
    m.insert(std::make_pair(l, root));
    return;
  }
  if (a == root) {
    *flag_a = 1;
    m.insert(std::make_pair(l, root));
  }
  if (b == root) {
    *flag_b = 1;
    m.insert(std::make_pair(l, root));
  }
  LCA2Sub(root->left, a, b, flag_a, flag_b, m, l);
  if (*flag_a + *flag_b == 1) {
    m.insert(std::make_pair(l, root));
  }
  LCA2Sub(root->right, a, b, flag_a, flag_b, m, l);
  if (*flag_a + *flag_b == 1) {
    m.insert(std::make_pair(l, root));
  }
}

template <typename type>
Node<type> * LCA2(Node<type> * root, Node<type> * a, Node<type> * b) {
  int flag_a = 0;
  int flag_b = 0;
  std::map<int, Node<type>*> m;
  int l = 0;
  LCA2Sub(root, a, b, &flag_a, &flag_b, m, l);
  if (m.size()) return m.begin()->second;
  return NULL;
}

// off line algorithm, because it need to read all query
// at start time.
template <typename type>
void Tarjan(Node<type> * root,
            std::map<Node<type> *,
            std::vector<Node<type>*> > & query,
            UFSet<Node<type> * > & uf,
            std::set<Node<type>*> & visited,
            std::map<std::pair<Node<type>*, Node<type>*>, Node<type> *> * rs) {
  if (root == NULL) return;
  uf.Insert(root);
  visited.insert(root);
  typename std::map<Node<type> *, std::vector<Node<type>*> >::iterator i;
  i = query.find(root);
  if (i != query.end()) {
    for (int j = 0; j < i->second.size(); j++) {
      if (visited.count(i->second[j])) {
        Node<type> * foo = NULL;
        CHECK(uf.Find(i->second[j], &foo));
        // LOG(INFO) << root << " " << " " <<  i->second[j] << " "<< root->value
        //           << " " << i->second[j]->value;
        rs->insert(std::make_pair(std::make_pair(root, i->second[j]), foo));
      }
    }
  }
  
  Tarjan(root->left, query, uf, visited, rs);
  if (root->left != NULL) {
    uf.Union(root, root->left, true);
  }
  Tarjan(root->right, query, uf, visited, rs);
  if (root->right != NULL) {
    uf.Union(root, root->right, true);
  }

}

template <typename type>
Node<type> * Tarjan(Node<type> * root, Node<type> * a, Node<type> * b) {
  UFSet<Node<type> *> uf;
  std::map<Node<type> *, std::vector<Node<type>*> > query;
  std::vector<Node<type> *> foo;
  foo.push_back(b);
  query.insert(std::make_pair(a, foo));
  foo.clear();
  foo.push_back(a);
  query.insert(std::make_pair(b, foo));
  std::map<std::pair<Node<type>*, Node<type>*>, Node<type> *> rs;
  std::set<Node<type>*> visited;
  Tarjan(root, query, uf, visited, &rs);
  CHECK(rs.size() == 1) << rs.size();
  return rs.begin()->second;
}

}  // namespace algorithm

#endif  // __LCA_H_

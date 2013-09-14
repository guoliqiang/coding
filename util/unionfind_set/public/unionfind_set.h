// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-14 18:57:23
// File  : unionfind_set.h
// Brief :

#ifndef  __UNIONFIND_SET_H_
#define  __UNIONFIND_SET_H_

#include "base/public/logging.h"
#include "base/public/shared_ptr.h"
#include <map>

namespace util {

template <typename type>
struct UFNode {
 public:
  UFNode(type t) : v(t), count(1), parent(NULL){}

  type v;
  int count;
  UFNode<type> * parent;
};

template <typename type>
class UFSet {
 public:
  int Insert(const type & v);
  bool Find(const type & v, type * re);
  bool Union(const type & a, const type & b);
  bool FindPC(const type & v, type * re);
  bool Exist(const type & v);

 private:
  UFNode<type> * FindPCSub(UFNode<type> * v);

 private:
  std::map<type, base::shared_ptr<UFNode<type> > > set_;
};

template <typename type>
bool UFSet<type>::Exist(const type & v) {
  return set_.count(v);
}

template <typename type>
int UFSet<type>::Insert(const type & v) {
  if (Exist(v)) {
    return 0;
  }
  base::shared_ptr<UFNode<type> >ptr(new UFNode<type>(v));
  set_.insert(std::make_pair(v, ptr));
  return 1;
}

template <typename type>
bool UFSet<type>::Find(const type & v, type * re) {
  if (!Exist(v)) return false;
  UFNode<type> * foo = set_[v].get();
  while (foo->parent != NULL) {
    foo = foo->parent;
  }
  *re = foo->v;
  return true;
}

template <typename type>
bool UFSet<type>::FindPC(const type & v, type * re) {
  if (!Exist(v)) return false;
  UFNode<type> * foo =  FindPCSub(set_[v].get());
  *re = foo->v;
  return true;
}

template <typename type>
UFNode<type> * UFSet<type>::FindPCSub(UFNode<type> * v) {
  if (v->parent == NULL) {
    return v;
  }
  v->parent = FindPCSub(v->parent);
  return v->parent;
}

template <typename type>
bool UFSet<type>::Union(const type & a, const type & b) {
  if (!Exist(a)) return false;
  if (!Exist(b)) return false;
  UFNode<type> * foo = set_[a].get();
  UFNode<type> * bar = set_[b].get();
  while (foo->parent != NULL) {
    foo = foo->parent;
  }
  while (bar->parent != NULL) {
    bar = bar->parent;
  }
  if (foo->count < bar->count) {
    bar->count += foo->count;
    foo->parent = bar;
  } else {
    foo->count += bar->count;
    bar->parent = foo;
  }
  return true;
}

}  // namespace util


#endif  // __UNIONFIND_SET_H_

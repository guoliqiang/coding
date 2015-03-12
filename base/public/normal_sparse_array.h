// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-07 11:52:59
// File  : normal_sparse_array.h
// Brief :

#ifndef  __NORMAL_SPARSE_ARRAY_H_
#define  __NORMAL_SPARSE_ARRAY_H_

#include "logging.h"
#include <map>

namespace base {

template <typename Type>
class NormalSarray {
 public:
  void set(const std::map<int32_t, Type> & elements) {
    elements_.clear();
    for (const_iterator i = elements.begin(); i!= elements.end();
         i ++) {
      insert(i->first, i->second);
    }
  }
  std::map<int32_t, Type> & Get() {
    return elements_;
  }

  const std::map<int32_t, Type> & Get() const {
    return elements_;
  }

  bool insert(const int32_t & i, const Type & v) {
    bool rs = exist(i);
    elements_[i] = v;
    return rs;
  }

  bool get(const int32_t & i, Type * p) const {
    if(!exist(i)) return false;
    *p = elements_.find(i)->second;
    return true;
  }
  bool erase(const int32_t & i) {
    if (!exist(i)) return false;
    elements_.erase(i);
    return true;
  }

  typedef typename std::map<int32_t, Type>::iterator iterator;
  typedef typename std::map<int32_t, Type>::const_iterator const_iterator;
  
  iterator begin() {
    return elements_.begin();
  }

  iterator end() {
    return elements_.end();
  }

  const_iterator begin() const {
    return elements_.begin();
  }

  const_iterator end() const {
    return elements_.end();
  }
  
  int32_t size() const {
    return elements_.size();
  }
  
  bool exist(const int32_t & i) const {
    return elements_.find(i) != elements_.end();
  }
 private:
  std::map<int32_t, Type> elements_;
};

}  // namespace base

#endif  // __NORMAL_SPARSE_ARRAY_H_

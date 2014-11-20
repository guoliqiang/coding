// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-19 22:33:51
// File  : fast_k_merger_tree.h
// Brief :

#ifndef  __FAST_K_MERGER_TREE_H_
#define  __FAST_K_MERGER_TREE_H_

#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace kway {
template <typename T, typename Iterator, class Cmp = std::less<T> >
class FastKmergeTree {
 public:
  void Add(const Iterator & begin, const Iterator & end) {
    if (begin != end) data_.push_back(std::make_pair(begin, end));
  }

  void Build() {
    if (data_.size() == 0) return;
    success_tree_.resize(2 * data_.size() - 1);
    for (int i = data_.size() - 1; i >= 0; i--) {
      int index = success_tree_.size() - data_.size() + i;
      success_tree_[index] = i;
    }
    for (int i = success_tree_.size() - 1; i >= 2; i -= 2) {
      int parent = i / 2 - 1;
      int brother = i - 1;
      int cur_index = success_tree_[i];
      int brother_index = success_tree_[brother];

      if (cmp_(*(data_[cur_index].first), *(data_[brother_index].first))){
        success_tree_[parent] = cur_index;
      } else {
        success_tree_[parent] = brother_index;
      }
    }
  }

  bool Top(Iterator * rs) {
    if (data_.size() == 0) return false;
    int top = success_tree_[0];
    if (data_[top].first == data_[top].second) return false;
    *rs = data_[top].first;
    return true;
  }

  void Next() {
    int top = success_tree_[0];
    data_[top].first++;
    int index = data_.size() - 1 + top;
    while (index > 0) {
      int parent = index % 2 == 0 ? index / 2 - 1 : index / 2;
      int brother =  (2 * parent + 1) == index ? index + 1 : index - 1;
      int cur_index = success_tree_[index];
      int brother_index = success_tree_[brother];

      if (data_[cur_index].first == data_[cur_index].second) {
          success_tree_[parent] =  brother_index;
      } else if (data_[brother_index].first == data_[brother_index].second) {
          success_tree_[parent] =  cur_index;
      } else {
        if (cmp_(*(data_[cur_index].first), *(data_[brother_index].first))) {
          success_tree_[parent] =  cur_index;
        } else {
          success_tree_[parent] =  brother_index;
        }
      }
      index = parent;
    }
  }

 private:
  std::vector<std::pair<Iterator, Iterator> > data_;
  std::vector<int> success_tree_;
  Cmp cmp_;
};

}  // namespace kway

#endif  // __FAST_K_MERGER_TREE_H_

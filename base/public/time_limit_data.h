// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-09-21 22:29:59
// File  : time_limit_data.h
// Brief :

#ifndef  __TIME_LIMIT_DATA_H_
#define  __TIME_LIMIT_DATA_H_

#include <map>
#include "shared_ptr.h"
#include "time.h"

namespace base {

template<typename KeyType, typename ValueType>
class TimeLimitData {
 public:
  struct Node {
    Node(const ValueType & v, int valid_time) {
      value.reset(new ValueType(v));
      end_time = static_cast<int32_t>(time(NULL)) + valid_time;
    }

    base::shared_ptr<ValueType> value;
    int32_t end_time;
  };

 public:
  base::shared_ptr<ValueType> Find(const KeyType & key) {
    if (!Count(key)) return shared_ptr<ValueType>(NULL);
    return data_[key].value;
  }

  bool Count(const KeyType & key) {
    if (!data_.count(key)) return false;
    if (data_[key].end_time < static_cast<int32_t>(time(NULL))) {
      data_.erase(key);
      return false;
    }
    return true;
  }

  void Begin() {
    cur_ = data_.begin();
    Next(false);
  }

  void Next(bool increase = true) {
    if (increase) cur_++;
    while(!IsEnd()) {
      if (cur_->second.end_time < static_cast<int32_t>(time(NULL))) {
        cur_ = data_.erase(cur_);
      } else break;
    }
  }

  bool IsEnd() {
    return cur_ == data_.end();
  }

  base::shared_ptr<ValueType> CurrentValue() {
    Next(false);
    if (IsEnd()) return base::shared_ptr<ValueType>(NULL);
    return cur_->second.value;
  }

  bool Add(const KeyType & key, const ValueType & value, int32_t valid_time) {
    data_[key] = Node(value, valid_time);
    return true;
  }

  bool Erase(const KeyType & key) {
    if (Count(key)) {
      data_.erase(key);
      return true;
    }
    return false;
  }

  bool EraseAll() {
    data_.clear();
    return true;
  }

 private:
  typename std::map<KeyType, Node>::iterator cur_;
  std::map<KeyType, Node> data_;
};

}  // namespace base

#endif  // __TIME_LIMIT_DATA_H_

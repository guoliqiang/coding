// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// File  : time_limit_data.h
// Brief :

#ifndef  __TIME_LIMIT_DATA_H_
#define  __TIME_LIMIT_DATA_H_

#include <map>
#include <list>
#include "shared_ptr.h"
#include "time.h"
#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "base/public/thread.h"

namespace base {
// not thread-safe
template<typename KeyType, typename ValueType>
class TimeLimitKVData {
 private:
  struct Node {
    Node(const ValueType & v, int valid_time) {
      value.reset(new ValueType(v));
      end_time = static_cast<int32_t>(time(NULL)) + valid_time;
    }
    Node() {
      value.reset(NULL);
      end_time = 0;
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
    need_erase_key_.clear();
    cur_ = data_.begin();
    Next(false);
  }

  void Next(bool increase = true) {
    if (increase && !IsEnd()) {
      cur_++;
    }
    while(!IsEnd()) {
      if (cur_->second.end_time < static_cast<int32_t>(time(NULL))) {
        need_erase_key_.push_back(cur_->first);
        cur_++;
      } else break;
    }
  }

  bool IsEnd() {
    if (cur_ == data_.end()) {
      for (typename std::list<KeyType>::iterator i = need_erase_key_.begin();
           i != need_erase_key_.end(); i++) {
        data_.erase(*i);
      }
      need_erase_key_.clear();
      return true;
    }
    return false;
  }

  std::pair<KeyType, base::shared_ptr<ValueType> > CurrentValue() {
    Next(false);
    if (IsEnd()) {
      return std::make_pair(KeyType(), base::shared_ptr<ValueType>(NULL));
    }
    return std::make_pair(cur_->first, cur_->second.value);
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

  int32_t ImpreciseSize() {
    return data_.size();
  }

  int32_t PreciseSize() {
    int cnt = 0;
    for (Begin(); !IsEnd(); Next()) cnt++;
    return cnt;
  }

 private:
  typename std::map<KeyType, Node>::iterator cur_;
  std::map<KeyType, Node> data_;
  std::list<KeyType> need_erase_key_;
};

template <typename KeyType, typename ValueType>
class ThreadSafeTimeLimitKVData;

template <typename KeyType, typename ValueType>
class TimeLimitKVDataThread : public base::Thread {
 public:
  TimeLimitKVDataThread(ThreadSafeTimeLimitKVData<KeyType, ValueType> * kv)
      : base::Thread() {
    kv_data_ = kv;
  }

 protected:
  virtual void Run() {
    while (true) {
      sleep(60 * 60);  // an hour
      kv_data_->PreciseSize();  // hack to clear expiration data
    }
  }

 private:
  ThreadSafeTimeLimitKVData<KeyType, ValueType> * kv_data_;
};

template<typename KeyType, typename ValueType>
class ThreadSafeTimeLimitKVData : public TimeLimitKVData<KeyType, ValueType> {
 public:
  ThreadSafeTimeLimitKVData() {
    thread_.reset(new TimeLimitKVDataThread<KeyType, ValueType>(this));
    thread_->Start();
  }

  base::shared_ptr<ValueType> Find(const KeyType & key) {
    base::MutexLock lock(&mutex_);
    return TimeLimitKVData<KeyType, ValueType>::Find(key);
  }

  bool Count(const KeyType & key) {
    base::MutexLock lock(&mutex_);
    return TimeLimitKVData<KeyType, ValueType>::Count(key);
  }

  bool Erase(const KeyType & key) {
    base::MutexLock lock(&mutex_);
    return TimeLimitKVData<KeyType, ValueType>::Erase(key);
  }

  bool EraseAll() {
    base::MutexLock lock(&mutex_);
    return TimeLimitKVData<KeyType, ValueType>::EraseAll();
  }

  int32_t ImpreciseSize() {
    base::MutexLock lock(&mutex_);
    return TimeLimitKVData<KeyType, ValueType>::ImpreciseSize();
  }

  int32_t PreciseSize() {
    base::MutexLock lock(&mutex_);
    return TimeLimitKVData<KeyType,  ValueType>::PreciseSize();
  }
  void Data(std::vector<std::pair<KeyType, base::shared_ptr<ValueType> > > *
      all_valid_data) {
    base::MutexLock lock(&mutex_);
    all_valid_data->clear();
    for (TimeLimitKVData<KeyType, ValueType>::Begin();
         !TimeLimitKVData<KeyType, ValueType>::IsEnd();
         TimeLimitKVData<KeyType, ValueType>::Next()) {
      std::pair<KeyType, base::shared_ptr<ValueType> > v
          = TimeLimitKVData<KeyType, ValueType>::CurrentValue();
      if (v.second.get() != NULL) all_valid_data->push_back(v);
    }
  }

 private:
  base::Mutex mutex_;
  base::shared_ptr<base::TimeLimitKVDataThread<KeyType, ValueType> > thread_;
};

}  // namespace base

#endif  // __TIME_LIMIT_DATA_H_

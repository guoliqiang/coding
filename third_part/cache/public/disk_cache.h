// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-08 01:42:46
// File  : disk_cache.h
// Brief :

#ifndef  __DISK_CACHE_H_
#define  __DISK_CACHE_H_

#include <vector>
#include <map>
#include <list>
#include "base/public/basictypes.h"
#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "file/public/file.h"
#include "file/public/file_posix.h"

namespace cache {
template <typename key_type>
class DiskCache {
 struct Node {
   std::vector<int> used_block;
   int last_block_offset;
 };

 public:
  DiskCache(const std::string & path, int block_num = 100,
            int block_size = 1024 * 1024 /*1MB*/) {
    path_ = path;
    block_num_ = block_num;
    block_size_ = block_size;
    file_ = NULL;
    realtime_flush_ = true;
    if (file::File::Exists(path_)) file::File::DeleteRecursively(path_);
    CHECK((file_ = fopen(path_.c_str(), "w+b")) != NULL)
        << "open file " << path << " error!";
    for (int i = 0; i < block_num_; i++) free_block_.push_back(i);
  }

  ~DiskCache() {
    if (file_ != NULL) fclose(file_);
    if (file::File::Exists(path_)) file::File::DeleteRecursively(path_);
  }

  void SetRealtimeFlush(bool flush) {
    base::MutexLock lock(&mutex_);
    realtime_flush_ = flush;
  }

  bool GetData(const key_type & key, std::string * data = NULL) {
    base::MutexLock lock(&mutex_);
    MagicCheck();
    if (!key_map_.count(key)) {
      VLOG(5) << "not find key:" << key;
      return false;
    }
    if (data != NULL) {
      Node & node = key_map_[key];
      CHECK(node.used_block.size() > 0) << node.used_block.size();
      int buffer_size = (node.used_block.size()  - 1 ) * block_size_
          + node.last_block_offset;
      data->resize(buffer_size);
      for (int i = 0; i < node.used_block.size() - 1; i++) {
        int b = node.used_block[i];
        fseek(file_, b * block_size_, SEEK_SET);
        fread(&((*data)[i * block_size_]), block_size_, 1, file_);
        VLOG(5) << "read block:" << b << " for key:" << key;
      }
      int b = node.used_block.back();
      fseek(file_, b * block_size_, SEEK_SET);
      fread(&((*data)[(node.used_block.size() - 1) * block_size_]),
            node.last_block_offset, 1, file_);
      VLOG(5) << "read block:" << b << " for key:" << key;
    }
    LruUpdateKey(key);
    return true;
  }

  bool SetData(const key_type & key, const std::string & data) {
    base::MutexLock lock(&mutex_);
    MagicCheck();
    if (key_map_.count(key)) {
      if (UpdateData(key, data)) LruUpdateKey(key);
      else {
        LOG(ERROR) << "update error, key:" << key << ", data size:" << data.size();
        return false;
      }
    } else {
      if (AddData(key, data)) LruAddKey(key);
      else {
        LOG(ERROR) << "add error, key:" << key << ", data size:" << data.size();
        return false;
      }
    }
    VLOG(5) << "add key:" << key << ", data size:" << data.size();
    return true;
  }

 private:
  bool EnoughSpace(int size) {
    int free_size = free_block_.size() * block_size_;
    return free_size >= size;
  }

  bool AddData(const key_type & key, const std::string & data) {
    while (!EnoughSpace(data.size()) && key_map_.size() > 0) {
      VLOG(5) << "delete key:" << order_list_.back();
      DeleteData(order_list_.back());
      LruDeleteKey(order_list_.back());
    }
    if (!EnoughSpace(data.size())) {
      LOG(ERROR) << "not enough memory for data size=" << data.size();
      return false;
    } else {
      key_map_.insert(std::make_pair(key, Node()));
      Node & node = key_map_[key];
      for (int i = 0; i < data.size() / block_size_; i++) {
        int b = GetFreeBlock();
        fseek(file_, b * block_size_, SEEK_SET);
        fwrite(data.c_str() + i * block_size_, 1, block_size_, file_);
        node.used_block.push_back(b);
        VLOG(5) << "write block:" << b << " for:" << key;
      }
      node.last_block_offset = data.size() % block_size_;
      int b = GetFreeBlock();
      fseek(file_, b * block_size_, SEEK_SET);
      fwrite(data.c_str() + data.size() - node.last_block_offset, 1, node.last_block_offset, file_);
      node.used_block.push_back(b);
      VLOG(5) << "write block:" << b << " for:" << key;
      if(realtime_flush_) fflush(file_);
      return true;
    }
  }

  bool DeleteData(const key_type & key) {
    CHECK(key_map_.count(key)) << key;
    const std::vector<int> used_block = key_map_[key].used_block;
    free_block_.insert(free_block_.end(), used_block.begin(), used_block.end());
    key_map_.erase(key);
    return true;
  }

  bool UpdateData(const key_type & key, const std::string & data) {
    DeleteData(key);
    return AddData(key, data);
  }

  int GetFreeBlock() {
    CHECK(free_block_.size() > 0);
    int t = free_block_.back();
    free_block_.pop_back();
    return t;
  }

  void LruUpdateKey(const key_type & key) {
    CHECK(key_iterator_.count(key)) << key;
    typename std::list<key_type>::iterator index = key_iterator_[key];
    order_list_.splice(order_list_.begin(), order_list_, index);
    VLOG(5) << "move key:" << key << " to first place!";
  }

  void LruAddKey(const key_type & key) {
    CHECK(!key_iterator_.count(key)) << key;
    order_list_.push_front(key);
    key_iterator_.insert(std::make_pair(key, order_list_.begin()));
    VLOG(5) << "add key:" << key << " at first place!";
  }

  void LruDeleteKey(const key_type & key) {
    CHECK(key_iterator_.count(key)) << key;
    order_list_.erase(key_iterator_[key]);
    key_iterator_.erase(key);
    VLOG(5) << "delete key:" << key << " in lru";
  }

  void MagicCheck() {
    CHECK(key_map_.size() == order_list_.size()) << key_map_.size() << " "
        << order_list_.size();
    CHECK(key_map_.size() == key_iterator_.size()) << key_map_.size() << " "
        << key_iterator_.size();
  }

 private:
  std::map<key_type, Node> key_map_;
  std::list<key_type> order_list_;
  std::map<key_type, typename std::list<key_type>::iterator> key_iterator_;
  std::vector<int> free_block_;
  FILE * file_;
  std::string path_;
  int block_num_;
  int block_size_;
  bool realtime_flush_;
  base::Mutex mutex_;
};
}  // namespace cache
#endif  // __DISK_CACHE_H_

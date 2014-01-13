// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-19 22:19:56
// File  : ring_buffer.h
// Brief :

#ifndef  __RING_BUFFER_H_
#define  __RING_BUFFER_H_

// http://www.asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html
// TODO unittest
//
#include "base/public/common_head.h"
#include "base/public/mutex.h"

namespace base {

class RingBuffer {
 public:
  RingBuffer(int capacity) {
    if (capacity <= 0) {
      LOG(FATAL) << "capacity can not <= 0.";
    } else {
      capacity_ = capacity;
      size_ = 0;
      b_index_ = 0;
      e_index_ = 0;
    }
  }

 void Write(char * data, int size) {
   base::MutexLock lock(&mutex_);  
   while (BlankSpace() < size) {
     w_con_.Wait(&mutex_);
   }
   if (capacity_ - e_index_ >= size) {
     memcpy(data_ + e_index_, data, size);
     e_index_ += size;
     if (e_index_ == capacity_) e_index_ = 0;
   } else {
     int size1 = capacity_ - e_index_;
     memcpy(data_ + e_index_, data, size1);
     int size2 = size - size1;
     memcpy(data_, data + size1, size2);
     e_index_ = size2;
   }
   size_ += size;
   r_con_.Signal();
 }

 void Read(char * data, int size) {
   base::MutexLock lock(&mutex_);
   while (DataSpace() < size) {
     r_con_.Wait(&mutex_);
   }
   if (capacity_ - b_index_ >= size) {
     memcpy(data, data_ + b_index_, size);
     b_index_ += size;
     if (b_index_ == capacity_) b_index_ = 0;
   } else {
     int size1 = capacity_ - b_index_;
     memcpy(data, data_ + b_index_, size1);
     int size2 = size - size1;
     memcpy(data + size1, data_, size2);
     b_index_ = size2;
   }
   size_ -= size;
   w_con_.Signal();
 }

 private:
  bool Full() {
    return BlankSpace() == 0;
  }

  bool Empty() {
    return DataSpace() == 0;
  }

  int BlankSpace() {
    return capacity_ - size_;
  }

  int DataSpace() {
    return size_;
  }
 
 private:
  int b_index_;  // if not empty, the first index can read
  int e_index_;  // if not full, the first index can write
  int size_;
  int capacity_;
  char * data_;
 
 private:
  Mutex mutex_;
  CondVar w_con_;
  CondVar r_con_;
};

}  // namespace base

#endif  //__RING_BUFFER_H_

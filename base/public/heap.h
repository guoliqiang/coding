// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-31 12:55:42
// File  : heap.h
// Brief :

#ifndef  __HEAP_H_
#define  __HEAP_H_
#include <algorithm>
#include "base/public/logging.h"
namespace base {

template<typename Type>
class Heap {
 public:
  explicit Heap(bool min = true):min_(min) {}

  static bool CompareMin(const Type & a, const Type & b) {
    return a > b;
  }
  static bool CompareMax(const Type & a, const Type & b) {
    return a < b;
  }

  void Pop() {
    CHECK(!values_.empty()) << "heap is empty";
    bool (*com_ptr)(const Type &, const Type &) = min_ ? CompareMin : CompareMax;
    std::pop_heap(values_.begin(), values_.end(), com_ptr);
    values_.pop_back();
  }
 
  void Push(const Type & input) {
    values_.push_back(input);
    bool (*com_ptr)(const Type &, const Type &) = min_ ? CompareMin : CompareMax;
    std::push_heap(values_.begin(), values_.end(), com_ptr); 
  }
 
  Type & Get() {
    CHECK(!values_.empty()) << "heap is empty";
    return * values_.begin();
  }
  
  int32_t Size() {
   return values_.size();
  }

 protected:
  bool min_;
  std::vector<Type> values_;
};

template<typename Type>
class FixSizeHeap : public Heap<Type> {
 public:
  FixSizeHeap(uint32_t size, bool min):Heap<Type>(!min), size_(size){}
  
  void Push(const Type & input)  {
   if (Heap<Type>::Size() < size_) Heap<Type>::Push(input);
   else {
     if (Heap<Type>::min_ && Heap<Type>::Get() > input) {
       Heap<Type>::Pop();
       Heap<Type>::Push(input);
     }
     if (!Heap<Type>::min_ && Heap<Type>::Get() < input) {
       Heap<Type>::Pop();
       Heap<Type>::Push(input);
     }
   }
  }

  void Pop() {
    Heap<Type>::Pop();
  }

  Type & Get() {
    return Heap<Type>::Get();
  }
  
  uint32_t Size() {
    return Heap<Type>::Size();
  }
 private:
  uint32_t size_;
}; 
}  // namespace

#endif  //__HEAP_H_

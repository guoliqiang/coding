// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 23:58:38
// File  : suffix_array.h
// Brief :
// http://www.nocow.cn/index.php/%E5%90%8E%E7%BC%80%E6%95%B0%E7%B
// B%84#.E5.90.8E.E7.BC.80.E6.95.B0.E7.BB.84.E7.9A.84.E5.BA.94.E7.94.A8

#ifndef  __SUFFIX_ARRAY_H_
#define  __SUFFIX_ARRAY_H_

#include <vector>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/shared_ptr.h"
#include "base/public/scoped_ptr.h"
#include "base/public/string_util.h"
#include "math.h"

namespace util {

class SuffixArray {
 public:
  SuffixArray(std::string & str_i) {
    // rank sa height
    // value range: [1, str_.size())  <==> [0, str_i.size())
    // index range: [1, str_.size())
    // rank is double size, which is useful in bucket sorting
    str_.resize(str_i.size() + 1);
    for (int i = 1; i < str_.size(); i++) {
      str_[i] = str_i[i - 1];
    }

    rank_.assign(str_.size() * 2, 0);
    sa_.assign(str_.size(), 0);
    height_.assign(str_.size(), 0);
    sum_ = new int[0xFF];
    memset(sum_, 0, 0xff * sizeof(int));

    int foo = pow(2, log(str_.size()) / log(2) + 1);
    rmq_ = new int[foo];
    rmq_max_ = new int[foo];
    rmq_max_index_ = new int[foo];
    
    // building
    Build();
  }

  ~SuffixArray() {
    delete [] sum_;
    delete [] rmq_;
    delete [] rmq_max_;
    delete [] rmq_max_index_;
  }

 public:
  std::vector<int> & get_rank() {
    return rank_;
  }
  std::vector<int> & get_sa() {
    return sa_;
  }

  int GetRMQ(int b, int e);
  int GetRMQMax(int b, int e, int * rs = NULL);
  bool NotOverlapCheck(int len);
  int NotOverlapLen();
  int DiffSubStringNum();

 private:
  int GetRMQInternalMax(int b, int e, int i,
                     int j, int index, int * rs);
  int GetRMQInternal(int b, int e, int i,
                     int j, int index);
  void Build();
  void Height();
  void BucketSort(int j);
  void RMQ(int index, int b, int e);
  void RMQMax(int index, int b, int e, int * rs = NULL);
 
 // only used for unittest
 public:
  void OutRank();
  void OutSa();
  void OutHeight();
  void OutRMQ();
 
 private:
  std::string str_;
  int * sum_;
  int * rmq_;
  int * rmq_max_;
  int * rmq_max_index_;

 private:
  std::vector<int> rank_;
  std::vector<int> sa_;
  std::vector<int> height_;
};

}  // namespace util

#endif  //  __SUFFIX_ARRAY_H_

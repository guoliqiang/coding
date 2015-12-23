// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 18:27:05
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

class NumArray {
 public:
  NumArray(std::vector<int> &nums) {
    size = nums.size();
    if (size > 0) build(nums, 0, size - 1);
  }

  int build(std::vector<int> & nums, int b, int e) {
    if (b == e) {
      if (!v.count(b)) {
        v.insert(std::make_pair(b, std::map<int, int>()));
      }
      v[b][e] = nums[b];
    } else {
      int mid = b + (e - b) / 2;
      v[b][e] = build(nums, b, mid) + build(nums, mid + 1, e);
    }
    return v[b][e];
  }

  int sumRange(int i, int j) {
    if (j < i) return 0;
    return sumRange(i, j, 0, size - 1);
  }
  int sumRange(int i, int j, int b, int e) {
    if (i == b && j == e) {
      return v[b][e];
    } else {
      int mid = b + (e - b) / 2;
      if (j <= mid) return sumRange(i, j, b, mid);
      else if (i >= mid + 1) return sumRange(i, j, mid + 1, e);
      else {
        return sumRange(i, mid, b, mid) + sumRange(mid + 1, j, mid + 1, e);
      }
    }
  }
  std::map<int, std::map<int, int> > v;
  int size;
};

}  // namespace algorithm

namespace NB {
class NumArray {
 public:
  NumArray(std::vector<int> &nums) {
    int accum=0;
    for (int i=0;i<nums.size();i++) {
      accum+=nums[i];
      _acarray.push_back(accum);
    }
  }

  int sumRange(int i, int j) {
    if(i==0) return (_acarray[j]);
    else return (_acarray[j]-_acarray[i-1]);
  }

 private:
  std::vector<int> _acarray;
};

}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
  }
  NumArray foo(vec);
  LOG(INFO) << foo.sumRange(0, 1);
  LOG(INFO) << foo.sumRange(3, 6);
  LOG(INFO) << foo.sumRange(0, 9);
  return 0;
}

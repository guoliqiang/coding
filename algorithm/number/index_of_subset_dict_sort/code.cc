// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-28 04:55:34
// File  : code.cc
// Brief :
/*
 * 对于集合S={1,2,3,...,n}的r组合a1,a2,...,ar 如何求它在所有S的r组合中的编号？
 * 比如S={1,2,3,4}的3组合有：123，124，134，234。
 * 那么组合123的编号是1,134的编号是3
 
 * 为了问题的一般化，规定所有的组合都是递增顺序
 * 对于组合(123),(132),(312),(321),(213),(231)来说，它们都是相同的组合,我们只用递增顺序的(123)来表示。
 * 那么对于r组合a1,a2,a3,...,ar(其中a1<a2<a3<...<ar)来说，它在S的所有r组合中的编号为
 * C(r,n)-C(r,n-a1)-C(r-1,n-a2)-...-C(1,n-ar)
 *
 *
 * 证明：
 * 首先计算出现在a1,a2,a3,...,ar后面的r组合的个数
 * 在a1,a2,a3,...,ar后存在C(r,n-a1)个r组合，其第一个元素比a1大 // 由于按升序排序，后面的元素都会比a1大
 * 在a1,a2,a3,...,ar后存在C(r-1,n-a2)个r组合，其第一个元素等于a1，且第二个元素比a2大.
 * 在a1,a2,a3,...,ar后存在C(r-2,n-a3)个r组合，其第一、第二个元素分别等于a1、a2，且第三个元素比a3大.
 * ......
 * 
 * 最后从总数C(r,n)减去a1,a2,a3,...,ar后面的r组合的个数即可。
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

int C(int n, int r) {
  if (r > n) return 0;
  r = std::min(r, n - r);
  int rs = 1;
  int t = r;
  while (r > 0) {
    rs *= n - t + r;
    r--;
  }
  r = t;
  while (r > 0) {
    rs /= r;
    r--;
  }
  return rs;
}

int Index(int n, std::vector<int> v) {
  std::sort(v.begin(), v.end());
  int t = 0;
  int r = v.size();
  for (int i = 0; i < v.size(); i++) {
    t += C(n - v[i], r);
    r--;
  }
  return C(n, v.size()) - t;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 4;
  std::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  v.push_back(4);
  LOG(INFO) << Index(n, v);
  return 0;
}

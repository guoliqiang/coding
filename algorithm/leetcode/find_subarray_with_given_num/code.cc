// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-13 21:42:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Sort(std::vector<int> & vec, int b, int e, int v, bool & ans) {
  if (b >= e) return;
  int mid = b + (e - b) / 2;
  Sort(vec, b, mid, v, ans);
  Sort(vec, mid + 1, e, v, ans);

  std::vector<int> tmp;
  int i = b;
  int j = mid + 1;
  int k = mid + 1;
  while (i <= mid || j <= e) {
    if (j > e || (i <= mid && vec[i] < vec[j])) {
      while (k <= e && vec[k] < vec[i] + v) k++;
      if (k <= e && vec[k] == vec[i] + v) ans = true;
      tmp.push_back(vec[i++]);
    } else {
      tmp.push_back(vec[j++]);
    }
  }
  for (i = 0; i < tmp.size(); i++) {
    vec[b + i] = tmp[i];
  }
}

bool Find(std::vector<int> & vec, int v) {
  int n = vec.size();
  if (n == 0) return false;
  std::vector<int> sum(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) sum[i] = vec[i];
    else sum[i] = sum[i - 1] + vec[i];
    if (sum[i] == v) return true;
  }
  bool ans = false;
  Sort(sum, 0, n - 1, v, ans);
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(0);
  vec.push_back(-6);
  vec.push_back(-4);
  vec.push_back(20);
  vec.push_back(-2);
  LOG(INFO) << Find(vec, 8);
}

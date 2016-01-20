// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 14:26:12
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Sort(std::vector<std::pair<int, int> > & num, int b, int e, std::vector<int> & ans) {
  if (b >= e) return;

  int mid = b + (e - b) / 2;
  Sort(num, b, mid, ans);
  Sort(num, mid + 1, e, ans);
  int i = b;
  int j = mid + 1;
  std::vector<std::pair<int, int> > tmp;
  while (i <= mid || j <= e) {
    if (i > mid || (j <= e && num[j].first < num[i].first)) {
      ans[num[j].second] += (mid - i + 1);
      tmp.push_back(num[j]);
      j++;
    } else {
      tmp.push_back(num[i]);
      i++;
    }
  }
  for (int k = 0; k < tmp.size(); k++) {
    num[b + k] = tmp[k];
  }
}

std::vector<int> BiggerNumberThanSelf(std::vector<int> & vec) {
  std::vector<int> ans;
  int n = vec.size();
  if (n == 0) return ans;

  std::vector<std::pair<int, int> > num;
  for (int i = 0; i < n; i++) {
    num.push_back(std::make_pair(vec[i], i));
    ans.push_back(0);
  }
  Sort(num, 0, n - 1, ans);
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(4);
  vec.push_back(2);
  vec.push_back(1);
  vec.push_back(3);
  vec.push_back(5);
  LOG(INFO) << JoinVector(BiggerNumberThanSelf(vec));
  return 0;
}

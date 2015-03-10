// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-13 18:02:15
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
void UnrepeatCombination(std::vector<int> & num, std::vector<int> & used,
                         std::vector<int> & path, int k) {
  LOG(INFO) << JoinVector(path);  // Note 每次都输出
  for (int i = k; i < num.size(); i++) {
    if (used[i] > 0) {
      used[i]--;
      path.push_back(num[i]);
      UnrepeatCombination(num, used, path, i);
      path.pop_back();
      used[i]++;
    }
  }
}

void UnrepeatCombination(std::vector<int> & num) {
  std::map<int, int> tmap;
  for (int i = 0; i < num.size(); i++) {
    if (tmap.count(num[i])) tmap[num[i]]++;
    else tmap[num[i]] = 1;
  }
  std::vector<int> v;
  std::vector<int> cnt;
  for (std::map<int, int>::iterator i = tmap.begin(); i != tmap.end(); i++) {
    v.push_back(i->first);
    cnt.push_back(i->second);
  }
  std::vector<int> path;
  UnrepeatCombination(v, cnt, path, 0);
}

void UnrepeatPermutation(std::vector<int> & num, std::vector<int> & used,
                         std::vector<int> & path, int cnt, int sum) {
  if (cnt == sum) {
    LOG(INFO) << JoinVector(path);
  }
  // 利用used标记是不是可以被选出来，所以不可能会有重复
  for (int i = 0; i < num.size(); i++) {
    if (used[i] > 0) {
      used[i]--;
      path.push_back(num[i]);
      UnrepeatPermutation(num, used, path, cnt + 1, sum);
      path.pop_back();
      used[i]++;
    }
  }
}

void UnrepeatPermutation(std::vector<int> & num) {
  std::map<int, int> tmap;
  for (int i = 0; i < num.size(); i++) {
    if (tmap.count(num[i])) tmap[num[i]]++;
    else tmap[num[i]] = 1;
  }
  std::vector<int> v;
  std::vector<int> cnt;
  int sum = 0;
  for (std::map<int, int>::iterator i = tmap.begin(); i != tmap.end(); i++) {
    v.push_back(i->first);
    cnt.push_back(i->second);
    sum += i->second;
  }
  std::vector<int> path;
  UnrepeatPermutation(v, cnt, path, 0, sum);
}

}  // namespace algorithm

namespace algorithm {
// see from twitter
void NewPermutation(std::vector<int>& vec, int n) {
  if (n == 0) {
    LOG(INFO) << JoinVector(vec) << "|";
  } else {
    for (int i = 0; i <= n; i++) {
      NewPermutation(vec, n - 1);
      int index = n % 2 == 0 ? i : 0;
      // LOG(INFO) << "swap vec[" << index << "]:" << vec[index]
      //           << " vec[" << n << "]:" << vec[n] << " n = " << n;
      std::swap(vec[index], vec[n]);
      // LOG(INFO) << JoinVector(vec);
    }
  }
}

void NormalPermutation(std::vector<int> & vec, int n) {
  if (n == vec.size()) {
    LOG(INFO) << JoinVector(vec);
  } else {
    for (int i = n; i < vec.size(); i++) {
      std::swap(vec[i], vec[n]);
      NormalPermutation(vec, n + 1);
      std::swap(vec[i], vec[n]);
    }
  }
}

}  // namespace algorithn

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(2);
  num.push_back(3);
  num.push_back(4);
  num.push_back(5);
  NewPermutation(num, 4);
  HERE(INFO);
  // NormalPermutation(num, 0);
  return 0;
  UnrepeatCombination(num);
  HERE(INFO);
  UnrepeatPermutation(num);

  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-13 18:02:15
// File  : code.cc
// Brief :

/*
在输入没有重复的时候，使用交换方式求得的permutation序列，可用next_permutation算法
求得的序列是一致的，这个规律之前没有留意到 poj1833

*/

#include "base/public/common_head.h"

namespace algorithm {
// 按顺序选，根据第一次选择分类.
void UnrepeatCombination(std::vector<int> & num, std::vector<int> & used,
                         std::vector<int> & path, int k) {
  LOG(INFO) << JoinVector(path);  // Note 每次都输出
  for (int i = k; i < num.size(); i++) {  // 因为需要按次序选择，因此只能从i之后的开始选
    if (used[i] > 0) {  // 如果某个元素重复了3次，结果中含有两个，这两个必然是前两个，不可能是后两个或第一与第三个
      used[i]--;
      path.push_back(num[i]);
      UnrepeatCombination(num, used, path, i);  // 因为i可能还有未被选择的元素，因此需要从i开始迭代
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
  if (cnt == sum) {  // path.size() == sum
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

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(3);
  num.push_back(3);
  UnrepeatCombination(num);
  HERE(INFO);
  UnrepeatPermutation(num);
  return 0;
}

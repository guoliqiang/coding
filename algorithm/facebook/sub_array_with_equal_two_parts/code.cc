// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-05 18:17:56
// File  : code.cc
// Brief :

/*
http://www.careercup.com/question?id=9332640
时间复杂度应该是:
max(O(n), O(pairs_num))
数组中只含有0，1（也可以是其它的什么东西），找出所有的subarray
其中0的个数和1的个数一样
*/
#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<std::vector<int> > Find(std::vector<int> vec) {
  std::map<int, std::vector<int> > tmap;
  vec.insert(vec.begin(), 0);
  int t = 0;
  tmap[0] = std::vector<int>();
  tmap[0].push_back(0);
  for (int i = 1; i < vec.size(); i++) {
    if (vec[i] == 1) t++;
    else t--;
    if (!tmap.count(t)) tmap[t] = std::vector<int>();
    tmap[t].push_back(i);
  }

  std::vector<std::vector<int> > rs;
  for (std::map<int, std::vector<int> >::iterator i = tmap.begin(); i != tmap.end(); i++) {
    const std::vector<int> & foo = i->second;
    for (int i = 0; i < foo.size(); i++) {
      for (int j = i + 1; j < foo.size(); j++) {
        rs.push_back(std::vector<int>(vec.begin() + foo[i] + 1, vec.begin() + foo[j] + 1));
      }
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<int> vec;
  // 0 1 0 0 1 1 1 1 0
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(0);
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(0);
  std::vector<std::vector<int> > rs = Find(vec);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}

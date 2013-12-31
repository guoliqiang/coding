// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 18:58:02
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"

namespace algorithm {
void Trace(std::vector<std::vector<int> > & rs, std::vector<int> & v, std::vector<int> & cnt,
           std::vector<int> & path, int k, int t) {
  if (path.size() == t) {
    rs.push_back(path);
  } else {
    for (int i = k; i < v.size(); i++) {
      if (cnt[i] > 0) {
        cnt[i]--;
        path.push_back(v[i]);
        Trace(rs, v, cnt, path, i, t);
        path.pop_back();
        cnt[i]++;
      }
    }
  }
}

std::vector<std::vector<int> > KSubset(std::vector<int> & num, int k) {
  std::map<int, int> tmap;
  for (int i = 0; i < num.size(); i++) {
    if (!tmap.count(num[i])) tmap[num[i]] = 0;
    tmap[num[i]]++;
  }
  std::vector<int> v;
  std::vector<int> cnt;
  for (std::map<int, int>::iterator i = tmap.begin(); i != tmap.end(); i++) {
    v.push_back(i->first);
    cnt.push_back(i->second);
  }
  std::vector<std::vector<int> > rs;
  std::vector<int> path;
  Trace(rs, v, cnt, path, 0, k);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(1);
  num.push_back(2);
  num.push_back(1);
  num.push_back(1);
  num.push_back(3);
  std::vector<std::vector<int> > rs = KSubset(num, 3);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}

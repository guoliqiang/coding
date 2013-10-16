// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 15:40:16
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 92 milli secs
 * Progress: 19/19 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void Subsets(std::vector<int> & s, std::vector<int> & v, int n,
             std::set<std::vector<int> > &rs) {
  if (n == s.size()) {
    std::vector<int> tmp;
    for (int i = 0; i < v.size(); i++) {
      if (v[i] == 1) tmp.push_back(s[i]);
    }
    std::sort(tmp.begin(), tmp.end());
    rs.insert(tmp);
  } else {
    for(int i = 0; i < 2; i++) {
      v[n] = i;
      Subsets(s, v, n + 1, rs);
    }
  }
}

std::vector<std::vector<int> > SubsetWithDup(std::vector<int> & s) {
  std::vector<int> v(s.size(), 0);
  std::set<std::vector<int> > sets;
  Subsets(s, v, 0, sets);
  std::vector<std::vector<int> > rs(sets.begin(), sets.end());
  return rs;
}

}  // namespace algorithm

namespace twice {
void Trace(std::vector<int> & vec, std::vector<int> & used, std::vector<int> & path,
           std::vector<std::vector<int> > &rs, int k) {
  rs.push_back(path);
  for (int i = k; i < vec.size(); i++) {
      if(used[i] > 0) {
          used[i]--;
          path.push_back(vec[i]);
          Trace(vec, used, path, rs, i);  // bug fixed: Note here is i not k
          path.pop_back();
          used[i]++;
      }
  }
}

std::vector<std::vector<int> > Trace(std::vector<int> & s) {
    std::map<int, int> tmap;
    for (int i = 0; i < s.size(); i++) {
        if (tmap.count(s[i])) tmap[s[i]]++;
        else tmap[s[i]] = 1;
    }
    std::vector<int> vec;
    std::vector<int> used;
    for (std::map<int, int>::iterator i = tmap.begin(); i != tmap.end(); i++) {
        vec.push_back(i->first);
        used.push_back(i->second);
    }
    tmap.clear();
    std::vector<int> path;
    std::vector<std::vector<int> > rs;
    Trace(vec, used, path, rs, 0);
    return rs;
}
}  // namespace twice

using namespace algorithm;

namespace NB {
using namespace std;

void Trace(std::vector<int> & vec, std::vector<int> & path, std::vector<std::vector<int> > & rs, int k) {
  rs.push_back(path);
  for (int i = k; i < vec.size(); i++) {
    if (i != k && vec[i] == vec[i - 1]) continue;
    path.push_back(vec[i]);
    Trace(vec, path, rs, i + 1);
    path.pop_back();
  }
}
std::vector<std::vector<int> > Trace(std::vector<int> & vec) {
  std::sort(vec.begin(), vec.end());
  std::vector<int> path;
  std::vector<std::vector<int> > rs;
  Trace(vec, path, rs, 0);
  return rs;
}
}  // namespace NB

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(0);
  foo.push_back(0);
  foo.push_back(1);
  std::vector<std::vector<int> > rs = NB::Trace(foo);
  // std::vector<std::vector<int> > rs = twice::Trace(foo);

  LOG(INFO) << JoinMatrix(&rs);
  LOG(INFO) << rs.size();
  return 0;
}

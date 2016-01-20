// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 15:48:35
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<std::vector<std::string> > GroupStrings(std::vector<std::string> & strs) {
  std::vector<std::vector<std::string> > ans;
  std::map<std::vector<int>, std::vector<std::string> > m;
  for (int i = 0; i < strs.size(); i++) {
    std::vector<int> cur;
    for (int j = 1; j < strs[i].size(); j++) {
      int dis = strs[i][j] - strs[i][j - 1];
      if (dis < 0) dis += 26;
      cur.push_back(dis);
    }
    if (!m.count(cur)) m[cur] = std::vector<std::string>();
    m[cur].push_back(strs[i]);
  }
  for (std::map<std::vector<int>, std::vector<std::string> >::iterator i = m.begin();
       i != m.end(); i++) {
    ans.push_back(i->second);
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("abc");
  vec.push_back("bcd");
  vec.push_back("acef");
  vec.push_back("xyz");
  vec.push_back("az");
  vec.push_back("ba");
  vec.push_back("a");
  vec.push_back("z");

  std::vector<std::vector<std::string> > matrix = GroupStrings(vec);
  LOG(INFO) << JoinMatrix(&matrix);
  return 0;
}

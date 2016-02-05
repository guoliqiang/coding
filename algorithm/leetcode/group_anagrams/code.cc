// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-24 23:22:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<std::vector<std::string> > GroupAnagrams(std::vector<std::string>& strs) {
    std::sort(strs.begin(), strs.end());

    std::map<std::string, std::vector<std::string> > dict;
    for (int i = 0; i < strs.size(); i++) {
        std::string cur = strs[i];
        std::sort(cur.begin(), cur.end());
        if (!dict.count(cur)) dict[cur] = std::vector<std::string>();
        dict[cur].push_back(strs[i]);
    }
    std::vector<std::vector<std::string> > ans;
    for (std::map<std::string, std::vector<std::string> >::iterator
         i = dict.begin(); i != dict.end(); i++) {
        ans.push_back(i->second);
    }
    return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

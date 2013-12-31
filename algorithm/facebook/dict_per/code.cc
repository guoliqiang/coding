// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 16:32:04
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Trace(std::string & path, std::vector<std::string> & rs, std::map<char, std::string> & dict, int k) {
  if (k == path.size()) {
    rs.push_back(path);
  } else {
    Trace(path, rs, dict, k + 1);
    char ch = path[k];
    if (dict.count(ch)) {
      for (int i = 0; i < dict[ch].size(); i++) {
        path[k] = dict[ch][i];
        Trace(path, rs, dict, k + 1);
      }
      path[k] = ch;
    }
  }
}

std::vector<std::string> Trace(std::string & str, std::map<char, std::string> & dict) {
  std::vector<std::string> rs;
  Trace(str, rs, dict, 0);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "fab";
  std::map<char, std::string> dict;
  dict['f'] = "F4";
  dict['b'] = "B8";
  std::vector<std::string> rs = Trace(str, dict);
  LOG(INFO) << JoinVector(rs);
  return 0;
}

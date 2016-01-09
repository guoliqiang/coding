// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 13:01:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int StringToInt(const std::string & str) {
  return atoi(str.c_str());
}

std::string IntToString(int v) {
  char buff[100];
  sprintf(buff, "%d", v);
  return std::string(buff);
}

void Get(std::vector<std::string> & path, std::vector<std::string> & ans,
         std::string & word, int idx, bool flag) {
  if (idx == word.size()) {
    std::string rs;
    for (int i = 0; i < path.size(); i++) rs += path[i];
    ans.push_back(rs);
    return;
  }
  path.push_back(std::string(1, word[idx]));
  Get(path, ans, word, idx + 1, false);
  path.pop_back();

  if (flag) {
    int cur = StringToInt(path.back()) + 1;
    path[path.size() - 1] = IntToString(cur);
    Get(path, ans, word, idx + 1, true);
  } else {
    path.push_back(std::string(1, '1'));
    Get(path, ans, word, idx + 1, true);
    path.pop_back();
  }
}

std::vector<std::string> GenerAbbreiation(std::string & word) {
  std::vector<std::string> ans;
  std::vector<std::string> path;
  Get(path, ans, word, 0, false);
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string word = "wordwordwordwordword";
  LOG(INFO) << JoinVector(GenerAbbreiation(word));
  return 0;
}

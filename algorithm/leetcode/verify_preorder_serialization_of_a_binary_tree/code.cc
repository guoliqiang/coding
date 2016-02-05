// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-02 00:43:38
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsValidSerialization(std::string preorder) {
  std::vector<std::string> vec;
  std::string cur;
  for (int i = 0; i < preorder.size(); i++) {
    if (preorder[i] == ',') {
      if (cur.size()) vec.push_back(cur);
      cur.clear();
    } else {
      cur.push_back(preorder[i]);
    }
  }
  if (cur.size()) vec.push_back(cur);

  int in = 0;
  int out = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == "#") out++;
    else in++;
    if (i != vec.size() - 1 && out > in) return false;
  }
  return out == in + 1;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

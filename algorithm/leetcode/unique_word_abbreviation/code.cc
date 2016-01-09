// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 15:54:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string IntToString(int v) {
  char buff[100];
  sprintf(buff, "%d", v);
  return std::string(buff);
}

class ValidWordAbbr {
 public:
  ValidWordAbbr(std::vector<std::string> &dictionary) {
    for (int i = 0; i < dictionary.size(); i++) {
      if (dictionary[i].size() <= 2) continue;
      std::string cur = dictionary[i].substr(0, 1) + IntToString(dictionary[i].size() - 2)
        + dictionary[i].substr(dictionary[i].size() - 1, 1);
      dict.insert(cur);
    }
  }

  bool isUnique(std::string word) {
    if (word.size() <= 2) return false;
    std::string cur = word.substr(0, 1) + IntToString(word.size() - 2) + word.substr(word.size() - 1, 1);
    return dict.count(cur) ? false : true;
  }
 private:
    std::set<std::string> dict;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("deer");
  vec.push_back("door");
  vec.push_back("cake");
  vec.push_back("card");
  ValidWordAbbr foo(vec);
  LOG(INFO) << foo.isUnique("dear");
  LOG(INFO) << foo.isUnique("cart");
  LOG(INFO) << foo.isUnique("cane");
  LOG(INFO) << foo.isUnique("make");
  return 0;
}

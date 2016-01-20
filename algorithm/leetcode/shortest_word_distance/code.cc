// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 17:06:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int ShortestDistance(std::vector<std::string>& words, std::string word1, std::string word2) {
  std::map<std::string, int> pre;
  pre[word1] = -1;
  pre[word2] = -1;

  int ans = INT_MAX;
  for (int i = 0; i < words.size(); i++) {
    if (words[i] == word1) {
      if (pre[word2] != -1) ans = std::min(ans, i - pre[word2]);
      pre[word1] = i;
    }
    if (words[i] == word2) {
      if (pre[word1] != -1) ans = std::min(ans, i - pre[word1]);
      pre[word2] = i;
    }
  }
  return ans;
}
}  // namespace algorithm

namespace NB {

int ShortestDistance(std::vector<std::string>& words, std::string word1, std::string word2) {
  int p1 = -1;
  int p2 = -1;
  int min = INT_MAX;
  for (int i = 0; i < words.size(); i++) {
    if (words[i] == word1) p1 = i;
    if (words[i] == word2) p2 = i;
    if (p1 != -1 && p2 != -1) min = std::min(min, int(fabs(p1 - p2)));
  }
  return min;
}
}  // namespace NB
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("practice");
  vec.push_back("makes");
  vec.push_back("perfect");
  vec.push_back("coding");
  vec.push_back("makes");
  LOG(INFO) << ShortestDistance(vec, "coding", "practice");
  LOG(INFO) << ShortestDistance(vec, "makes", "coding");
  return 0;
}

// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 17:06:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class WordDistance {
 public:
  WordDistance(std::vector<std::string> words) {
    for (int i = 0; i < words.size(); i++) {
      if (!dict.count(words[i])) dict[words[i]] = std::vector<int> ();
      dict[words[i]].push_back(i);
    }
  }

  int shortest(std::string word1, std::string word2) {
    int ans = INT_MAX;
    std::vector<int> & v1 = dict[word1];
    std::vector<int> & v2 = dict[word2];
    int i = 0;
    int j = 0;
    while (i < v1.size() && j < v2.size()) {
      if (v1[i] < v2[j]) {
        ans = std::min(v2[j] - v1[i], ans);
        i++;
      } else {
        ans = std::min(v1[i] - v2[j], ans);
        j++;
      }
    }
    return ans;
  }
  std::map<std::string, std::vector<int> > dict;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("practice");
  vec.push_back("makes");
  vec.push_back("perfect");
  vec.push_back("coding");
  vec.push_back("makes");
  WordDistance wd(vec);
  LOG(INFO) << wd.shortest("coding", "makes");
  return 0;
}

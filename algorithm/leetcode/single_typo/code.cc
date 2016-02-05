// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-21 10:48:14
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct TrieNode {
  int cnt;
  std::map<char, TrieNode *> child;
};

bool Match(const std::string & str, TrieNode * root) {
  for (int i = 0; i < str.size(); i++) {
    if (!root->child.count(str[i])) return false;
    root = root->child[str[i]];
  }
  return root->cnt > 0;
}

bool Match(const std::string & str, int idx, TrieNode * root) {
  if (idx == str.size()) return true;

  for (int i = 0; i < str.size(); i++) {
    for (std::map<char, TrieNode*>::iterator j = root->child.begin();
         j != root->child.end(); j++) {
      if (Match(str.substr(i + 1), j->second)) return true;
    }
    if (!root->child.count(str[i])) return false;
    root = root->child[str[i]];
  }
  return root->cnt > 0;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

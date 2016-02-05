// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-30 17:10:27
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct Node {
  std::string val;
  std::map<char, Node*> child;
};

void Build(const std::string & word, Node * root) {
  for (int i = 0; i < word.size(); i++) {
    if (!root->child.count(word[i])) root->child[word[i]] = new Node();
    root = root->child[word[i]];
  }
  root->val = word;
}

void Get(Node * root, std::vector<std::string> & ans) {
  if (root->val.size()) ans.push_back(root->val);
  for (std::map<char, Node*>::iterator i = root->child.begin();
       i != root->child.end(); i++) {
    Get(i->second, ans);
  }
}

void Match(const std::string & str, int idx, Node * root, std::vector<std::string> & ans) {
  if (idx == str.size()) Get(root, ans);
  else {
    for (std::map<char, Node*>::iterator i = root->child.begin(); i != root->child.end(); i++) {
      if (i->first >= 'A' && i->first <= 'Z') {
        if (i->first == str[idx]) Match(str, idx + 1, i->second, ans);
      } else {
        if (str[idx] >= 'A' && str[idx] <= 'Z') Match(str, idx, i->second, ans);
        else {
          if (i->first == str[idx]) Match(str, idx + 1, i->second, ans);
        }
      }
    }
  }
}

class Test {
 public:
  Test() { root = new Node(); }
  void Add(const std::string & str) {
    Build(str, root);
  }
  std::vector<std::string> Query(const std::string & str) {
    std::vector<std::string> ans;
    Match(str, 0, root, ans);
    return ans;
  }

 private:
  Node * root;
};


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  Test foo;
  foo.Add("TooBar");
  foo.Add("ToBarTest");
  foo.Add("TooTestBar");

  LOG(INFO) << JoinVector(foo.Query("TB"));
  LOG(INFO) << JoinVector(foo.Query("TooB"));
  return 0;
}

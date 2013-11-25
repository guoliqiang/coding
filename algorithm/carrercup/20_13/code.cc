// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 21:41:37
// File  : code.cc
// Brief :

/*
 * Given a dictionary of millions of words, give an algorithm to find the largest possible
 * rectangle of letters such that every row forms a word (reading left to right) and every
 * column forms a word (reading top to bottom).
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct Node {
  char val;
  std::map<char, Node *> child;
  Node(char v) : val(v){};
};

void Build(std::string & word, int i, Node * root) {
  if (i == word.size()) return;
  if (!root->child.count(word[i])) root->child[word[i]] = new Node(word[i]);
  Build(word, i + 1, root->child[word[i]]);
}

Node * Build(std::vector<std::string> & v) {
  Node * root = new Node(' ');
  for (int i = 0; i < v.size(); i++) {
    Build(v[i], 0, root);
  }
  return root;
}

Node * Build(std::set<std::string> & v) {
  std::vector<std::string> foo(v.begin(), v.end());
  return Build(foo);
}

bool IsPrefix(std::string & word, int i, Node * root) {
  if (i == word.size()) return true;
  if (!root->child.count(word[i])) return false;
  return IsPrefix(word, i + 1, root);
}

bool IsPrefix(std::string & word, Node * root) {
  return IsPrefix(word, 0, root);
}

// trie complete


void BuildDict(std::vector<std::string> & v,
               std::map<int, std::set<std::string> > & dict) {
  for (int i = 0; i < v.size(); i++) {
    int l = v[i].size();
    if (!dict.count(l)) dict.insert(std::make_pair(l, std::set<std::string>()));
    dict[l].insert(v[i]);
  }
}



bool CheckPrefix(std::vector<std::string> & v, Node * root) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  for (int i = 0; i < n; i++) {
    std::string foo;
    for (int j = 0; j < m; j++) foo.push_back(v[j][i]);
    if (!IsPrefix(foo, root)) return false;
  }
  return true;
}

bool MakeRetangle(std::set<std::string> & v, Node * root, std::vector<std::string> & path, int n) {
  // 此时trie树中word的长度都为n，因此当n == 0 时，其是出现的完整的单词
  if (n == 0) return true;
  for (std::set<std::string>::iterator i = v.begin(); i != v.end(); i++) {
    path.push_back(*i);
    if (CheckPrefix(path, root) && MakeRetangle(v, root, path, n - 1)) return true;
    path.pop_back();
  }
  return false;
}

std::vector<std::string> MaxRetangle(std::vector<std::string> & v) {
  std::vector<std::string> rs;
  std::map<int, std::set<std::string> > dict;
  BuildDict(v, dict);
  int max_len = dict.rbegin()->first;
  int tsize = max_len * max_len;
  for (int size = tsize; size > 0; size--) {
    for (int i = 1; i <= max_len; i++) {
      if (size % i == 0) {
        int j = size / i;
        if (dict.count(i) && dict.count(j)) {
          Node * root = Build(dict[j]);
          if (MakeRetangle(dict[i], root, rs, j)) return rs;
        }
      }
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> v;
  v.push_back("aa");
  v.push_back("aa");
  std::vector<std::string> rs = MaxRetangle(v);
  LOG(INFO) << JoinVector(rs);
  return 0;
}

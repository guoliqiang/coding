// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 22:40:27
// File  : code.cc
// Brief :

/*
求n个串的最长公共字串用后缀树
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TreeNode {
  char ch;
  std::map<char, TreeNode *> child;
  std::set<int> flag;
  TreeNode (char c): ch(c) {};
  TreeNode() {}
} Root;

void Build(TreeNode * father, const char * ptr, int n) {
  if (*ptr == '\0') return;
  char ch = *ptr;
  if (!(father->child.count(ch))) father->child[ch] = new TreeNode(ch);
  father->child[ch]->flag.insert(n);
  Build(father->child[ch], ptr + 1, n);
}

void Trace(TreeNode * root, int n, std::string & path, std::string & rs) {
  if (root->flag.size() == n) {
    path.append(1, root->ch);
    if (path.size() > rs.size()) rs = path;
    for (std::map<char, TreeNode*>::iterator i = root->child.begin(); i != root->child.end(); i++) {
      Trace(i->second, n, path, rs);
    }
    path.resize(path.size() - 1);
  }
}

std::string LCS(int n) {
  std::string rs = "";
  for (std::map<char, TreeNode*>::iterator i = Root.child.begin(); i != Root.child.end(); i++) {
    std::string path = "";
    Trace(i->second, n, path, rs);
  }
  return rs;
}

void Build(std::vector<std::string> & strs) {
  for (int i = 0; i < strs.size(); i++) {
    for (int j = 0; j < strs[i].size(); j++) {
      Build(&Root, strs[i].substr(j).c_str(), i);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<std::string> v;
  v.push_back("antedraon");
  v.push_back("rabelatedly");
  v.push_back("rabigotedlray");
  v.push_back("closerafistedly");
  v.push_back("coldtahearatedly");
  Build(v);
  LOG(INFO) << LCS(v.size());
  return 0;
}

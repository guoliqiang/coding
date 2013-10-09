// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 11:20:24
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

struct Node {
  char val;
  Node * failure;
  bool end;
  std::map<char, Node*> children;
  Node(char v = ' ') : val(v), failure(NULL), end(false) {}
};

void Build(const std::string & word, int i, Node * root) {
  if (i == word.size()) {
    root->end = true;
    return;
  } else {
    if (!root->children.count(word[i]))
      root->children[word[i]] = new Node(word[i]);
    Build(word, i + 1, root->children[word[i]]);
  }
}

Node * Build(std::set<std::string> words) {
  Node * root = new Node();
  for (std::set<std::string>::iterator i = words.begin();
       i != words.end(); i++) {
    if ((*i).size() != 0) Build(*i, 0, root);
  }
  return root;
}

/*
 * Node 节点是可以省略val的，这是Node节点中省略val的建立failure的函数
 *
 * */
void BuildFailure(Node * n, Node * root, char ch) {
  if (root == NULL) return;
  if (root->children.size() == 0) return;
  if (n == root) n->failure = root;
  for (std::map<char, Node*>::iterator i = n->children.begin();
       i != n->children.end(); i++) {
    Node * t = n->failure;
    while (t != root && !(t->children.count(ch))) t = t->failure;
    if (ch == ' ') i->second->failure = root;
    else {
      if (t->children.count(ch)) i->second->failure = t->children[ch];
      else i->second->failure = root;
    }
    BuildFailure(i->second, root, i->first);
  }
}

void BuildFailure(Node * n, Node * root) {
  if (root == NULL) return;
  if (root->children.size() == 0) return;
  if (n == root) n->failure = root;
  for (std::map<char, Node*>::iterator i = n->children.begin();
       i != n->children.end(); i++) {
    Node * t = n->failure;
    while (t != root && !(t->children.count(n->val))) t = t->failure;
    if (t->children.count(n->val)) i->second->failure = t->children[n->val];
    else i->second->failure = root;
    BuildFailure(i->second, root);
  }
}
void BuildFailure(Node * root) {
  // BuildFailure(root, root, ' ');
  BuildFailure(root, root);
}

void Match(std::string & str, Node * root, std::set<std::string> & v) {
  int b = 0;
  int e = 0;
  Node * t = root;
  while (b < str.size()) {
    if (t->end == true) {
      v.insert(str.substr(b, e - b));
    };
    if (e == str.size()) {
      b++;
      e = b;
      t = root;
      continue;
    }

    char ch = str[e];
    if (t->children.count(ch)) {
      t = t->children[ch];
      e++;
    } else {
      t = t->failure;
    }
    if (t == root) {
      b++;
      e = b;
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::set<std::string> pattern;
  std::string str = "my name is ieven";
  pattern.insert("is");
  pattern.insert("m");
  pattern.insert("ieven");
  pattern.insert("ies");
  Node * root = Build(pattern);
  BuildFailure(root);
  std::set<std::string> rs;
  Match(str, root, rs);
  for (std::set<std::string>::iterator i = rs.begin(); i != rs.end(); i++) {
    LOG(INFO) << *i;
  }
  return 0;
}

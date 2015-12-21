// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 16:10:14
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct Node {
  Node() : val(0) {}

  int val;
  std::map<char, Node*> child;
};

class WordDictionary {
 public:
  WordDictionary() {
    root = new Node();
  }
  // Adds a word into the data structure.
  void addWord(std::string word) {
    Node * node = root;
    for (int i = 0; i< word.size(); i++) {
      if (!(node->child.count(word[i]))) {
        node->child[word[i]] = new Node();
      }
      node = node->child[word[i]];
    }
    node->val++;
  }

  // Returns if the word is in the data structure. A word could
  // contain the dot character '.' to represent any one letter.
  bool search(std::string word) {
    return search(word, 0, root);
  }

  bool search(const std::string & word, int index, Node * node) {
    if (index == word.size()) return node->val > 0;
    if (word[index] == '.') {
      for (std::map<char, Node*>::iterator i = node->child.begin();
          i != node->child.end(); i++) {
        if (search(word, index + 1, i->second)) return true;
      }
    } else {
      if (node->child.count(word[index]) &&
          search(word, index + 1, node->child[word[index]])) {
        return true;
      }
    }
    return false;
  }

  Node * root;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  WordDictionary wd;
  wd.addWord("bad");
  wd.addWord("dad");
  wd.addWord("mad");
  wd.addWord("ba");
  LOG(INFO) << wd.search("pad");
  LOG(INFO) << wd.search("bad");
  LOG(INFO) << wd.search(".ad");
  LOG(INFO) << wd.search("b..");
  LOG(INFO) << wd.search("ba");
  LOG(INFO) << wd.search(".a");
  return 0;
}

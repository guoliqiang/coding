// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 14:59:43
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

class TrieNode {
 public:
  // Initialize your data structure here.
  TrieNode() {
    val = 0;
  }
  int val;
  std::map<char, TrieNode *> child;
};

class Trie {
 public:
  Trie() {
    root = new TrieNode();
  }

  // Inserts a word into the trie.
  void insert(std::string word) {
    TrieNode * node = root;
    for (int i = 0; i < word.size(); i++) {
      if (!(node->child.count(word[i]))) {
        node->child[word[i]] = new TrieNode();
      }
      node = node->child[word[i]];
    }
    node->val++;
  }

  // Returns if the word is in the trie.
  bool search(std::string word) {
    TrieNode * node = root;
    for (int i = 0; i < word.size(); i++) {
      if (!(node->child.count(word[i]))) return false;
      node = node->child[word[i]];
    }
    return node->val > 0;
  }

  // Returns if there is any word in the trie
  // that starts with the given prefix.
  bool startsWith(std::string prefix) {
    TrieNode * node = root;
    for (int i = 0; i < prefix.size(); i++) {
      if (!(node->child.count(prefix[i]))) return false;
      node = node->child[prefix[i]];
    }
    return true;
  }

 private:
  TrieNode* root;
};

}  // namespace algorithm

namespace twice {
using namespace std;

class TrieNode {
public:
    // Initialize your data structure here.
    TrieNode() {
        end = false;
    }
    bool end;
    std::map<char, TrieNode *> child;
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    // Inserts a word into the trie.
    void insert(string word) {
        TrieNode * cur = root;
        for (int i = 0; i < word.size(); i++) {
            if (!(cur->child.count(word[i]))) {
                cur->child[word[i]] = new TrieNode();
            }
            cur = cur->child[word[i]];
        }
        cur->end = true;
    }

    // Returns if the word is in the trie.
    bool search(string word) {
        TrieNode * cur = root;
        for (int i = 0; i < word.size(); i++) {
            if (!(cur->child.count(word[i]))) return false;
            cur = cur->child[word[i]];
        }
        return cur->end;
    }

    // Returns if there is any word in the trie
    // that starts with the given prefix.
    bool startsWith(string prefix) {
        TrieNode * cur = root;
        for (int i = 0; i < prefix.size(); i++) {
            if (!(cur->child.count([prefix[i]]))) return false;
            cur = cur->child[prefix[i]];
        }
        return true;
    }

private:
    TrieNode* root;
};
}  // namespace twice

using namespace algorithm;
int main(int argc, char** argv) {
  algorithm::Trie trie;
  trie.insert("ab");
  trie.search("abc");
  trie.search("ab");
  trie.startsWith("abc");
  trie.startsWith("ab");
  trie.insert("ab");
  trie.search("abc");
  return 0;
}

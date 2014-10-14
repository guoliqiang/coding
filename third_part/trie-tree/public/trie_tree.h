// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (PENG YongYi)
// Modifier : LiqiangGuo
//            replace string with string 16
#ifndef TRIE_TREE_H_
#define TRIE_TREE_H_

#include <climits>
#include <map>
#include <string>
#include <set>
#include <vector>

#include "../../../base/public/logging.h"
#include "../../../base/public/string16.h"
#include "../../../base/public/utf_string_conversions.h"

using namespace std;
using namespace base;


namespace trie_tree {

template <typename Val>
class TrieTree {
 public:
  TrieTree() {
    root_ = NULL;
    min_trie_ = INT_MAX;
  }
  ~TrieTree() {
    delete root_;
  }

  // put a value identified by key into trie tree.
  // if value is already exists, trie tree is not changed.
  // if key is shorter than GetMinTrie(), GetMinTrie value is changed
  void Put(const string16& key, Val& value);

  void Put(const string& key, Val& value) {
    Put(UTF8ToUTF16(key), value);
  }

  // remove all values identified by key
  // may be cause GetMinTrie() value changed.
  void Remove(const string16& key);

  void Remove(const string& key) {
    Remove(UTF8ToUTF16(key));
  }
  // remove all values identified by key.
  // may be cause GetMinTrie() value changed
  // if the value is the only identified by given key.
  void Remove(const string16& key, Val& value);

  void Remove(const string& key, Val& value) {
    Remove(UTF8ToUTF16(key), value);
  }

  //////////// Find //////////////
  // return true if there is a prefix in tree
  bool Find(const string16& key) const {
    return Find(key, 0);
  }

  bool Find(const string& key) const {
    return Find(key, 0);
  }
  // return true if there is a prefix in tree
  bool Find(const string16& key, int begin_pos) const {
    return FindInternal(key, begin_pos, false);
  }

  bool Find(const string& key, int begin_pos) const {
    return FindInternal(key, begin_pos, false);
  }
  // return true if there is a matched string in tree
  bool FullMatchFind(const string16& key) const {
    return FullMatchFind(key, 0);
  }

  bool FullMatchFind(const string& key) const {
    return FullMatchFind(key, 0);
  }
  // return true if there is a matched string in tree
  bool FullMatchFind(const string16& key, int begin_pos) const {
    return FindInternal(key, begin_pos, true);
  }

  bool FullMatchFind(const string& key, int begin_pos) const {
    return FindInternal(key, begin_pos, true);
  }

  //////////// Get //////////////
  // get all values identified by longgest prefix of key to result
  void Get(const string16& key, vector<Val>* result) const {
    Get(key, 0, result);
  }

  void Get(const string& key, vector<Val>* result) const {
    Get(key, 0, result);
  }
  // get all values identified by substring of key from begin_pos into result
  void Get(const string16& key, int begin_pos, vector<Val>* result) const {
    GetInternal(key, begin_pos, false, result);
  }

  void Get(const string& key, int begin_pos, vector<Val>* result) const {
    GetInternal(key, begin_pos, false, result);
  }
  // push all values identified by key to result
  void FullMatchGet(const string16& key, vector<Val>* result) const {
    FullMatchGet(key, 0, result);
  }

  void FullMatchGet(const string& key, vector<Val>* result) const {
    FullMatchGet(key, 0, result);
  }
  // push all values identified by key from begin_pos into result
  void FullMatchGet(const string16& key,
                    int begin_pos, vector<Val>* result) const {
    GetInternal(key, begin_pos, true, result);
  }

  void FullMatchGet(const string& key,
                    int begin_pos, vector<Val>* result) const {
    GetInternal(key, begin_pos, true, result);
  }
  // push all values identified by key to result
  void SubTreeGet(const string16& key, vector<Val>* result) const {
    SubTreeGet(key, 0, result);
  }

  void SubTreeGet(const string& key, vector<Val>* result) const {
    SubTreeGet(key, 0, result);
  }
  // push all values identified by key from begin_pos into result
  void SubTreeGet(const string16& key, int begin_pos, vector<Val>* result) const;
  void SubTreeGet(const string& key, int begin_pos, vector<Val>* result) const {
    SubTreeGet(UTF8ToUTF16(key), begin_pos, result);
  }

  int GetMinTrie() const {
    CHECK_GE(min_trie_, 0);
    return min_trie_;
  };

 private:
  struct Node {
    Node() {
      ch = '\0';
      parent = NULL;
      char16_table = new map<char16, Node*>();
      values = new set<Val>();
    };
    ~Node() {
      delete values;
      typename map<char16, Node*>::iterator it = char16_table->begin();
      for (; it != char16_table->end(); ++it) {
        delete it->second;
      }
      delete char16_table;
    };
    char16 ch;
    Node* parent;
    map<char16, Node*>* char16_table;
    set<Val>* values;
  };
  void RemoveNode(Node* node);
  int CountMinTrie(Node* node) const;
  bool FindInternal(const string16 &key, int begin_pos, bool full_match) const;

  bool FindInternal(const string &key, int begin_pos, bool full_match) const {
    return FindInternal(UTF8ToUTF16(key), begin_pos, full_match);
  }

  void GetInternal(const string16 &key, int begin_pos, bool full_match,
                   vector<Val> *result) const;

  void GetInternal(const string &key, int begin_pos, bool full_match,
                   vector<Val> *result) const {
    GetInternal(UTF8ToUTF16(key), begin_pos, full_match, result);
  }

  Node* root_;
  int min_trie_;
  set<Val> empty_set_;
  DISALLOW_COPY_AND_ASSIGN(TrieTree);
};

}  // namespace trie_tree

#endif  // TRIE_TREE_H_

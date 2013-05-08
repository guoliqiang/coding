// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (PENG YongYi)
// Modifier : LiqiangGuo
//            replace string with string 16
#ifndef TRIE_TREE_UNION_H_
#define TRIE_TREE_UNION_H_

#include <climits>
#include <map>
#include <string>
#include <set>
#include <vector>

#include "../../../base/public/logging.h"

using namespace std;
using namespace base;


namespace trie_tree {

template <typename Key, typename Val>
class TrieTreeUnion {
 public:
  TrieTreeUnion() {
    root_ = NULL;
    min_trie_ = INT_MAX;
  }
  ~TrieTreeUnion() {
    delete root_;
  }

  // put a value identified by key into trie tree.
  // if value is already exists, trie tree is not changed.
  // if key is shorter than GetMinTrie(), GetMinTrie value is changed
  void Put(const vector<Key> &key, Val& value);

  // remove all values identified by key
  // may be cause GetMinTrie() value changed.
  void Remove(const vector<Key> &key);

  // remove all values identified by key.
  // may be cause GetMinTrie() value changed
  // if the value is the only identified by given key.
  void Remove(const vector<Key>& key, Val& value);

  //////////// Find //////////////
  // return true if there is a prefix in tree
  bool Find(const vector<Key>& key) const {
    return Find(key, 0);
  }

  // return true if there is a prefix in tree
  bool Find(const vector<Key>& key, int begin_pos) const {
    return FindInternal(key, begin_pos, false);
  }

  // return true if there is a matched string in tree
  bool FullMatchFind(const vector<Key>& key) const {
    return Find(key, 0);
  }

  // return true if there is a matched string in tree
  bool FullMatchFind(const vector<Key>& key, int begin_pos) const {
    return FindInternal(key, begin_pos, true);
  }


  //////////// Get //////////////
  // get all values identified by longgest prefix of key to result
  void Get(const vector<Key>& key, vector<Val>* result) const {
    Get(key, 0, result);
  }

  // get all values identified by substring of key from begin_pos into result
  void Get(const vector<Key>& key, int begin_pos, vector<Val>* result) const {
    GetInternal(key, begin_pos, false, result);
  }

  // push all values identified by key to result
  void FullMatchGet(const vector<Key>& key, vector<Val>* result) const {
    FullMatchGet(key, 0, result);
  }

  // push all values identified by key from begin_pos into result
  void FullMatchGet(const vector<Key>& key,
                    int begin_pos, vector<Val>* result) const {
    GetInternal(key, begin_pos, true, result);
  }

  // push all values identified by key to result
  void SubTreeGet(const vector<Key>& key, vector<Val>* result) const {
    SubTreeGet(key, 0, result);
  }

  // push all values identified by key from begin_pos into result
  void SubTreeGet(const vector<Key>& key, int begin_pos, vector<Val>* result) const;

  int GetMinTrie() const {
    CHECK_GE(min_trie_, 0);
    return min_trie_;
  };

 private:
  struct NodeUnion {
    NodeUnion() {
      parent = NULL;
      key_table = new map<Key, NodeUnion*>();
      values = new set<Val>();
    };
    ~NodeUnion() {
      delete values;
      typename map<Key, NodeUnion*>::iterator it = key_table->begin();
      for (; it != key_table->end(); ++it) {
        delete it->second;
      }
      delete key_table;
    };
    Key ch;
    NodeUnion* parent;
    map<Key, NodeUnion*>* key_table;
    set<Val>* values;
  };
  void RemoveNodeUnion(NodeUnion* node);
  int CountMinTrie(NodeUnion* node) const;
  bool FindInternal(const vector<Key> &key, int begin_pos, bool full_match) const;

  void GetInternal(const vector<Key> &key, int begin_pos, bool full_match,
                   vector<Val> *result) const;

  NodeUnion* root_;
  int min_trie_;
  set<Val> empty_set_;
  DISALLOW_COPY_AND_ASSIGN(TrieTreeUnion);
};

}  // namespace trie_tree

#endif  // TRIE_TREE_UNION_H_

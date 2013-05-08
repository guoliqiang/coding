// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (CHANGE TO YOUR NAME)
#ifndef TRIE_TREE_INL_UNION_H_
#define TRIE_TREE_INL_UNION_H_

#include <climits>
#include <map>
#include <string>
#include <set>
#include <vector>
#include "../public/trie_tree_union.h"

namespace trie_tree {
//
template<typename Key, typename Val>
void TrieTreeUnion<Key, Val>::Put(const vector<Key>& key, Val& value) {
  if (key.size() < min_trie_) {
    min_trie_ = key.size();
  }
  if (root_ == NULL) {
    root_ = new NodeUnion();
  }
  NodeUnion* node = root_;
  for (typename vector<Key>::const_iterator it = key.begin(); it != key.end(); ++it) {
    Key ch = *it;
    NodeUnion* temp = (*node->key_table)[ch];
    if (temp == NULL) {
      temp = new NodeUnion();
      temp->ch = ch;
      temp->parent = node;
      (*node->key_table)[ch] = temp;
    }
    node = temp;
  }
  if (node->values == NULL) {
    node->values = new set<Val> ();
  }
  node->values->insert(value);
}

//
template<typename Key, typename Val>
bool TrieTreeUnion<Key, Val>::FindInternal(const vector<Key> &key,
                                 int begin_pos,
                                 bool full_match) const {
  if (root_ == NULL) return false;
  const NodeUnion* node = root_;
  for (typename vector<Key>::const_iterator it = key.begin() + begin_pos; it != key.end();
       ++it) {
    typename map<Key, NodeUnion*>::const_iterator tmp = node->key_table->find(*it);
    if (tmp == node->key_table->end())
      return false;
    node = tmp->second;
    if (!full_match && node->values != NULL && node->values->size() > 0)
      return true;
  }
  if (!full_match)
    return false;
  return node->values->size() > 0;
}

//
template<typename Key, typename Val>
void TrieTreeUnion<Key, Val>::GetInternal(const vector<Key>& key,
                                int begin_pos,
                                bool full_match,
                                vector<Val>* result) const {
  if (root_ == NULL) return;
  const NodeUnion* node = root_;
  for (typename vector<Key>::const_iterator it = key.begin() + begin_pos; it != key.end();
      ++it) {
    typename map<Key, NodeUnion*>::const_iterator tmp = node->key_table->find(*it);
    if (tmp == node->key_table->end())
      return;
    node = tmp->second;
    if (!full_match && node->values != NULL && node->values->size() > 0) {
      for (typename set<Val>::const_iterator it = node->values->begin();
           it != node->values->end(); ++it)
        result->push_back(*it);
    }
  }
  if (full_match && node->values != NULL && node->values->size() > 0) {
    for (typename set<Val>::const_iterator it = node->values->begin();
         it != node->values->end(); ++it)
      result->push_back(*it);
  }
}

//
template<typename Key, typename Val>
void TrieTreeUnion<Key, Val>::SubTreeGet(const vector<Key>& key,
                               int begin_pos,
                               vector<Val>* result) const {
  if (root_ == NULL) {
    return;
  }
  const NodeUnion* node = root_;
  for (typename vector<Key>::const_iterator it = key.begin() + begin_pos; it != key.end();
      ++it) {
    typename map<Key, NodeUnion*>::const_iterator tmp = node->key_table->find(*it);
    if (tmp == node->key_table->end()) {
      return;
    }
    node = tmp->second;
  }
  vector<const NodeUnion*> level_order;
  level_order.push_back(node);
  while (level_order.size() > 0) {
    node = level_order.front();
    level_order.erase(level_order.begin());
    if (node->values != NULL && node->values->size() > 0) {
      set<Val>* values = node->values;
      for (typename set<Val>::const_iterator it = values->begin();
           it != values->end(); ++it)
        result->push_back(*it);
    }
    typename map<Key, NodeUnion*>::const_iterator it = node->key_table->begin();
    for (; it != node->key_table->end(); ++it)
      level_order.push_back(it->second);
  }
}

//
template<typename Key, typename Val>
void TrieTreeUnion<Key, Val>::Remove(const vector<Key>& key) {
  if (root_ == NULL) {
    return;
  }
  NodeUnion* node = root_;
  for (typename vector<Key>::const_iterator it = key.begin(); it != key.end(); ++it) {
    Key ch = *it;
    NodeUnion* temp = (*(node->key_table))[ch];
    if (temp == NULL) {
      return;
    }
    node = temp;
  }
  RemoveNodeUnion(node);
}

template<typename Key, typename Val>
void TrieTreeUnion<Key, Val>::Remove(const vector<Key>& key, Val& value) {
  if (root_ == NULL) {
    return;
  }
  NodeUnion* node = root_;
  for (typename vector<Key>::const_iterator it = key.begin(); it != key.end(); ++it) {
    Key ch = *it;
    NodeUnion* temp = (*(node->key_table))[ch];
    if (temp == NULL) {
      return;
    }
    node = temp;
  }
  node->values->erase(value);
  if (node->values->size() <= 0) {
    RemoveNodeUnion(node);
  }
}

template<typename Key, typename Val>
void TrieTreeUnion<Key, Val>::RemoveNodeUnion(NodeUnion* n) {
  NodeUnion* node = n;
  while (node->parent) {
    NodeUnion* parent = node->parent;
    parent->key_table->erase(node->ch);
    delete node;
    if (parent->key_table->size() > 0 ||
        parent->values->size() > 0) {
      break;
    } else {
      node = parent;
    }
  }
  min_trie_ = CountMinTrie(root_) - 1;
}

//
template<typename Key, typename Val>
int TrieTreeUnion<Key, Val>::CountMinTrie(NodeUnion* n) const {
  if (n->values->size() > 0) {
    return 1;
  }
  int trie = INT_MAX;
  typename map<Key, NodeUnion*>::const_iterator it;
  for (it = n->key_table->begin(); it != n->key_table->end(); ++it) {
    int tmp = CountMinTrie(it->second);
    if (tmp < trie) {
      trie = tmp;
      if (trie <= 1) {
        break;
      }
    }
  }
  return trie + 1;
}

}  // namespace trie_tree

#endif  // TRIE_TREE_INL_H_

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (PENG YongYi)

#ifndef UTIL_PARSE_MASTER2_INTERNAL_FAST_TRIE_TREE_H_
#define UTIL_PARSE_MASTER2_INTERNAL_FAST_TRIE_TREE_H_

#include <string>

#include "base/public/logging.h"

//TODO (guoliqiang) use double-array-trie re-implement this trie-tree

namespace util {
// fast trie tree is fast on finding and slow on inserting;
// it only supports 0~127 char in string as key;
// high memory used on sparse tree;
// values in this trie tree cannot be removed.

template <typename Val>
class FastTrieTree {
 public:
  FastTrieTree() {}

  // put a value identified by key into trie tree.
  // if value is already exists, trie tree is not changed.
  // if key is shorter than GetMinTrie(), GetMinTrie value is changed
  void Put(const std::string& key, const Val* val) {
    Node* node = &root_;
    for (int i = 0; i < key.size(); ++i) {
      char ch = key[i];
      CHECK(ch >= 0 && ch <= 127);
      if (node->sub_node_size_ <= 0) {
        node->min_ch_ = ch;
        node->sub_node_size_ = 1;
        node->sub_nodes_ = new Node[node->sub_node_size_];
      } else if (ch < node->min_ch_) {
        int32 extend_size = node->min_ch_ - ch;
        int32 new_size = node->sub_node_size_ + extend_size;
        Node* new_sub_nodes = new Node[new_size];
        for (int j = 0; j < node->sub_node_size_; ++j) {
          new_sub_nodes[extend_size + j] = node->sub_nodes_[j];
          node->sub_nodes_[j].sub_nodes_ = NULL;
        }
        delete[] node->sub_nodes_;

        node->min_ch_ = ch;
        node->sub_node_size_ = new_size;
        node->sub_nodes_ = new_sub_nodes;
      } else if (ch >= node->min_ch_ + node->sub_node_size_) {
        int32 extend_size = ch - node->min_ch_ - node->sub_node_size_ + 1;
        int32 new_size = node->sub_node_size_ + extend_size;
        Node* new_sub_nodes = new Node[new_size];
        for (int j = 0; j < node->sub_node_size_; ++j) {
          new_sub_nodes[j] = node->sub_nodes_[j];
          node->sub_nodes_[j].sub_nodes_ = NULL;
        }
        delete[] node->sub_nodes_;

        node->sub_node_size_ = new_size;
        node->sub_nodes_ = new_sub_nodes;
      }
      node = node->sub_nodes_ + (ch - node->min_ch_);
    }
    node->val_ = val;
  }

  // get all values identified by longgest prefix of key to result
  inline const Val* Get(const std::string& key) const {
    return Get(key.c_str(), key.size());
  }

  const Val* Get(const char* key, int key_size) const {
    const Node* node = &root_;
    for (int i = 0; i < key_size; ++i) {
      char ch = key[i];
      if (node->sub_node_size_ <= 0) {
        return NULL;
      } else if (ch < node->min_ch_) {
        return NULL;
      } else if (ch >= node->min_ch_ + node->sub_node_size_) {
        return NULL;
      }
      node = node->sub_nodes_ + (ch - node->min_ch_);
    }
    return node->val_;
  }

 private:
  struct Node {
    Node() : min_ch_('\127'), sub_node_size_(0), sub_nodes_(NULL), val_(NULL) {
    };
    ~Node() {
      delete[] sub_nodes_;
    };
    char min_ch_;
    int8 sub_node_size_;
    Node* sub_nodes_;
    const Val* val_;
  };

  Node root_;
  DISALLOW_COPY_AND_ASSIGN(FastTrieTree);
};

}  // namespace util

#endif  // UTIL_PARSE_MASTER2_INTERNAL_FAST_TRIE_TREE_H_

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)

#include "../public/da_trie.h"

#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

#include "../../../base/public/logging.h"
using namespace std;  // NOLINT
using namespace base;  // NOLINT


void DATrieBasic::BuildFrom(const base::hash_map<std::string, int> &map) {
  // Sort first
  std::vector<std::pair<string, int> > vec;
  VLOG(2) << "Copy map...";
  vec.reserve(map.size());
  std::copy(map.begin(), map.end(), std::back_inserter(vec));
  VLOG(2) << "Sort trie entries...";
  std::sort(vec.begin(), vec.end());
  // Bug fix for 20120313: random file content crash searchroot and rewriter
  const int kMaxKeyLen = 512;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (vec[i].first.size() > kMaxKeyLen) {
      LOG(WARNING) << "Too long key: " << vec[i].first.substr(0, kMaxKeyLen);
      vec.erase(vec.begin() + i);
      i--;
    }
  }
  BuildFromSorted(vec);
}


void DATrieBasic::BuildFrom(const std::vector<std::string> &keys) {
  base::hash_map<std::string, int> map;
  for (std::vector<std::string>::const_iterator it = keys.begin();
      it < keys.end(); ++it) {
    map[*it] = 1;
  }
  BuildFrom(map);
}

void DATrieBasic::BuildFromSorted(const vector<pair<string, int> >&pairs) {
  VLOG(2) << "Prepare to build trie...";
  // Split keys and values
  std::vector<const char*> keys(pairs.size());
  std::vector<size_t> key_lens(pairs.size());
  std::vector<int> values(pairs.size());
  size_t key_len_sum = 0;
  for (size_t i = 0; i < pairs.size(); i++) {
    keys[i] = pairs[i].first.c_str();
    key_lens[i] = pairs[i].first.length();
    key_len_sum += key_lens[i];
    values[i] = pairs[i].second;
  }  // end for

  VLOG(2) << "Build trie...";
  da_.clear();
  size_t i = 0;
  while (pairs.size() > 0 && i < pairs.size() && pairs[i].first.empty()) {
    ++i;
  }
  if (i >= pairs.size()) {
    da_.clear();
    VLOG(1) << "Da Trie data is empty!";
    return;
  }
  int ret = da_.build(keys.size() - i, &keys[i], &key_lens[i], &values[i]);
  if (ret != 0) {
    da_.clear();
    LOG(FATAL) << "Build Double Array failed! " << ret;
  }
  VLOG(1) << "DA Trie Key Size:" << pairs.size() << ", Key Total Length:" <<
      key_len_sum << ", Memory use: " << da_.size() * 8 / 1024 << "K bytes.";
}

bool DATrieBasic::ExactMatchSearch(const std::string& key,
                                   int begin_pos,
                                   int* result) const {
  if (begin_pos == key.length()) return false;
  da_.exactMatchSearch(key.c_str() + begin_pos,
                       *result, key.length() - begin_pos);
  return (*result != -1);
}

bool DATrieBasic::MaxPrefixSearch(const std::string& key,
                                  int begin_pos,
                                  ResultPair* result) const {
  if (begin_pos == key.length()) return false;
  result->length = 0;
  da_.maxPrefixSearch(key.c_str() + begin_pos,
                      result, key.length() - begin_pos);
  return (result->length > 0);
}

bool DATrieBasic::MatchAny(const std::string& input) const {
  for (size_t i = 0; i < input.size(); ++i) {
    if (da_.MatchAny(input.c_str() + i, input.size() - i)) {
      return true;
    }
  }
  return false;
}

bool DATrieBasic::MatchFirst(const std::string& input, std::string* out) const {
  DATrieBasic::ResultPair pair;
  pair.length = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    if (da_.commonPrefixSearch(input.c_str() + i,
                               &pair, 1, input.size() - i)) {
      out->assign(input.c_str() + i, pair.length);
      return true;
    }
  }
  return false;
}

/*
 * Only deal with edit distance 1
 * If needed, we can  Add a new method with distance 2 and call this one
 * Procedure: travel prefix, edit and exact match the lefting
 *
 */
void DATrieBasic::LikelyMatch(const std::string input,
                              std::vector<std::string>* out) const {
  out->clear();
  if (input.empty()) return;
  ResultPair pair;
  std::string s = " " + input + " ";  // padding left and right for edit

  // Special case
  pair = da_.exactMatchSearch<ResultPair>(input.data(), input.size()- 1, 0);
  if (pair.value > 0) {
    std::string like(input.data(), input.size() - 1);
    out->push_back(like);
  }

  for (size_t i = 0; i <= input.size(); ++i) {
    size_t node_pos = 0, key_pos = 0;
    // 1. Prefix
    if (i > 0) {
      node_pos = 0;
      da_.traverse(input.data(), node_pos, key_pos, i);
      if (i != key_pos) {
        return;
      }
    } else {
      // node_pos = 1;
    }
    // 2. Insert one char
    for (char c = 'a'; c <= 'z'; ++c) {
      s[i] = c;
      pair = da_.exactMatchSearch<ResultPair>(
                     s.data() + i, input.size() - i + 1, node_pos);
      if (pair.value > 0) {
        std::string like;
        like.reserve(input.size() + 1);
        if (i > 0) {
          like.append(input.data(), i);
        }
        like.append(s.data() + i, input.size() - i + 1);
        out->push_back(like);
      }
    }
    // 3. Remove one char
    if (input.size() - i > 1) {
      pair = da_.exactMatchSearch<ResultPair>(
                     input.data() + i + 1, input.size() - i - 1, node_pos);
      if (pair.value > 0) {
        std::string like;
        like.reserve(input.size() - 1);
        if (i > 0) {
          like.append(input.data(), i);
        }
        like.append(input.data() + i + 1, input.size() - i - 1);
        out->push_back(like);
      }
    }
    // 4. Switch two chars
    if (input.size() - i > 1) {
      std::swap(s[i + 1], s[i + 2]);
      pair = da_.exactMatchSearch<ResultPair>(
                     s.data() + i + 1, input.size() - i, node_pos);
      if (pair.value > 0) {
        std::string like;
        like.reserve(input.size());
        if (i > 0) {
          like.append(input.data(), i);
        }
        like.append(s.data() + i + 1, input.size() - i);
        out->push_back(like);
      }
      std::swap(s[i + 1], s[i + 2]);
    }
    // 5. Edit one char
    if (i < input.size()) {
      for (char c = 'a'; c <= 'z'; ++c) {
        if (c == input[i]) continue;
        s[i + 1] = c;
        pair = da_.exactMatchSearch<ResultPair>(
                       s.data() + i + 1, input.size() - i, node_pos);
        if (pair.value > 0) {
          std::string like;
          like.reserve(input.size() + 1);
          if (i > 0) {
            like.append(input.data(), i);
          }
          like.append(s.data() + i + 1, input.size() - i);
          out->push_back(like);
        }
      }
    }
  }  // end of main loop
}


// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)
// double array trie classes
// Compare to normal trie:
//   cons:
//     It take long time to collapse the trie tree into double array
//     Once double array is builded, it can not be modified.
//     It suitable for data like dictionaries rarely changed.
//   prof:
//     Save large memory space when the trie tree is big.

#ifndef DOUBLE_ARRAY_TRIE_DA_TRIE_H_
#define DOUBLE_ARRAY_TRIE_DA_TRIE_H_

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "../../../base/public/logging.h"
#include "../../../base/public/hash_tables.h"
#include "darts.h"


// Use DATrieBasic if the value is UNSIGNED basic type and smaller than int
//   else use DATrie
// If you deal with chinese strings, DATrie16Basic is your choice.
class DATrieBasic {
 public:
  typedef darts::DoubleArrayImpl<char, unsigned char,
                                 int, unsigned int> DoubleArray;

  // Same as  struct ResultPair { size_t length; V value; };
  typedef DoubleArray::result_pair_type ResultPair;

  DATrieBasic() : utf16_(false) {}

  virtual ~DATrieBasic() {}

  // Cache the trie if it is build from file.
  void CacheForFile(const std::string filename);

  void BuildFrom(const base::hash_map<std::string, int> &map);

  void BuildAsUTF16From(const base::hash_map<std::string, int> &map);

  // When value is useless, build from vector directly
  void BuildFrom(const std::vector<std::string> &keys);

  bool ExactMatchSearch(const std::string& key, int* result) const {
    return ExactMatchSearch(key, 0, result);
  }

  bool MaxPrefixSearch(const std::string& key, ResultPair* result) const {
    return MaxPrefixSearch(key, 0, result);
  }

  bool ExactMatchSearch(const std::string& key, int begin_pos,
      int* result) const;

  bool MaxPrefixSearch(const std::string& key, int begin_pos,
      ResultPair* result) const;

  bool MatchAny(const std::string& input) const;

  bool MatchFirst(const std::string& input, std::string* out) const;

  // Input should be english in lower case.
  // Likely means edit distance is 1.
  void LikelyMatch(const std::string input,
                   std::vector<std::string>* out) const;

  // Low level interface, performance first
  inline void ExactMatchSearch(const char* key, size_t len,
                               ResultPair* result) const;

  inline void MaxPrefixSearch(const char* key, size_t len,
                              ResultPair* result) const;

  inline void MaxPrefixSearch(const char16* key, size_t len,
                              ResultPair* result) const {
    result->length = 0;
    da_.maxPrefixSearch(reinterpret_cast<const char*>(key), result, len * 2);
    result->length /= 2;
  }

  size_t CommonPrefixMatch(const char* key, size_t len,
                           ResultPair* result,
                           size_t result_len) const {
    return da_.commonPrefixSearch(key, result, result_len, len);
  }

  size_t CommonPrefixMatch(const char16* key, size_t len,
                           ResultPair* result,
                           size_t result_len) const {
    size_t n = da_.commonPrefixSearch(reinterpret_cast<const char*>(key),
                                      result, result_len, len * 2);
    if (n == 0) return 0;
    for (size_t i = 0; i < n; i++) {
      result[i].length /= 2;
    }
    return n;
  }

  const void* Begin() {
    return da_.array();
  }

  size_t Size() {
    return da_.total_size();
  }

  void SetArray(void *ptr, size_t raw_size) {
    da_.set_array(ptr, raw_size);
  }

  void PrintSize() {
    LOG(INFO) << "Trie size: " << da_.size();
  }

 protected:
  bool LikelyMatch(const char* key, size_t node_pos, size_t len,
                   std::vector<std::string>* out);

  void BuildFromSorted(const std::vector<std::pair<std::string, int> >&pairs);

 private:
  bool LoadFromCache();
  DoubleArray da_;
  bool utf16_;
  std::string trie_source_file_;
  std::string trie_source_md5_;
  std::string cached_file_;
};

template <class V>
class DATrie : public DATrieBasic {
 public:
  DATrie() {}
  virtual ~DATrie() {}

  void BuildFrom(const base::hash_map<std::string, V> &map);

  bool ExactMatchSearch(const std::string& key, V* result) const {
    DATrieBasic::ResultPair pair;
    DATrieBasic::ExactMatchSearch(key.c_str(), key.length(), &pair);
    if (pair.value < 0) {
      return false;
    }
    *result = values_[pair.value];
    return true;
  }

  bool ExactMatchSearch(const char* key, size_t len, V* result) const {
    DATrieBasic::ResultPair pair;
    DATrieBasic::ExactMatchSearch(key, len, &pair);
    if (pair.value < 0) {
      return false;
    }
    *result = values_[pair.value];
    return true;
  }

  const V& GetValueAt(int index) const {
    return values_[index];
  }
  size_t GetValueVecSzie() {
    return values_.size();
  }
  void AppendValue(V value) {
    values_.push_back(value);
  }
  void Reserve(size_t size) {
    values_.reserve(size);
  }

 private:
  std::vector<V> values_;
};

template <class V>
void DATrie<V>::BuildFrom(const base::hash_map<std::string, V> &map) {
  base::hash_map<std::string, int> basic_map;
  basic_map.resize(map.size());
  values_.clear();
  values_.reserve(map.size());
  typename base::hash_map<std::string, V>::const_iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    basic_map[it->first] = values_.size();
    values_.push_back(it->second);
  }
  DATrieBasic::BuildFrom(basic_map);
}

void DATrieBasic::ExactMatchSearch(const char* key, size_t len,
    ResultPair* result) const {
  result->length = 0;
  da_.exactMatchSearch(key, *result, len);
}

void DATrieBasic::MaxPrefixSearch(const char* key, size_t len,
    ResultPair* result) const {
  result->length = 0;
  da_.maxPrefixSearch(key, result, len);
}


#endif  // DOUBLE_ARRAY_TRIE_DA_TRIE_H_

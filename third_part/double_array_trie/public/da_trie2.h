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

#ifndef UTIL_DOUBLE_ARRAY_TRIE_DA_TRIE2_H_
#define UTIL_DOUBLE_ARRAY_TRIE_DA_TRIE2_H_

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "base/public/logging.h"
#include "base/public/hash_tables.h"
#include "third_part/double_array_trie/public/darts_core2.h"

namespace util {

// Use DATrieBasic2 if the value is UNSIGNED basic type and smaller than int
//   else use DATrie2
template <typename StringT>
class DATrieBasic2 : public darts::DoubleArrayCore2<StringT> {
 public:
  typedef typename darts::DoubleArrayCore2<StringT>::ResultPair ResultPair;
  DATrieBasic2() {}
  virtual ~DATrieBasic2() {}

  void Save(const std::string& filename) const;
  void Load(const std::string& filename);

  void BuildFrom(const base::hash_map<StringT, int> &map);
  // When value is useless, build from vector directly
  void BuildFrom(const std::vector<StringT> &keys);

  // Cache the trie if it is build from file.
  void CacheForFile(const std::string& filename);
  void CacheForFiles(const std::vector<std::string>& files);

  bool ExactMatch(const StringT& key, int* result) const {
    ResultPair pair;
    darts::DoubleArrayCore2<StringT>::exactMatch(
        key.c_str(), key.length(), 0, &pair);
    if (pair.length > 0) {
      *result = pair.value;
      return true;
    }
    return false;
  }

  bool MaxPrefixSearch(const StringT& key, ResultPair* result) const;

  bool MatchAny(const StringT& input) const;

  bool MatchFirst(const StringT& input, StringT* out) const;

 protected:
  typedef typename DATrieBasic2<StringT>::Unit Unit;
  void BuildFromSorted(const std::vector<std::pair<StringT, int> >&pairs);

 private:
  bool LoadFromCache();
  std::vector<std::string> trie_source_files_;
  std::string trie_source_md5_;
  std::string cached_file_;
};


template <typename StringT, typename V>
class DATrie2 : public DATrieBasic2<StringT> {
 public:
  DATrie2() {}
  virtual ~DATrie2() {}

  void BuildFrom(const base::hash_map<StringT, V> &map);

  const V& GetValueAt(int index) const {
    return values_[index];
  }

 private:
  std::vector<V> values_;
};

template <typename StringT, typename V>
void DATrie2<StringT, V>::BuildFrom(const base::hash_map<StringT, V> &map) {
  base::hash_map<StringT, int> basic_map;
  basic_map.resize(map.size());
  values_.resize(1);
  values_.reserve(map.size() + 1);
  typename base::hash_map<StringT, V>::const_iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    basic_map[it->first] = values_.size();
    values_.push_back(it->second);
  }
  DATrieBasic2<StringT>::BuildFrom(basic_map);
}

}  // namespace util

#endif  // UTIL_DOUBLE_ARRAY_TRIE_DA_TRIE2_H_

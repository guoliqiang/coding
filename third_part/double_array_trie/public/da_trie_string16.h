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

#ifndef DOUBLE_ARRAY_TRIE_DA_TRIE_STRING16_H_
#define DOUBLE_ARRAY_TRIE_DA_TRIE_STRING16_H_

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "../../../base/public/logging.h"
#include "../../../base/public/string16.h"
#include "../../../base/public/hash_tables.h"
#include "darts.h"

// Use DATrie16Basic if the value is UNSIGNED basic type and smaller than int
//   else use DATrie16
class DATrie16Basic {
 public:
  typedef darts::DoubleArrayImpl<char16, unsigned char16,
                                 int, unsigned int> DoubleArray;

  // Same as  struct ResultPair { size_t length; V value; };
  typedef DoubleArray::result_pair_type ResultPair;

  DATrie16Basic() {}

  virtual ~DATrie16Basic() {}

  void BuildFrom(const base::hash_map<std::string, int> &map);

  void BuildFrom(const base::hash_map<string16, int> &map);

  // When value is useless, build from vector directly
  void BuildFrom(const std::vector<std::string> &keys);

  void BuildFrom(const std::vector<string16> &keys);

  bool ExactMatchSearch(const std::string& key, int* result) const;

  bool ExactMatchSearch(const string16& key, int* result) const {
    return ExactMatchSearch(key, 0, result);
  }

  bool ExactMatchSearch(const string16& key, int begin_pos,
                        int* result) const;

  bool MaxPrefixSearch(const string16& key, ResultPair* result) const {
    return MaxPrefixSearch(key, 0, result);
  }

  bool MaxPrefixSearch(const string16& key, int begin_pos,
                       ResultPair* result) const;

  bool MatchAny(const std::string& input) const;

  bool MatchAny(const string16& input) const;

  bool MatchFirst(const string16& input, string16* out) const;

  // Input should be english in lower case.
  // Likely means edit distance is 1.
  void LikelyMatch(const string16 &input,
                   std::vector<string16>* out) const;

  // !! NOTICE: the string is compare with string16 internally
  void LikelyMatch(const std::string &input,
                   std::vector<std::string>* out) const;

  // Low level interface, performance first
  inline void ExactMatchSearch(const char16* key, size_t len,
                               ResultPair* result) const;

  inline void MaxPrefixSearch(const char16* key, size_t len,
                              ResultPair* result) const;

  size_t CommonPrefixMatch(const char16* key, size_t len,
                           ResultPair* result,
                           size_t result_len) const {
    return da_.commonPrefixSearch(key, result, result_len, len);
  }

  void PrintSize() {
    LOG(INFO) << "Memory use: " << da_.size() * 8 << " bytes.";
  }

 protected:
  void BuildFromSorted(const std::vector<std::pair<string16, int> > &pairs);

  bool LikelyMatch(const char16* key, size_t node_pos, size_t len,
                   std::vector<string16>* out);

  DoubleArray da_;
};

// The value in result pair is an index
// Call GetValueAt to get the value
template <class V>
class DATrie16 : public DATrie16Basic {
 public:
  DATrie16() {}
  virtual ~DATrie16() {}

  void BuildFrom(const base::hash_map<string16, V> &map);

  bool ExactMatchSearch(const string16& key, V* result) const {
    DATrie16Basic::ResultPair pair;
    DATrie16Basic::ExactMatchSearch(key.c_str(), key.length(), &pair);
    if (pair.value < 0) {
      return false;
    }
    *result = values_[pair.value];
    return true;
  }

  bool ExactMatchSearch(const char16* key, size_t len, V* result) const {
    DATrie16Basic::ResultPair pair;
    if (!DATrie16Basic::ExactMatchSearch(key, len, &pair)) {
      return false;
    }
    result = values_[pair.value];
    return true;
  }

  const V& GetValueAt(int index) const {
    return values_[index];
  }

 private:
  std::vector<V> values_;
};

template <class V>
void DATrie16<V>::BuildFrom(const base::hash_map<string16, V> &map) {
  std::vector<std::pair<string16, int> > vec;
  vec.reserve(map.size());
  values_.clear();
  values_.reserve(map.size());
  typename base::hash_map<string16, V>::const_iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    vec.push_back(std::make_pair<string16, int>(it->first, values_.size()));
    values_.push_back(it->second);
  }
  DATrie16Basic::BuildFrom(vec);
}

void DATrie16Basic::ExactMatchSearch(const char16* key, size_t len,
    ResultPair* result) const {
  result->length = 0;
  da_.exactMatchSearch(key, *result, len);
}

void DATrie16Basic::MaxPrefixSearch(const char16* key, size_t len,
    ResultPair* result) const {
  result->length = 0;
  da_.maxPrefixSearch(key, result, len);
}

#endif  // DOUBLE_ARRAY_TRIE_DA_TRIE_STRING16_H_

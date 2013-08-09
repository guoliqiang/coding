// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)

#include "../public/da_trie_string16.h"

#include <utility>
#include <string>
#include <vector>
#include <algorithm>

#include "../../../base/public/logging.h"
#include "../../../base/public/utf_string_conversions.h"

using namespace base;  // NOLINT
using namespace std;  // NOLINT


void DATrie16Basic::BuildFrom(const base::hash_map<string16, int> &map) {
  // Sort first
  std::vector<std::pair<string16, int> > vec;
  VLOG(2) << "Copy map...";
  vec.reserve(map.size());
  std::copy(map.begin(), map.end(), std::back_inserter(vec));
  VLOG(2) << "Sort trie entries...";
  std::sort(vec.begin(), vec.end());
  BuildFromSorted(vec);
}

void DATrie16Basic::BuildFrom(const base::hash_map<string, int> &map) {
  // Sort first
  std::vector<std::pair<string16, int> > vec;
  vec.reserve(map.size());
  base::hash_map<string, int>::const_iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    vec.push_back(make_pair<string16, int>(
        UTF8ToUTF16(it->first), it->second));
  }
  std::sort(vec.begin(), vec.end());
  BuildFromSorted(vec);
}

void DATrie16Basic::BuildFromSorted(const vector<pair<string16, int> >&pairs) {
  VLOG(2) << "Prepare to build trie...";
  // Split keys and values
  std::vector<const char16 *> keys(pairs.size());
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
  while (pairs.size() > 0 && pairs[i].first.empty()) {
    ++i;
  }
  int ret = da_.build(keys.size() - i, &keys[i], &key_lens[i], &values[i]);
  if (ret != 0) {
    da_.clear();
    LOG(FATAL) << "Build Double Array failed! " << ret;
  }
  VLOG(1) << "DA Trie Key Size:" << pairs.size() << ", Key Total Length:" <<
      key_len_sum << ", Memory use: " << da_.size() * 8 / 1024 << "K bytes.";
}

void DATrie16Basic::BuildFrom(const std::vector<string16> &keys) {
  base::hash_map<string16, int> map;
  for (std::vector<string16>::const_iterator it = keys.begin();
      it < keys.end(); ++it) {
    map[*it] = 1;
  }
  BuildFrom(map);
}

void DATrie16Basic::BuildFrom(const std::vector<string> &keys) {
  base::hash_map<string16, int> map;
  for (std::vector<string>::const_iterator it = keys.begin();
      it < keys.end(); ++it) {
    map[UTF8ToUTF16(*it)] = 1;
  }
  BuildFrom(map);
}

bool DATrie16Basic::ExactMatchSearch(const string& key, int* result) const {
  return ExactMatchSearch(UTF8ToUTF16(key), result);
}

bool DATrie16Basic::ExactMatchSearch(
    const string16& key, int begin_pos, int* result) const {
  if (begin_pos == key.length()) return false;
  da_.exactMatchSearch(key.c_str() + begin_pos,
      *result, key.length() - begin_pos);
  return (*result != -1);
}

bool DATrie16Basic::MaxPrefixSearch(
    const string16& key, int begin_pos, ResultPair* result) const {
  if (begin_pos == key.length()) return false;
  result->length = 0;
  da_.maxPrefixSearch(key.c_str() + begin_pos,
      result, key.length() - begin_pos);
  return (result->length > 0);
}

bool DATrie16Basic::MatchAny(const string16& input) const {
  for (size_t i = 0; i < input.size(); ++i) {
    if (da_.MatchAny(input.c_str() + i, input.size() - i)) {
      return true;
    }
  }
  return false;
}

bool DATrie16Basic::MatchAny(const std::string& input) const {
  return MatchAny(UTF8ToUTF16(input));
}

bool DATrie16Basic::MatchFirst(const string16& input, string16* out) const {
  DATrie16Basic::ResultPair pair;
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
void DATrie16Basic::LikelyMatch(const string16 &input,
      std::vector<string16>* out) const {
  out->clear();
  if (input.empty()) return;
  ResultPair pair;
  string16 s;
  // padding left and right for edit
  s.push_back(' ');
  s.append(input);
  s.push_back(' ');

  // Special case
  pair = da_.exactMatchSearch<ResultPair>(input.data(), input.size()- 1, 0);
  if (pair.value > 0) {
    string16 like(input.data(), input.size() - 1);
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
    // 2. Insert one char16
    for (char16 c = 'a'; c <= 'z'; ++c) {
      s[i] = c;
      pair = da_.exactMatchSearch<ResultPair>(
                     s.data() + i, input.size() - i + 1, node_pos);
      if (pair.value > 0) {
        string16 like;
        like.reserve(input.size() + 1);
        if (i > 0) {
          like.append(input.data(), i);
        }
        like.append(s.data() + i, input.size() - i + 1);
        out->push_back(like);
      }
    }
    // 3. Remove one char16
    if (input.size() - i > 1) {
      pair = da_.exactMatchSearch<ResultPair>(
                     input.data() + i + 1, input.size() - i - 1, node_pos);
      if (pair.value > 0) {
        string16 like;
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
        string16 like;
        like.reserve(input.size());
        if (i > 0) {
          like.append(input.data(), i);
        }
        like.append(s.data() + i + 1, input.size() - i);
        out->push_back(like);
      }
      std::swap(s[i + 1], s[i + 2]);
    }
    // 5. Edit one char16
    if (i < input.size()) {
      for (char16 c = 'a'; c <= 'z'; ++c) {
        if (c == input[i]) continue;
        s[i + 1] = c;
        pair = da_.exactMatchSearch<ResultPair>(
                       s.data() + i + 1, input.size() - i, node_pos);
        if (pair.value > 0) {
          string16 like;
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

void DATrie16Basic::LikelyMatch(const string &input,
      std::vector<string>* out) const {
  std::vector<string16> tmp;
  LikelyMatch(UTF8ToUTF16(input), &tmp);
  out->clear();
  for (size_t i = 0; i < tmp.size(); ++i) {
    out->push_back(UTF16ToUTF8(tmp[i]));
  }
}


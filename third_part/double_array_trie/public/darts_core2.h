// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)
// Description:
//      Improve build speed and reduce memory usage for string16

#ifndef UTIL_DOUBLE_ARRAY_TRIE_DARTS_CORE2_H_
#define UTIL_DOUBLE_ARRAY_TRIE_DARTS_CORE2_H_

#include <vector>
#include <cstring>
#include <string>
#include <cstdio>
#include <algorithm>

#include "base/logging.h"
#include "base/basictypes.h"
#include "base/string16.h"

namespace darts {

// Naming convention:
//   lower case for char level function,
//   upper case for string level
template <typename StringT>
class DoubleArrayCore2 {
 public:
  typedef int  ValueT;
  typedef int  BaseT;
  typedef uint CheckT;
  typedef uint SizeT;
  typedef typename StringT::value_type CharT;

  DoubleArrayCore2()
      : array_(0),
      array_size_(0),
      error_(0) {
  }

  struct ResultPair {
    ResultPair() : value(0), length(0) {}
    ValueT value;
    SizeT length;
  };

  template <typename ResultT>
  void exactMatch(const CharT* key,
                  SizeT key_len,
                  SizeT node_pos,
                  ResultT* result) const;

  template <typename ResultT>
  int prefixMatch(const CharT* key,
                  SizeT key_len,
                  SizeT node_pos,
                  ResultT* result,
                  SizeT result_len) const;

  bool matchAny(const CharT* key, SizeT len, SizeT node_pos = 0) const {
    ResultPair result;
    return prefixMatch(key, len, 0, &result, 1) > 0;
  }

  template <typename ResultT>
  void maxPrefixMatch(const CharT* key,
                      SizeT key_len,
                      SizeT node_pos,
                      ResultT* result) const;

  // Likely means edit distance is 1.
  void LikelyMatch(const StringT input,
                   std::vector<StringT>* out);

  ValueT traverse(const CharT* key,
                  SizeT key_len,
                  SizeT &node_pos,
                  SizeT &key_pos) const {
    if (!array_) return -2;

    BaseT b = array_[node_pos].base;
    CheckT p;
    for (; key_pos < key_len;) {
      if (!MoveOneStepMatch(key[key_pos], b, p)) {
        return -2;  // no node
      }
      ++key_pos;
      node_pos = p;
    }

    ValueT v = 0;
    if (GetValue(b, v)) {
      return v;
    }
    return -1;  // found, but no value
  }
 public:
  // Old intreface
  template <class ResultT>
  inline void exactMatchSearch(const CharT* key,
                               ResultT& result,
                               SizeT len,
                               SizeT node_pos = 0) const {
    result = exactMatch<ResultT> (key, len, node_pos, &result);
  }

  template <class T>
  size_t commonPrefixSearch(const CharT* key,
                            T* result,
                            size_t result_len,
                            size_t key_len,
                            size_t node_pos = 0) const {
    return prefixMatch(key, key_len, node_pos, result);
  }
  inline int exactMatchDbg(const CharT *key,
                           size_t len) const;

  template <class T>
  size_t commonPrefixSearchDbg(const CharT* key,
                            T* result,
                            size_t result_len,
                            size_t len = 0,
                            size_t node_pos = 0) const;


  bool Empty() const {
    return units_.empty();
  }

  void set_array(void *ptr, size_t raw_size);
  void set_array_with_ownership(void *ptr, size_t raw_size);

  const void *array() const {
    return const_cast<const void *>(reinterpret_cast<void *>(array_));
  }
  void clear() {}

  size_t unit_size()  const { return sizeof(Unit); }
  size_t size()       const { return array_size_; }
  size_t total_size() const { return array_size_ * sizeof(Unit); }

  size_t nonzero_size() const {
    size_t result = 0;
    for (size_t i = 0; i < array_size_; ++i)
      if (array_[i].check) ++result;
    return result;
  }

  size_t padding_size() const {
    return (1 << 8 * sizeof(CharT)) + 2;
  }
  size_t resize_step() const {
    return std::max(3 * padding_size(), key_size_);
  }

  int build(size_t       key_size,
            const CharT* *key,
            const size_t *length = 0,
            const ValueT *value = 0);

 protected:
  struct Node {
    Node() :
      code(0), depth(0), left(0), right(0), begin(0), end(0),
      parent(NULL), index(0), base(0), split(0) {}

    BaseT      code;  // == [actual char] + 1   !!!
    int        depth;
    SizeT      left;
    SizeT      right;
    int        begin;  // index in siblings
    int        end;  // index in siblings
    Node*      parent;
    int        index;  // index in array
    int        base;
    int8       split;
  };

  struct Unit {
    Unit() : base(0), check(0) {}
    BaseT   base;
    CheckT  check;
  };

  struct NodeRangeComparer {
    bool operator() (Node *a, Node *b) {
      return (a->right - a->left > b->right - b->left);
    }
  };

  inline uint32 UnsignedCode(CharT c) const;

  void SetResult(ValueT *x, ValueT r, size_t) const {
    *x = r;
  }
  void SetResult(ResultPair *x, ValueT r, size_t l) const {
    x->value = r;
    x->length = l;
  }

  void resize(const size_t new_size);

  size_t key_len(size_t i) {
    return length_[i];
  }

  void FetchAll(std::vector<Node> *nodes);

  void FetchSiblings(Node* parent, std::vector<Node> *siblings) {
    FetchSiblings(parent, siblings, 1);
  }

  void FetchSiblings(Node* parent, std::vector<Node> *siblings, int method);

  size_t InsertSiblings(Node* start, Node* end);

  bool MoveOneStepMatch(const CharT c, BaseT& b) const {  // NOLINT
    register CheckT pos;
    return MoveOneStepMatch(c, b, pos);
  }

  bool GetValue(BaseT b, ValueT& v) const {  // NOLINT
    if (sizeof(CharT) == 2 && b > kMaxBase) {
      b -= kMaxBase;
    }
    if (static_cast<CheckT>(b) == array_[b].check) {
      if (array_[b].base < 0) {
        v = -array_[b].base -1;
        return true;
      }
    }
    return false;
  }

  bool MoveOneStepMatch(const CharT c, BaseT& b, CheckT &p) const {  // NOLINT
    if (b < 0) return false;
    // Speical case first: if it is a splited base
    if (sizeof(CharT) == 2 && b > kMaxBase) {
      b -= kMaxBase;
    // It's about 15% slow to test splited_
    //  if (sizeof(CharT) == 2 && splited_[b / 64] && (1 << b % 64)) {
      p = b + (UnsignedCode(c) >> 8) + 1;
      if (static_cast<CheckT>(b) != array_[p].check) {
        return false;
      }
      b = array_[p].base;
      p = b + (UnsignedCode(c) & 0xFF) + 1;
      if (static_cast<CheckT>(b) != array_[p].check) {
        return false;
      }
      b = array_[p].base;
      return true;
    }
    // General case
    p = b + UnsignedCode(c) + 1;
    if (static_cast<CheckT>(b) != array_[p].check) {
      return false;
    }
    b = array_[p].base;
    return true;
  }

  int ListNextStepChars(SizeT pos, CharT* buf, SizeT* poses) {
    BaseT b = array_[pos].base;
    if (b < 0) return 0;
    CheckT p = b + 1;
    int n = 0;
    // Speical case first: if it is a splited base
    if (sizeof(CharT) == 2 && b > kMaxBase) {
      b -= kMaxBase;
      p = b + 1;
      for (int h = 0; h < 256; h++) {
        if (static_cast<CheckT>(b) == array_[p + h].check) {
          BaseT b2 = array_[p + h].base;
          CheckT p2 = b2 + 1;
          for (int l = 0; l < 256; l++) {
            if (static_cast<CheckT>(b2) == array_[p2 + l].check) {
              buf[n] = (h << 8) + l;
              poses[n++] = p2 + l;
            }
          }
        }
      }
      return n;
    }
    for (int i = 0; i < kCharRange; ++i) {
      if (static_cast<CheckT>(b) == array_[p + i].check) {
        buf[n] = i;
        poses[n++] = p + i;
      }
    }
    return n;
  }

  static const int kMaxBase = 0x40000000;
  static const int kCharRange = 1 << (sizeof(CharT) * 8);
  // Inputs
  size_t        key_size_;
  const CharT*  *key_;
  const size_t  *length_;
  const ValueT  *value_;
  // Internal build data
  Unit         *array_;
  size_t        array_size_;
  size_t        next_check_pos_;
  int           error_;
  //  use vector to handle resizing
  std::vector<Unit> units_;
  std::vector<bool>   used_;  // flag indices which BASE has been used.
  std::vector<uint64> usedc_;  // compacted used.
  std::vector<uint64> splited_;  // compacted used.
  // For LikelyMatch cache
  std::vector<CharT> first_chars_;
  std::vector<SizeT> first_poses_;
  // For debug
  size_t        progress_;
  size_t        try_count_;
  size_t        max_checked_;
  size_t        max_base_;
  size_t        skipped_space_;
};

template<typename StringT>
inline uint32 DoubleArrayCore2<StringT>::UnsignedCode(CharT c) const {
  return c;
}
template<>
inline uint32 DoubleArrayCore2<std::string>::UnsignedCode(
    char c) const {
  return static_cast<unsigned char>(c);
}
template<>
inline uint32 DoubleArrayCore2<string16>::UnsignedCode(
    char16 c) const {
  return static_cast<unsigned char16>(c);
}

template<typename StringT> template<typename ResultT>
inline void DoubleArrayCore2<StringT>::exactMatch(
    const CharT* key,
    SizeT len,
    SizeT node_pos,
    ResultT* result) const {
  SetResult(result, -1, 0);
  if (!array_) return;

  register BaseT b = array_[node_pos].base;
  ValueT v = 0;
  for (register size_t i = 0; i < len;) {
    if (!MoveOneStepMatch(key[i], b)) {
      return;
    }
    ++i;
    if (b < 0) {
      if (i == len) {  // Hit at le last
        SetResult(result, -b-1, i);
      }
      return;
    }
  }
  if (GetValue(b, v)) {
    SetResult(result, v, len);
  }
  return;
}

template<typename StringT>
int DoubleArrayCore2<StringT>::exactMatchDbg(
    const CharT* key, size_t len) const {
  if (!array_) return -2;

  register BaseT b = array_[0].base;
  for (register size_t i = 0; i < len;) {
    if (!MoveOneStepMatch(key[i], b)) {
      return -1;
    }
    ++i;
    if (b < 0) {
      return (i == len) ? -b-1 : -1;
    }
  }

  ValueT v = 0;
  if (GetValue(b, v)) {
    return v;
  }
  return -1;
}

template<typename StringT> template<typename ResultT>
int DoubleArrayCore2<StringT>::prefixMatch(
    const CharT* key,
    SizeT key_len,
    SizeT node_pos,
    ResultT* result,
    SizeT result_len) const {
  if (!array_) return 0;

  register BaseT b = array_[node_pos].base;
  register int num = 0;
  ValueT v = 0;
  for (register size_t i = 0; i < key_len;) {
    if (!MoveOneStepMatch(key[i], b)) {
      return num;
    }
    ++i;
    if (b < 0) {
      // Hit at le last
      SetResult(&result[num], -b-1, i);
      return ++num;
    }

    if (GetValue(b, v) && num < result_len) {
      SetResult(&result[num], v, i);
      ++num;
    }
  }
  return num;
}

template<typename StringT> template<typename ResultT>
void DoubleArrayCore2<StringT>::maxPrefixMatch(
    const CharT* key,
    SizeT key_len,
    SizeT node_pos,
    ResultT* result) const {
  if (!array_) return;

  register BaseT  b = array_[node_pos].base;
  register ValueT v = 0;

  SetResult(result, -1, 0);
  for (register size_t i = 0; i < key_len;) {
    if (!MoveOneStepMatch(key[i], b)) {
      return;
    }
    ++i;
    if (b < 0) {
      SetResult(result, -b-1, i);
      return;
    }
    if (GetValue(b, v)) {
      SetResult(result, v, i);
    }
  }
}

}  // namespace darts
#endif  // UTIL_DOUBLE_ARRAY_TRIE_DARTS_CORE2_H_

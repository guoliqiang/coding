/*
  Darts -- Double-ARray Trie System

  $Id: darts.h.in 1674 2008-03-22 11:21:34Z taku $;

  Copyright(C) 2001-2007 Taku Kudo <taku@chasen.org>
  Make a copy here to modify specially for segmenter dict.
*/
#ifndef DOUBLE_ARRAY_TRIE_DARTS_H_
#define DOUBLE_ARRAY_TRIE_DARTS_H_

#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>

#include "../../../base/public/logging.h"

namespace darts {

template <class T>
inline T _max(T x, T y){
  return(x > y) ? x : y;
}

template <class T>
inline T* _resize(T* ptr, size_t n, size_t l, T v) {
  T *tmp = new T[l];
  for (size_t i = 0; i < n; ++i) tmp[i] = ptr[i];
  for (size_t i = n; i < l; ++i) tmp[i] = v;
  delete [] ptr;
  return tmp;
}

template <class T>
class Length {
 public:
  size_t operator()(const T *key) const {
    size_t i;
    for (i = 0; key[i] != static_cast<T>(0); ++i) {}
    return i;
  }
};

template <>
class Length<char> {
 public:
  size_t operator()(const char *key) const {
    return std::strlen(key);
  }
};

template <typename node_type_,  typename node_u_type_,
          typename array_type_, typename array_u_type_,
          typename length_func_ = Length<node_type_> >
class DoubleArrayImpl {
 public:
  typedef array_type_  value_type;
  typedef node_type_   key_type;
  typedef array_type_  result_type;  // for compatibility

  struct result_pair_type {
    result_pair_type() : value(0), length(0) {}
    value_type value;
    size_t     length;
  };

  explicit DoubleArrayImpl(): array_(0), used_(0),
                              size_(0), alloc_size_(0),
                              no_delete_(0), error_(0) {}
  virtual ~DoubleArrayImpl() {
    clear();
  }

  void set_result(value_type *x, value_type r, size_t) const {
    *x = r;
  }

  void set_result(result_pair_type *x, value_type r, size_t l) const {
    x->value = r;
    x->length = l;
  }

  void set_array(void *ptr, size_t raw_size) {
    clear();
    array_ = reinterpret_cast<unit_t *>(ptr);
    no_delete_ = true;
    size_ = raw_size / sizeof(unit_t);
  }

  void set_array_with_ownership(void *ptr, size_t raw_size) {
    clear();
    array_ = reinterpret_cast<unit_t *>(ptr);
    size_ = raw_size / sizeof(unit_t);
  }

  const void *array() const {
    return const_cast<const void *>(reinterpret_cast<void *>(array_));
  }

  void clear() {
    if (!no_delete_)
      delete [] array_;
    delete [] used_;
    array_ = 0;
    used_ = 0;
    alloc_size_ = 0;
    size_ = 0;
    no_delete_ = false;
  }

  size_t unit_size()  const {
    return sizeof(unit_t);
  }

  size_t size() const {
    return size_;
  }

  size_t total_size() const {
    return size_ * sizeof(unit_t);
  }

  size_t nonzero_size() const {
    size_t result = 0;
    for (size_t i = 0; i < size_; ++i)
      if (array_[i].check) ++result;
    return result;
  }

  size_t padding_size() const {
    return (1 << 8 * sizeof(key_type)) + 1;
  }

  size_t resize_step() const {
    return _max(3 * padding_size(), key_size_);
  }

  int build(size_t           key_size,
            const key_type   **key,
            const size_t     *length = 0,
            const value_type *value = 0,
            int (*progress_func)(size_t, size_t) = 0) {
    if (!key_size || !key) {
      resize(padding_size());
      return 0;
    }

    progress_func_ = progress_func;
    key_           = key;
    length_        = length;
    key_size_      = key_size;
    value_         = value;
    progress_      = 0;
    try_count_     = 0;
    max_checked_   = 0;
    max_begin_     = 0;
    skipped_space_ = 0;

    // 平均词长近似值
    const int avg_keyword_len = sizeof(key_type) == 1 ? 15 : 7;
    size_t approximate_size = key_size * avg_keyword_len;
    // 先分配足够大的内存 [james]
    resize(_max(_max(size_t(8192), padding_size()), approximate_size));

    array_[0].base = 1;
    next_check_pos_ = 0;

    node_t root_node;
    root_node.left  = 0;
    root_node.right = key_size;
    root_node.depth = 0;

    std::vector<node_t> siblings;
    fetch(root_node, siblings);
    insert(siblings);

    size_ += padding_size();
    if (size_ >= alloc_size_) resize(size_);

    delete [] used_;
    used_ = 0;

    size_t checked = 0;
    for (size_t i = 0; i < size_; ++i) {
      if (array_[i].check) {
        checked++;
      }
    }

    VLOG(0) << "Usage: " << 100.0 * checked / size_ << "%";
    VLOG(1) << "Next max: " << next_check_pos_ << ", Skip:" <<
            skipped_space_ << ", size: " << size_;

    VLOG(5) << "Try count: " << try_count_;
    return error_;
  }

  template <class T>
  inline void exactMatchSearch(const key_type *key,
                               T & result,
                               size_t len = 0,
                               size_t node_pos = 0) const {
    result = exactMatchSearch<T> (key, len, node_pos);
    return;
  }

  template <class T>
  inline T exactMatchSearch(const key_type *key,
                            size_t len = 0,
                            size_t node_pos = 0) const {
    if (!len) len = length_func_()(key);

    T result;
    set_result(&result, -1, 0);
    if (!array_) return result;

    register array_type_  b = array_[node_pos].base;
    register array_u_type_ p;
//  printf("b:　%d", b);

    for (register size_t i = 0; i < len; ++i) {
      p = b +(node_u_type_)(key[i]) + 1;
      if (static_cast<array_u_type_>(b) == array_[p].check)
        b = array_[p].base;
      else
        return result;
    }

    p = b;
    array_type_ n = array_[p].base;
    if (static_cast<array_u_type_>(b) == array_[p].check && n < 0)
      set_result(&result, -n-1, len);
    // "-n-1" is the index of key_ (inputs of patterns)
    // len is the input of this function

    return result;
  }

  template <class T>
  size_t commonPrefixSearch(const key_type *key,
                            T* result,
                            size_t result_len,
                            size_t len = 0) const {
    if (!len) len = length_func_()(key);
    if (!array_) return 0;

    register array_type_  b   = array_[0].base;
    register size_t     num = 0;
    register array_type_  n;
    register array_u_type_ p;

    for (register size_t i = 0; i < len; ++i) {
      p = b;  // + 0;
      n = array_[p].base;
      if ((array_u_type_) b == array_[p].check && n < 0 && i > 0) {
        // result[num] = -n-1;
        if (num < result_len) set_result(&result[num], -n-1, i);
        ++num;
      }

      p = b +(node_u_type_)(key[i]) + 1;
      if ((array_u_type_) b == array_[p].check)
        b = array_[p].base;
      else
        return num;
    }

    p = b;
    n = array_[p].base;

    if ((array_u_type_)b == array_[p].check && n < 0) {
      if (num < result_len) set_result(&result[num], -n-1, len);
      ++num;
    }

    return num;
  }

  bool MatchAny(const key_type *key, size_t len) const {
    if (!array_) return false;

    register array_type_  b   = array_[0].base;
    register array_type_  n;
    register array_u_type_ p;
    if (!array_) return false;

    for (register size_t i = 0; i < len; ++i) {
      p = b;  // + 0;
      n = array_[p].base;
      if ((array_u_type_) b == array_[p].check && n < 0) {
        return true;
      }

      p = b +(node_u_type_)(key[i]) + 1;
      if ((array_u_type_) b == array_[p].check)
        b = array_[p].base;
      else
        return false;
    }

    p = b;
    n = array_[p].base;

    if ((array_u_type_)b == array_[p].check && n < 0) {
      return true;
    }

    return false;
  }

  // Add by [james]
  // Check result if something match
  template <class T>
  void maxPrefixSearch(const key_type *key,
                       T* result,
                       size_t len = 0) const {
    if (!len) len = length_func_()(key);
    if (!array_) return;

    register array_type_  b   = array_[0].base;
    register array_type_  n;
    register array_u_type_ p;

    for (register size_t i = 0; i < len; ++i) {
      p = b;  // + 0;
      n = array_[p].base;
      if ((array_u_type_) b == array_[p].check && n < 0) {
        set_result(result, -n-1, i);
      }

      p = b +(node_u_type_)(key[i]) + 1;
      if ((array_u_type_) b == array_[p].check)
        b = array_[p].base;
      else
        return;
    }

    p = b;
    n = array_[p].base;
    if ((array_u_type_)b == array_[p].check && n < 0) {
      set_result(result, -n-1, len);
    }
  }

  value_type traverse(const key_type *key,
                      size_t &node_pos,
                      size_t &key_pos,
                      size_t len = 0) const {
    if (!len) len = length_func_()(key);
    if (!array_) return -2;

    register array_type_  b = array_[node_pos].base;
    register array_u_type_ p;

    for (; key_pos < len; ++key_pos) {
      p = b + (node_u_type_)(key[key_pos]) + 1;
      if (static_cast<array_u_type_>(b) == array_[p].check) {
        node_pos = p;
        b = array_[p].base;
      } else {
        return -2;  // no node
      }
    }

    p = b;
    array_type_ n = array_[p].base;
    if (static_cast<array_u_type_>(b) == array_[p].check && n < 0)
      return -n-1;

    return -1;  // found, but no value
  }

 private:
  struct node_t {
    array_u_type_ code;
    size_t     depth;
    size_t     left;
    size_t     right;
  };

  struct unit_t {
    array_type_   base;
    array_u_type_ check;
  };

  unit_t              *array_;
  unsigned char       *used_;
  size_t              size_;
  size_t              alloc_size_;
  size_t              key_size_;
  const node_type_    **key_;
  const size_t        *length_;
  const array_type_   *value_;
  size_t              progress_;
  size_t              next_check_pos_;
  bool                no_delete_;
  int                 error_;
  size_t              try_count_;
  size_t              max_checked_;
  size_t              max_begin_;
  size_t              skipped_space_;

  int (*progress_func_)(size_t, size_t);

  size_t resize(const size_t new_size) {
    VLOG(5) << "DA Trie resize: " <<  new_size;
    unit_t tmp;
    tmp.base = 0;
    tmp.check = 0;
    array_ = _resize(array_, alloc_size_, new_size, tmp);
    used_  = _resize(used_, alloc_size_, new_size,
                     static_cast<unsigned char>(0));
    alloc_size_ = new_size;
    VLOG(5) << "resize completed";
    return new_size;
  }

  size_t key_len(size_t i) {
    return length_ ? length_[i] : length_func_()(key_[i]);
  }

  void fetch(const node_t &parent, std::vector <node_t> &siblings) {
    if (error_ < 0) return;

    array_u_type_ prev = 0;

    for (size_t i = parent.left; i < parent.right; ++i) {
      if (key_len(i) < parent.depth) {
        CHECK(false);
        continue;
      }

      const node_u_type_ *tmp =
          reinterpret_cast<const node_u_type_ *>(key_[i]);

      array_u_type_ cur = 0;
      if (key_len(i) != parent.depth)
        cur = (array_u_type_)tmp[parent.depth] + 1;

      if (prev > cur) {
        error_ = -3;
        return;
      }

      if (cur != prev || siblings.empty()) {
        node_t tmp_node;
        tmp_node.depth = parent.depth + 1;
        tmp_node.code  = cur;
        tmp_node.left  = i;
        if (!siblings.empty()) siblings[siblings.size()-1].right = i;
        siblings.push_back(tmp_node);
      }

      prev = cur;
    }
    CHECK(siblings.size());

    siblings[siblings.size()-1].right = parent.right;
  }

  struct note_t_comparer {
    bool operator() (const node_t &i, const node_t &j) {
      return (i.right - i.left > j.right - j.left);
    }
  };

  size_t insert(std::vector <node_t> &siblings) {
    if (error_ < 0) return 0;

    size_t begin = 0;
    size_t pos   = _max((size_t)siblings[0].code + 1, next_check_pos_);
    size_t nonzero_num = 0, used_num = 0;

    if (alloc_size_ <= pos + 3 * padding_size()) {
      resize(pos + resize_step());
    }

    while (array_[pos].check) pos++;
    next_check_pos_ = pos;

    for (int c = 0; ; c++, pos++) {
      if (alloc_size_ <= pos + 3 * padding_size()) {
        resize(pos + resize_step());
      }
      if (array_[pos].check) {
        nonzero_num++;
        continue;
      }
      begin = pos - siblings[0].code;
      if (used_[begin]) {
        used_num++;
        continue;
      }
      bool found_space = true;
      for (size_t i = 1; i < siblings.size(); ++i) {
        if (array_[begin + siblings[i].code].check != 0) {
          try_count_ += i;
          found_space = false;
          break;
        }
      }
      // -- Simple heuristics --
      // if the percentage of non-empty contents in check between the index
      // 'next_check_pos' and 'check' is greater than some constant
      // value(e.g. 0.9),
      // new 'next_check_pos' index is written by 'check'.
      if (1.0 * nonzero_num/(pos - next_check_pos_ + 1) >=
          (sizeof(key_type) == 1 ? 0.95 : 0.2)) {
        CHECK_LE(next_check_pos_, pos);
        CHECK_LE(nonzero_num, pos - next_check_pos_);
        nonzero_num = 0;
        skipped_space_ += pos - next_check_pos_ - nonzero_num;
        next_check_pos_ = pos;
      }

      if (found_space) break;
    }


    max_begin_ = _max(max_begin_, begin);
    used_[begin] = 1;
    size_ = _max(size_, begin + siblings[siblings.size() - 1].code + 1);

    for (size_t i = 0; i < siblings.size(); ++i) {
      array_[begin + siblings[i].code].check = begin;
      max_checked_ = _max(max_checked_, begin + siblings[i].code);
    }

    // std::sort(siblings.begin(), siblings.end(), note_t_comparer());

    std::vector <node_t> new_siblings;
    for (size_t i = 0; i < siblings.size(); ++i) {
      new_siblings.clear();
      if (siblings[i].right == siblings[i].left + 1 &&
          key_len(siblings[i].left) < siblings[i].depth) {
        array_[begin + siblings[i].code].base =
          value_ ?
          static_cast<array_type_>(-value_[siblings[i].left]-1) :
          static_cast<array_type_>(-siblings[i].left-1);

        if (value_ && (array_type_)(-value_[siblings[i].left]-1) >= 0) {
          error_ = -2;
          return 0;
        }

        ++progress_;
        if (progress_func_)(*progress_func_)(progress_, key_size_);

      } else {
        fetch(siblings[i], new_siblings);
        size_t h = insert(new_siblings);
        array_[begin + siblings[i].code].base = h;
      }
    }
    return begin;
  }
};
}  // namespace darts
#endif  // DOUBLE_ARRAY_TRIE_DARTS_H_

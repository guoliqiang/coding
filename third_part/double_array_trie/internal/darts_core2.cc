// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)
// Description:

#include "util/double_array_trie/darts_core2.h"

#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <string>

#include "base/logging.h"
#include "base/yr.h"
#include "base/string_util.h"
#include "file/file.h"
#include "util/progress_indicator/progress_indicator.h"

DEFINE_bool(darts2_sort_before_build, false, "");
DEFINE_bool(darts2_smart_split_big, false, "");
DEFINE_int32(darts2_intense_level, 2, "");

namespace darts {
using file::File;

static const int kSplitThres = 20;
static const int kSplitSpan = 512;

static const int8 kSplitHigh = 2;
static const int8 kSplitLow = 3;
static const int8 kSplitParent = 4;

template <typename StringT>
int DoubleArrayCore2<StringT>::build(
    size_t       key_size,
    const CharT* *key,
    const size_t *length,
    const ValueT *value) {
  if (!key_size || !key) {
    resize(padding_size());
    return 0;
  }

  key_           = key;
  length_        = length;
  key_size_      = key_size;
  value_         = value;
  CHECK(value_);
  for (size_t i = 0; i < key_size; ++i) {
    if (value_[i] < 0) {
      LOG(ERROR) << "Negtive value for trie!!";
      error_ = -2;
      return 0;
    }
  }

  // Build variables and debug counters
  error_         = 0;
  try_count_     = 0;
  max_checked_   = 0;
  max_base_     = 0;
  skipped_space_ = 0;
  next_check_pos_ = 0;

  Node root_node;
  root_node.code = 0;
  root_node.left  = 0;
  root_node.right = key_size;
  root_node.depth = 0;
  root_node.parent = NULL;
  root_node.index = 0;

  std::vector<Node> nodes;
  nodes.push_back(root_node);
  FetchAll(&nodes);
  VLOG(4) << "Fetch all trie nodes complete.";
  std::vector<Node*> seq_nodes;
  seq_nodes.reserve(nodes.size());
  for (size_t i = 0; i < nodes.size(); ++i) {
    seq_nodes.push_back(&nodes[i]);
  }
  // Must be STABLE to reserve orinal order!
  if (FLAGS_darts2_sort_before_build) {
    std::stable_sort(seq_nodes.begin(), seq_nodes.end(), NodeRangeComparer());
    VLOG(3) << "Sort trie nodes complete.";
  }

  // 先分配足够大的内存
  units_.clear();
  used_.clear();
  usedc_.clear();
  resize(nodes.size() * 2 + padding_size());
  used_[0] = true;  // To keep code more safe, base 0 should NOT be used
  usedc_[0] = 1;

  util::ProgressIndicator<size_t> indicator(seq_nodes.size());
  indicator.SetDescription("Build Trie");
  for (size_t i = 0; i < seq_nodes.size(); ++i) {
    if (seq_nodes[i]->right > seq_nodes[i]->left + 1 ||
        key_len(seq_nodes[i]->left) > seq_nodes[i]->depth) {
      BaseT b =
        InsertSiblings(&nodes[seq_nodes[i]->begin], &nodes[seq_nodes[i]->end]);
      array_[seq_nodes[i]->index].base = b;
      if (seq_nodes[i]->split == kSplitParent) {
        array_[seq_nodes[i]->index].base += kMaxBase;
        // Use splited flag
        // splited_[b / 64] && (1 << b % 64)
      }
    }
    if ((i % 1000 == 0) && FLAGS_v >= 3) {
      indicator.SetValue(i);
    }
  }

  resize(max_base_ + padding_size());  // Set the safe size

  // Print statistic info
  size_t checked = 0;
  for (size_t i = 0; i < array_size_; ++i) {
    if (array_[i].check) {
      checked++;
    }
  }
  VLOG(0) << "Usage: " << 100.0 * checked / array_size_ << "% " << nodes.size();
  VLOG(1) << "Next max: " << next_check_pos_ << ", Skip:" <<
             skipped_space_ << ", size: " << array_size_;
  VLOG(5) << "Try count: " << try_count_;
  nodes.clear();
  // used_.clear(); NOT clear for dynamic operation
  return error_;
}

template <typename StringT>
void DoubleArrayCore2<StringT>::resize(const size_t new_size) {
  VLOG(5) << "DA Trie resize: " <<  new_size;
  units_.resize(new_size);
  used_.resize(new_size);
  usedc_.resize(new_size);
  splited_.resize(new_size / 64);
  array_ = units_.data();
  array_size_ = units_.size();
}

template <typename StringT>
void DoubleArrayCore2<StringT>::FetchAll(
    std::vector<Node> *nodes) {
  size_t b = 0, e = nodes->size();
  std::vector <Node> subs;
  while (true) {
    for (size_t i = b; i < e; ++i) {
      if ((*nodes)[i].right > (*nodes)[i].left + 1 ||
          key_len((*nodes)[i].left) > (*nodes)[i].depth) {
        subs.clear();
        if ((*nodes)[i].split == kSplitHigh) {
          FetchSiblings(&(*nodes)[i], &subs, kSplitLow);
//        } else if ((*nodes)[i].split == kSplitLow) {
//          FetchSiblings(&(*nodes)[i], &subs);
        } else {
          FetchSiblings(&(*nodes)[i], &subs);
          if (sizeof(CharT) == 2 &&
              FLAGS_darts2_smart_split_big &&
              subs.size() > kSplitThres &&
              subs[subs.size() - 1].code > subs[1].code + kSplitSpan) {
            (*nodes)[i].split = kSplitParent;
            subs.clear();
            FetchSiblings(&(*nodes)[i], &subs, kSplitHigh);
          }
        }
        for (size_t j = 0; j < subs.size(); ++j) {
          subs[j].parent = &(*nodes)[i];
          subs[j].begin = subs[j].end = 0;
        }
        (*nodes)[i].begin = nodes->size();
        nodes->insert(nodes->end(), subs.begin(), subs.end());
        (*nodes)[i].end = nodes->size();
      }
    }
    if (e == nodes->size()) break;  // No more sub siblings
    b = e;
    e = nodes->size();
  }
}

template <typename StringT>
void DoubleArrayCore2<StringT>::FetchSiblings(
    Node* parent, std::vector<Node> *siblings, int method) {
  if (error_ < 0) return;

  BaseT prev = 0;

  for (size_t i = parent->left; i < parent->right; ++i) {
    // It allow to be equal in the case parent has a value.
    CHECK_GE(key_len(i), parent->depth);

    const CharT* str = key_[i];

    BaseT cur = 0;
    if (key_len(i) == parent->depth) {
      // It's a value node
      cur = 0;
    } else if (key_len(i) > parent->depth) {
      cur = UnsignedCode(str[parent->depth]) + 1;
      if (method == kSplitHigh) {
        cur = (UnsignedCode(str[parent->depth]) >> 8) + 1;
      } else if (method == kSplitLow) {
        cur = (UnsignedCode(str[parent->depth]) & 0xFF) + 1;
      }
    }

    if (prev > cur) {
      // If the input CharT is sorted by signed, this will happened!
      // string should be sorted by memcmp
      LOG(FATAL) << "Input key NOT sorted!!" << "\t " << prev << ", " << cur;
      error_ = -3;
      return;
    }

    if (cur != prev || siblings->empty()) {
      Node tmp_node;
      if (method == kSplitHigh) {
        tmp_node.depth = parent->depth;
      } else {
        tmp_node.depth = parent->depth + 1;
      }
      tmp_node.code  = cur;
      tmp_node.left  = i;
      if (method == kSplitHigh || method == kSplitLow) {
        tmp_node.split = method;
      }
      if (!siblings->empty()) (*siblings)[siblings->size()-1].right = i;

      siblings->push_back(tmp_node);
    }

    prev = cur;
  }

  CHECK(siblings->size());

  (*siblings)[siblings->size()-1].right = parent->right;
}

template <typename StringT>
size_t DoubleArrayCore2<StringT>::InsertSiblings(Node* start, Node* end) {
  if (error_ < 0) return 0;

  size_t base = 0;
  size_t pos   = std::max((size_t)start->code + 1, next_check_pos_);
  size_t nonzero_num = 0, used_num = 0;

  if (units_.size() <= max_checked_ + 3 * padding_size()) {
    resize(2 * units_.size());
  }

  while (array_[pos].check) pos++;
  next_check_pos_ = pos;

  for (int c = 0; ; c++, pos++) {
    if (array_[pos].check) {
      nonzero_num++;
      continue;
    }
    base = pos - start->code;  // found a slot for the first
    if (used_[base]) {
      used_num++;
      continue;
    }
    bool found_space = true;
    for (Node* p = start + 1; p < end; ++p) {
      if (array_[base + p->code].check != 0) {
        try_count_++;  // add i?
        found_space = false;
        break;
      }
    }
    if (found_space) break;
    // -- Simple heuristics --
    // if the percentage of non-empty contents in check between the index
    // 'next_check_pos' and 'check' is greater than some constant
    // value(e.g. 0.9),
    if (1.0 * nonzero_num/(pos - next_check_pos_ + 1) >=
        (sizeof(CharT) == 1 ? 0.95 : (FLAGS_darts2_intense_level / 10.0))) {
      skipped_space_ += pos - next_check_pos_ - nonzero_num;
      next_check_pos_ = pos;
      nonzero_num = 0;
    }
  }

  for (Node* p = start; p < end; ++p) {
    array_[base + p->code].check = base;
    p->index = base + p->code;
  }
  for (Node* p = start; p < end; ++p) {
    // If it is the leaf node, or fake leaf node for parent has value
    // put the value into base
    if (p->right == p->left + 1 && key_len(p->left) <= p->depth) {
      ValueT v =  value_ ?
          static_cast<ValueT>(-value_[p->left]-1) :
          static_cast<ValueT>(-p->left-1);
      array_[base + p->code].base = v;
    }
  }
  used_[base] = 1;

  max_base_ = std::max(max_base_, base);
  max_checked_ = std::max(max_checked_, base + (end - 1)->code);

  return base;
}

template <typename StringT>
void DoubleArrayCore2<StringT>::set_array(void *ptr, size_t raw_size) {
  array_ = reinterpret_cast<Unit *>(ptr);
  array_size_ = raw_size / sizeof(Unit);
}

template <typename StringT>
void DoubleArrayCore2<StringT>::set_array_with_ownership(
    void *ptr, size_t raw_size) {
  array_ = reinterpret_cast<Unit *>(ptr);
  array_size_ = raw_size / sizeof(Unit);
  units_.clear();
  units_.insert(units_.end(), array_, array_ + array_size_);
  delete array_;
  array_ = &units_[0];
}

template<typename StringT> template<class T>
size_t DoubleArrayCore2<StringT>::commonPrefixSearchDbg(
    const CharT *key,
    T* result,
    size_t result_len,
    size_t len,
    size_t node_pos) const {
  if (!array_) return 0;

  register BaseT b = array_[node_pos].base;
  register size_t num = 0;
  ValueT v = 0;
  for (register size_t i = 0; i < len;) {
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

// Travel i, edit one char, and match the tail
// Not const because lazy init first_chars_
template<typename StringT>
void DoubleArrayCore2<StringT>::LikelyMatch(
    const StringT input, std::vector<StringT>* out) {
  out->clear();
  if (input.empty() || !array_) return;
  ResultPair pair;
  StringT s;
  // s.push_back(' ');
  s += input;
  s.push_back(' ');  // padding left and right for edit
  CharT next_chars[kCharRange];
  SizeT next_poses[kCharRange];

  // Special case: drop last char
  exactMatch(input.data(), input.size()- 1, 0, &pair);
  if (pair.length > 0) {
    StringT like(input.data(), input.size() - 1);
    out->push_back(like);
    VLOG(3) << "drop last char likely match";
  }

  for (size_t i = 0; i <= input.size(); ++i) {
    SizeT node_pos = 0, key_pos = 0;
    // 1. Prefix
    if (i > 0) {
      traverse(input.data(), i, node_pos, key_pos);
      if (i != key_pos) {
        return;
      }
    } else {
      // node_pos = 1;
    }
    // 2. Remove/skip one char
    if (input.size() - i > 1) {
      exactMatch(input.data() + i + 1, input.size() - i - 1, node_pos, &pair);
      if (pair.length > 0) {
        StringT like = input;
        like.erase(i, 1);
        out->push_back(like);
        VLOG(3) << "Remove char likely match: " << i;
      }
    }
    // 3. Switch two chars
    if (input.size() - i > 1) {
      std::swap(s[i], s[i + 1]);
      exactMatch(s.data() + i, input.size() - i, node_pos, &pair);
      if (pair.length > 0) {
        StringT like(s.data(), input.size());
        VLOG(3) << "Switch char likely match: " << i;
        out->push_back(like);
      }
      std::swap(s[i], s[i + 1]);
    }
    // 4. Insert/Edit one char
    // 4.1 get next candidiate chars
    int n = 0;
    CharT* chars = NULL;
    SizeT* poses = NULL;
    if (i == 0) {
      if (first_chars_.empty()) {
        first_chars_.resize(kCharRange);
        first_poses_.resize(kCharRange);
        chars = &first_chars_[0];
        poses = &first_poses_[0];
        int n = ListNextStepChars(0, chars, poses);
        first_chars_.resize(n);
        first_poses_.resize(n);
      }
      chars = &first_chars_[0];
      poses = &first_poses_[0];
      n = first_chars_.size();
    } else {
      n = ListNextStepChars(node_pos, next_chars, next_poses);
      chars = next_chars;
      poses = next_poses;
    }
    if (n == 0) {
      continue;
    }

    // 4.2 append one char
    if (i + 1 >= input.size()) {
      for (int j = 0; j < n; ++j) {
        exactMatch(chars + j, 1, node_pos, &pair);
        if (pair.length > 0) {
          StringT like = input;
          if (i == input.size()) {
            like.push_back(chars[j]);  // append
            VLOG(3) << "Append char likely match: ";
          } else {
            if (input[i] == chars[j]) continue;
            like[i] = chars[j];  // edit the last char
          }
          out->push_back(like);
        }
      }
    }

    // Try the chars
    if (i == input.size()) continue;
    for (int j = 0; j < n; ++j) {
      if (poses[j] < 0) continue;
      // Insert
      exactMatch(input.data() + i, input.size() - i, poses[j], &pair);
      if (pair.length > 0) {
        StringT like = input;
        like.insert(like.begin() + i, chars[j]);
        out->push_back(like);
      }
      // Edit
      if (i + 1 == input.size() || chars[j] == input[i]) continue;
      exactMatch(input.data() + i + 1, input.size() - i -1, poses[j], &pair);
      if (pair.length > 0) {
        StringT like = input;
        like[i] = chars[j];
        out->push_back(like);
      }
    }
  }  // end of main loop
  // fix bugs
  for (size_t i = 0; i < out->size(); ++i) {
    if (input == out->at(i)) {
      out->erase(out->begin() + i);
      i--;
    }
  }
}


// 模板在cc文件，需要显示实例化
template class DoubleArrayCore2<std::string>;
template class DoubleArrayCore2<string16>;
template size_t DoubleArrayCore2<string>::commonPrefixSearchDbg(
    const char*,
    ResultPair*,
    size_t,
    size_t,
    size_t) const;
template size_t DoubleArrayCore2<string16>::commonPrefixSearchDbg(
    const char16*,
    ResultPair*,
    size_t,
    size_t,
    size_t) const;

}  // namespace cjk2

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_PUBLIC_BUFF_INDEX_H_
#define UTIL_PARSE_MASTER2_PUBLIC_BUFF_INDEX_H_

#include <map>
#include <string>
#include <vector>

#include "base/public/hash_tables.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "html_tag.h"

namespace parse_master {
class BuffIndex {
 public:
  explicit BuffIndex(int offset = 0) : offset_(offset), length_(0) {}

  inline void Clear() {
    offset_ = 0;
    length_ = 0;
  }

  inline std::string* ToString(const char* buff, std::string* str) const {
    str->assign(buff + offset_, length_);
    return str;
  }

  inline void AppendToString(const char* buff, std::string* str) const {
    str->append(buff + offset_, length_);
  }

  inline int GetOffset() const {
    return offset_;
  }

  inline int GetLength() const {
    return length_;
  }

  inline void SetOffset(int offset) {
    offset_ = offset;
  }

  inline void SetEndPos(int end_pos) {
    length_ = end_pos - offset_;
  }
  inline void SetOffsetAndLength(int offset, int length) {
    offset_ = offset;
    length_ = length;
  }

  inline bool Empty() const {
    return offset_ == 0 && length_ == 0;
  }

 private:
  int offset_;
  int length_;
};

}

#endif  // UTIL_PARSE_MASTER2_PUBLIC_BUFF_INDEX_H_

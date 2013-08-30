// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_INTERNAL_HTML_TOKEN_H_
#define UTIL_PARSE_MASTER2_INTERNAL_HTML_TOKEN_H_

#include <map>
#include <string>
#include <vector>

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest_prod.h"
#include "buff_index.h"

namespace parse_master {
template<class T> class ObjPool;

enum HtmlTokenType {
  TOKEN_TYPE_UNKNOW = 0,
  TOKEN_TYPE_DOCTYPE = 1,
  TOKEN_TYPE_COMMENT = 2,
  TOKEN_TYPE_SCRIPT = 3,
  TOKEN_TYPE_STYLE = 4,
  TOKEN_TYPE_OPEN_TAG = 5,
  TOKEN_TYPE_SELF_CLOSE_TAG = 6,
  TOKEN_TYPE_TEXT_BLOCK = 7,
  TOKEN_TYPE_CLOSE_TAG = 8,
  TOKEN_TYPE_CDATA = 9,
  TOKEN_TYPE_MAX = 10,  // the size of HtmlTokenType
};

struct HtmlAttrIndex {
  BuffIndex name_;
  BuffIndex val_;
};

class HtmlToken {
 public:
  ~HtmlToken() {
  }

  inline HtmlTokenType GetType() const {
    return type_;
  }

  inline const HtmlTag* GetTag() const {
    return tag_;
  }

  inline int GetBeginPos() const {
    return buff_index_.GetOffset();
  }

  inline int GetEndPos() const {
    return buff_index_.GetOffset() + buff_index_.GetLength();
  }

  inline int GetLength() const {
    return buff_index_.GetLength();
  }

  inline BuffIndex& GetBuffIndex() {
    return buff_index_;
  }

  inline const BuffIndex& GetTagName() const {
    return tag_name_;
  }

  inline const std::vector<HtmlAttrIndex>* GetAttrs() const {
    return &attrs_;
  }

 private:
  HtmlToken() : type_(TOKEN_TYPE_UNKNOW), tag_(NULL) {
                buff_index_.SetEndPos(0);
  }
  inline void clear() {
    Clear();
  }

  inline void Clear() {
    type_ = TOKEN_TYPE_UNKNOW;
    tag_ = NULL;
    if (attrs_.size() > 0) {
      attrs_.clear();
    }
    buff_index_.Clear();
    tag_name_.Clear();
  }

  inline void SetType(HtmlTokenType type) {
    type_ = type;
  }

  inline void SetTag(const HtmlTag* tag) {
    tag_ = tag;
  }

  inline void SetBeginPos(const int begin_pos) {
    buff_index_.SetOffset(begin_pos);
  }

  inline void SetEndPos(const int end_pos) {
    buff_index_.SetEndPos(end_pos);
  }

 private:
  BuffIndex buff_index_;
  HtmlTokenType type_;
  BuffIndex tag_name_;
  const HtmlTag* tag_;
  std::vector<HtmlAttrIndex> attrs_;
  friend class HtmlLexer;
  friend class HtmlLexerUnittest;
  friend class HtmlTokenPool;
  friend class ObjPool<HtmlToken>;
  DISALLOW_COPY_AND_ASSIGN(HtmlToken);
};
}

#endif  // UTIL_PARSE_MASTER2_INTERNAL_HTML_TOKEN_H_

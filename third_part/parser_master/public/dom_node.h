// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_PUBLIC_DOM_NODE_H_
#define UTIL_PARSE_MASTER2_PUBLIC_DOM_NODE_H_

#include <map>
#include <string>
#include <vector>

#include "base/public/hash_tables.h"
#include "base/public/logging.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "html_tag.h"
#include "obj_pool.h"
#include "buff_index.h"

namespace parse_master {
class BuffIndex;
class HtmlAttrIndex;
class DomNode;
class ParseMaster;
class StringPool;

enum DomNodeType {
  DOM_NODE_ROOT,
  DOM_NODE_TAG,
  DOM_NODE_TEXT,
  DOM_NODE_SCRIPT,
  DOM_NODE_STYLE,
  DOM_NODE_COMMENT,
  DOM_NODE_DOCTYPE,
  DOM_NODE_CDATA,
};

// HtmlAttrIterator using sample code:
//  for (HtmlAttrIterator it = node.GetAttrIter(); it.NotEnd(); it.Next()) {
//    const string* attr_key = it.GetName();
//    const string* attr_value = it.GetValue();
//    LOG(INFO) << *attr_key << "=" << *attr_value;
//  }

class HtmlAttrIterator {
 public:
  explicit HtmlAttrIterator(const DomNode* node) : node_(node), pos_(0) {}
  HtmlAttrIterator(const DomNode* node, int pos) : node_(node), pos_(pos) {}
  bool NotEnd();
  bool Next();
  const std::string* GetName();
  //  NOTE: If a attribute has no value,
  //  GetValue/GetLowerValue returns pointer of a empty string,
  const std::string* GetValue();
  const std::string* GetLowerValue();
 private:
  const DomNode* node_;
  int pos_;
};

const std::string EMPTY;

class DomNode {
 public:
  DomNode();
  // return offset of node buffer start
  int GetHtmlOffset() const;
  
  // return length of node buffer
  int GetHtmlSize() const;
  
  // get begin pointer of buffer
  // note: this buffer is not end with \0, please refer to GetHtmlSize()
  const char* GetBuff() const;
  
  //  if text is <html><body><a>,
  //  function GetXpath will return "//html/body/a"
  //  function GetReverseXpath will return "a/body/html//"
  //  NOTE: if depth of the node is more than kMaxDepthForXpath(100),
  //  below two function will return empty string
  std::string GetXpath() const;
  std::string GetReverseXpath() const;

  //  Similar with GetXpath() and GetReverseXpath(),
  //  but get xpath for only max_level, and return how many level has been
  //  got for xpath
  int GetXpath(int max_level, std::string* out) const;
  int GetReverseXpath(int max_level, std::string* out) const;

  // this function now ASSIGN rather than APPEND tag name to given string
  // if this cause your code break, please using FetchTagNameToTail
  void GetTagName(std::string* tag_name) const;
  // append tag name to tail of given string
  void FetchTagNameToTail(std::string* tag_name) const;

  // this function now ASSIGN rather than APPEND lower tag name
  // to given string
  // if this cause your code break, please using FetchLowerTagNameToTail
  void GetLowerTagName(std::string* tag_name) const;
  void FetchLowerTagNameToTail(std::string* tag_name) const;

  void GetFirstTopContent(std::string* str) const;

  // this function now ASSIGN rather than APPEND html to given string
  // if this cause your code break, please using FetchHtmlToTail
  void GetHtml(std::string* html) const;
  void FetchHtmlToTail(std::string* html) const;

  const std::string& GetLowerTagName() const;

  DomNode* LeftSibling() const;
  DomNode* RightSibling() const;

  void GetInnerHtml(std::string* html) const;

  int32 GetContentLength() const;
  
  // this function now ASSIGN rather than APPEND attribute to given string
  // if this cause your code break, please using FetchContentToTail
  void GetContent(std::string* content) const;
  void GetContent(std::string* content, int32 max_depth) const;
  void FetchContentToTail(std::string* content) const;
  void FetchContentToTail(std::string* content, int32 max_depth) const;

  // do NOT release string ptr in vector pls.
  void GetContent(std::vector<const std::string*>* str_vector) const;
  void GetContent(std::vector<const std::string*>* str_vector,
                  int32 max_depth) const;
  //  NOTE: only text block can call this function, If a domnode is a tag,
  //  this function will return empty string
  const std::string& GetContent() const;

  void GetEscapedContent(std::string* content) const;
  void GetEscapedContent(std::string* content, int32 max_depth) const;

  HtmlAttrIterator GetAttrIter() const;
  
  // return false if not exists attribute identified by name;
  // reutrn true string if the attribute don't have a value.
  // this function now ASSIGN rather than APPEND attribute to given string
  // if this cause your code break, please using FetchAttributeToTail
  bool GetAttribute(const std::string& name, std::string* value) const;
  // please refer to GetAttribute
  bool FetchAttributeToTail(const std::string& name,
                    std::string* value) const;
  
  // return NULL if not exists attribute identified by name
  // return a empty string point if the attribute don't have a value.
  const std::string* GetLowerAttribute(const std::string& name) const;
  const std::string* GetAttribute(const std::string& name) const;
  const std::string* GetAttribute(const char* name, int name_len) const;
  
  // if attribute not exists, Iterator.NotEnd() will be false;
  // if the attribute don't have a value, Iterator.GetValue() will be NULL;
  HtmlAttrIterator GetAttrIter(const std::string& name) const;
  HtmlAttrIterator GetAttrIter(const char* name, int name_len) const;

  // this function should ONLY used for debug, don't use in product code
  // this function now ASSIGN rather than APPEND formal html to given string
  // if this cause your code break, please using FetchHtmlToTail
  void GetFormalHtml(std::string* str) const;
  void FetchFormalHtmlToTail(std::string* str) const;
  void FetchFormalHtmlToTail(int32 max_depth,
                             bool ignore_content,
                             std::string* str) const;
  const std::vector<HtmlAttrIndex>* GetAttrs() const {
    return attrs_;
  }


  const DomNode* Parent() const {
    return parent_;
  }

  const std::vector<DomNode*>& Children() const {
    return children_;
  }

  int ChildrenCount() const {
    return children_.size();
  }

  int GetIndexOfParent() const {
    return index_;
  }

  const HtmlTag* GetTag() const {
    return tag_;
  }

  int GetDepth() const {
    return depth_;
  }

  DomNodeType GetType() const {
    return type_;
  }

  bool IsRoot() const {
    return parent_ == NULL;
  }

  bool IsTextBlock() const {
    return type_ == DOM_NODE_TEXT;
  }

  bool IsCustomTag() const {
    return type_ == DOM_NODE_TAG && tag_ == NULL;
  }

  inline void clear() {
    Clear();
  }

 private:
  struct HtmlAttr {
    HtmlAttr() : name_str_(NULL), val_str_(NULL), val_lower_str_(NULL) {}
    const std::string* name_str_;
    const std::string* val_str_;
    const std::string* val_lower_str_;
  };

  const std::vector<HtmlAttrIndex>::const_iterator GetAttr(const char* name,
                                                           int name_len) const;
  void SetAttrs(std::vector<HtmlAttrIndex>* attrs);
  bool InitAttrStrs() const;
  void SetBeginPos(int begin_pos);
  void SetEndPos(int end_pos);

  void SetBuff(const BuffIndex& buff_index) {
    buff_index_ = buff_index;
  }

  const BuffIndex& GetBuffIndex() const {
    return buff_index_;
  }

  void SetTag(const HtmlTag* tag) {
    tag_ = tag;
  }

  void SetTagName(const BuffIndex& tag_name) {
    tag_name_ = tag_name;
  }

  void ParseContentCached(int32 max_depth) const;
  void ParseContent(int32 max_depth, std::string* str) const;
  void EscapeContentInternal(int32 max_depth) const;
  //  clear all data member before reuse
  void Clear();

 private:
  StringPool* string_pool_;   // point to the sington instance.
  BuffIndex buff_index_;
  DomNodeType type_;
  DomNode* parent_;
  const HtmlTag* tag_;
  BuffIndex tag_name_;
  const std::vector<HtmlAttrIndex>* attrs_;
  
  std::vector<DomNode*> children_;
  int depth_;
  int index_;

  //  cache some data for performance
  mutable std::vector<HtmlAttr> attr_strs_;
  mutable const std::string* tag_name_lower_str_;
  
  // -1 as content_length_ not parsed.
  mutable int content_length_;
  mutable const std::string* content_;
  mutable const std::string* escaped_content_;

  friend class ParseMaster;
  friend class HtmlAttrIterator;
  friend class DomNodePool;
  DISALLOW_COPY_AND_ASSIGN(DomNode);
};
}

#endif  // UTIL_PARSE_MASTER2_PUBLIC_DOM_NODE_H_

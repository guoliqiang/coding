// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include "../public/dom_node.h"
#include <string>
#include <stack>
#include "base/public/hash_tables.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "../public/parse_master.h"
#include "../public/buff_index.h"
#include "../public/html_lexer.h"
#include "third_part/html/public/html_unescape_char.h"

DEFINE_bool(parse_master_cache_content, false,
            "if true, cache content in paser_master");

DECLARE_int32(default_parse_master_max_depth);

using namespace std;  // NOLINT
using namespace base;  // NOLINT

namespace {
static const int kMaxDepthForXpath = 100;
}

namespace parse_master {
bool HtmlAttrIterator::NotEnd() {
  return node_->attrs_ != NULL && (pos_ < node_->attrs_->size());
}

bool HtmlAttrIterator::Next() {
  if (NotEnd()) {
    ++pos_;
    return true;
  }
  return false;
}

const string* HtmlAttrIterator::GetName() {
  const HtmlAttrIndex& attr_index = (*node_->attrs_)[pos_];
  DomNode::HtmlAttr& attr = node_->attr_strs_[pos_];
  if (attr.name_str_ == NULL) {
    attr.name_str_ = node_->string_pool_->GetLowerString(attr_index.name_);
  }
  return attr.name_str_;
}

const string* HtmlAttrIterator::GetValue() {
  const HtmlAttrIndex& attr_index = (*node_->attrs_)[pos_];
  if (attr_index.val_.GetLength() <= 0) {
    return &EMPTY;
  }
  DomNode::HtmlAttr& attr = node_->attr_strs_[pos_];
  if (attr.val_str_ == NULL) {
    attr.val_str_ = node_->string_pool_->GetString(attr_index.val_);
  }
  return attr.val_str_;
}

const string* HtmlAttrIterator::GetLowerValue() {
  const HtmlAttrIndex& attr_index = (*node_->attrs_)[pos_];
  if (attr_index.val_.GetLength() <= 0) {
    return &EMPTY;
  }
  DomNode::HtmlAttr& attr = node_->attr_strs_[pos_];
  if (attr.val_lower_str_ == NULL) {
    attr.val_lower_str_ = node_->string_pool_->GetLowerString(attr_index.val_);
  }
  return attr.val_lower_str_;
}

DomNode::DomNode() : string_pool_(NULL),
                     type_(DOM_NODE_ROOT),
                     parent_(NULL),
                     tag_(NULL),
                     attrs_(NULL),
                     depth_(0),
                     index_(0),
                     tag_name_lower_str_(NULL),
                     content_length_(-1),
                     content_(NULL),
                     escaped_content_(NULL) {}

void DomNode::Clear() {
  type_ = DOM_NODE_ROOT;
  parent_ = NULL;
  tag_ = NULL;
  attrs_ = NULL;
  children_.clear();
  depth_ = 0;
  index_ = 0;
  buff_index_.Clear();
  tag_name_.Clear();

  attr_strs_.clear();
  tag_name_lower_str_ = NULL;
  content_length_ = -1;
  content_ = NULL;
  escaped_content_ = NULL;
}

int DomNode::GetHtmlSize() const {
  return buff_index_.GetLength();
}

int DomNode::GetHtmlOffset() const {
  return buff_index_.GetOffset();
}

const char* DomNode::GetBuff() const {
  return string_pool_->GetBuff() + buff_index_.GetOffset();
}

void DomNode::FetchTagNameToTail(string* tag_name) const {
  tag_name->append(string_pool_->GetBuff() + tag_name_.GetOffset(),
                   tag_name_.GetLength());
}

void DomNode::GetTagName(string* tag_name) const {
  tag_name->assign(string_pool_->GetBuff() + tag_name_.GetOffset(),
                   tag_name_.GetLength());
}

void DomNode::GetLowerTagName(string* tag_name) const {
  tag_name->assign(GetLowerTagName());
}

void DomNode::FetchLowerTagNameToTail(string* tag_name) const {
  tag_name->append(GetLowerTagName());
}

void DomNode::GetFirstTopContent(string* str) const {
  if (IsTextBlock()) {
    str->assign(GetBuff(), buff_index_.GetLength());
    return;
  }
  for (vector<DomNode*>::const_iterator it = children_.begin();
      it != children_.end(); ++it) {
    if ((*it)->IsTextBlock()) {
      str->assign((*it)->GetContent());
      return;
    }
  }
  str->clear();
}

void DomNode::FetchHtmlToTail(string* html) const {
  html->append(GetBuff(), buff_index_.GetLength());
}

void DomNode::GetHtml(string* html) const {
  html->assign(GetBuff(), buff_index_.GetLength());
}

string DomNode::GetXpath() const {
  if (depth_ > kMaxDepthForXpath) return "";
  const DomNode* index = this;
  if (type_ != DOM_NODE_TAG) {
    index = Parent();
  }
  stack<const DomNode*> nodes;
  while (!index->IsRoot()) {
    nodes.push(index);
    index = index->Parent();
  }
  string xpath("/");
  while (!nodes.empty()) {
    xpath += string("/") + nodes.top()->GetLowerTagName();
    nodes.pop();
  }
  return xpath;
}

//  获得逆向的xpath
string DomNode::GetReverseXpath() const {
  if (depth_ > kMaxDepthForXpath) return "";
  const DomNode* index = this;
  if (type_ != DOM_NODE_TAG) {
    index = Parent();
  }
  string xpath;
  while (!index->IsRoot()) {
    xpath += index->GetLowerTagName() + "/";
    index = index->Parent();
  }
  xpath += "/";
  return xpath;
}

int DomNode::GetXpath(int max_level, string* out) const {
  if (depth_ > kMaxDepthForXpath) return 0;
  const DomNode* index = this;
  if (type_ != DOM_NODE_TAG) {
    index = Parent();
  }
  stack<const DomNode*> nodes;
  while (!index->IsRoot()) {
    nodes.push(index);
    index = index->Parent();
  }
  out->assign("/");
  int count = 0;
  while (!nodes.empty()) {
    out->append("/");
    out->append(nodes.top()->GetLowerTagName());
    nodes.pop();
    ++count;
    if (count == max_level) {
      break;
    }
  }
  return count;
}

int DomNode::GetReverseXpath(int max_level, string* out) const {
  if (depth_ > kMaxDepthForXpath) return 0;
  const DomNode* index = this;
  if (type_ != DOM_NODE_TAG) {
    index = Parent();
  }
  int count = 0;
  out->clear();
  while (!index->IsRoot()) {
    out->append(index->GetLowerTagName());
    out->append("/");
    index = index->Parent();
    ++count;
    if (count == max_level) {
      break;
    }
  }
  out->append("/");
  return count;
}

const string& DomNode::GetLowerTagName() const {
  if (tag_ != NULL) {
    return tag_->GetName();
  }

  if (!tag_name_.Empty()) {
    if (!tag_name_lower_str_) {
      tag_name_lower_str_ = string_pool_->GetLowerString(tag_name_);
    }
    return *tag_name_lower_str_;
  }
  return EMPTY;
}

const string& DomNode::GetContent() const {
  if (IsTextBlock()) {
    if (content_ == NULL) {
      content_ = string_pool_->GetString(buff_index_);
    }
    return *content_;
  }
  return EMPTY;
}

DomNode* DomNode::LeftSibling() const {
  if (parent_ == NULL) {
    return NULL;
  }
  if (index_ <= 0  || index_ >= parent_->children_.size()) {
    return NULL;
  }
  return parent_->children_[index_ -1];
}

DomNode* DomNode::RightSibling() const {
  if (parent_ == NULL) {
    return NULL;
  }
  if (index_ < 0  || index_ >= parent_->children_.size() - 1) {
    return NULL;
  }
  return parent_->children_[index_ + 1];
}

void DomNode::FetchFormalHtmlToTail(string* str) const {
  FetchFormalHtmlToTail(0, false, str);
}

void DomNode::FetchFormalHtmlToTail(int32 max_depth,
                                    bool ignore_content,
                                    string* str) const {
  if (max_depth > 0 && GetDepth() > max_depth) {
    return;
  }
  switch (type_) {
  case DOM_NODE_TEXT:
  case DOM_NODE_SCRIPT:
  case DOM_NODE_STYLE:
  case DOM_NODE_COMMENT:
  case DOM_NODE_DOCTYPE:
  case DOM_NODE_CDATA: {
    if (!ignore_content) {
      str->append(GetBuff(), buff_index_.GetLength());
    }
    break;
  }
  case DOM_NODE_ROOT:
  case DOM_NODE_TAG: {
    if (type_ != DOM_NODE_ROOT) {
      str->push_back('<');
      str->append(GetLowerTagName());
      if (attrs_ != NULL) {
        for (vector<HtmlAttrIndex>::const_iterator
            it = attrs_->begin();
            it != attrs_->end(); ++it) {
          str->push_back(' ');
          str->append(string_pool_->GetLowerBuff() + it->name_.GetOffset(),
                      it->name_.GetLength());
          if (!it->val_.Empty()) {
            str->push_back('=');
            str->push_back('"');
            str->append(string_pool_->GetBuff() + it->val_.GetOffset(),
                        it->val_.GetLength());
            str->push_back('"');
          }
        }
      }
      if (tag_ != NULL && tag_->IsSelfClosed()) {
        str->append("/>");
      } else {
        str->push_back('>');
      }
    }
    for (vector<DomNode*>::const_iterator it = children_.begin();
        it != children_.end(); ++it) {
      (*it)->FetchFormalHtmlToTail(max_depth, ignore_content, str);
    }
    if (type_ != DOM_NODE_ROOT && (tag_ == NULL || !tag_->IsSelfClosed())) {
      str->append("</");
      str->append(GetLowerTagName());
      str->push_back('>');
    }
  }
  break;
  default:
    CHECK(false) << "not valid dom node type";
    break;
  }
}

void DomNode::GetInnerHtml(string* html) const {
  switch (type_) {
  case DOM_NODE_SCRIPT:
  case DOM_NODE_STYLE:
  case DOM_NODE_COMMENT:
  case DOM_NODE_TEXT:
  case DOM_NODE_DOCTYPE:
  case DOM_NODE_CDATA:
    return;
  case DOM_NODE_ROOT:
    html->assign(GetBuff(), buff_index_.GetLength());
    return;
  case DOM_NODE_TAG: {
    if (children_.size() <= 0) {
      html->clear();
      return;
    }
    const DomNode* first_child = children_.front();
    const DomNode* last_child = children_.back();
    int inner_len = last_child->GetHtmlOffset() + last_child->GetHtmlSize() -
        first_child->GetHtmlOffset();
    html->assign(first_child->GetBuff(), inner_len);
    return;
  }
  default:
    CHECK(false) << "illegal Type of Node.";
    return;
  }
}

void DomNode::GetFormalHtml(string* str) const {
  str->clear();
  FetchFormalHtmlToTail(str);
}

int32 DomNode::GetContentLength() const {
  if (content_length_ >= 0)
    return content_length_;

  content_length_ = 0;
  if (IsTextBlock()) {
    if (index_ == 0 && parent_->GetTag() == &HtmlTag::STYLE) {
      content_length_ = 0;
    } else {
      content_length_ = buff_index_.GetLength();
    }
  } else if (tag_ == NULL || !tag_->IsSelfClosed()) {  // not self close node
    vector<const string*> contents;
    GetContent(&contents);
    for (vector<const string*>::const_iterator it = contents.begin();
        it != contents.end(); ++it) {
      content_length_ += (*it)->size();
    }
  }
  return content_length_;
}

void DomNode::GetContent(string* str) const {
  GetContent(str, FLAGS_default_parse_master_max_depth);
}

void DomNode::GetContent(string* str, int32 max_depth) const {
  if (!FLAGS_parse_master_cache_content) {
    str->clear();
    ParseContent(max_depth, str);
  } else {
    ParseContentCached(max_depth);
    str->assign(*content_);
  }
}

void DomNode::FetchContentToTail(string* str) const {
  FetchContentToTail(str, FLAGS_default_parse_master_max_depth);
}

void DomNode::FetchContentToTail(string* str, int32 max_depth) const {
  if (!FLAGS_parse_master_cache_content) {
    ParseContent(max_depth, str);
  } else {
    ParseContentCached(max_depth);
    str->append(*content_);
  }
}

void DomNode::ParseContentCached(int32 max_depth) const {
  if (content_) return;
  string* content = string_pool_->Get();
  ParseContent(max_depth, content);
  content_ = content;
}

void DomNode::ParseContent(int32 max_depth, string* str) const {
  vector<const string*> str_vector;
  GetContent(&str_vector, max_depth);
  for (vector<const string*>::iterator it = str_vector.begin();
      it != str_vector.end(); ++it) {
    str->append(*(*it));
  }
}

void DomNode::GetEscapedContent(string* content) const {
  GetEscapedContent(content, FLAGS_default_parse_master_max_depth);
}

void DomNode::GetEscapedContent(string* content, int32 max_depth) const {
  if (!FLAGS_parse_master_cache_content) {
    string str;
    ParseContent(max_depth, &str);
    content->clear();
    string_pool_->GetEscaper()->UnescapeString(str, content);
  } else {
    EscapeContentInternal(max_depth);
    content->assign(*escaped_content_);
  }
}

void DomNode::EscapeContentInternal(int32 max_depth) const {
  if (escaped_content_) {
    return;
  }
  string* escaped_content = string_pool_->Get();
  ParseContentCached(max_depth);
  string_pool_->GetEscaper()->UnescapeString(*content_, escaped_content);
  escaped_content_ = escaped_content;
}

void DomNode::GetContent(vector<const string*>* str_vector) const {
  GetContent(str_vector, FLAGS_default_parse_master_max_depth);
}

void DomNode::GetContent(vector<const string*>* str_vector,
                         int32 max_depth) const {
  switch (type_) {
  case DOM_NODE_ROOT:
    break;
  case DOM_NODE_SCRIPT:
  case DOM_NODE_STYLE:
  case DOM_NODE_COMMENT:
  case DOM_NODE_DOCTYPE:
  case DOM_NODE_CDATA:
    return;
  case DOM_NODE_TEXT: {
    str_vector->push_back(string_pool_->GetString(buff_index_));
    return;
  }
  case DOM_NODE_TAG:
    if (tag_ != NULL && tag_->IsSelfClosed()) {
      // self close node
      return;
    }
    break;
  default:
    CHECK(false) << "not valid dom node type";
    return;
  }

  stack<const DomNode*> node_stack;
  node_stack.push(this);
  while (node_stack.size() > 0) {
    const DomNode* node = node_stack.top();
    node_stack.pop();
    switch (node->GetType()) {
    case DOM_NODE_TEXT: {
      str_vector->push_back(string_pool_->GetString(node->buff_index_));
      break;
    }
    case DOM_NODE_SCRIPT:
    case DOM_NODE_STYLE:
    case DOM_NODE_COMMENT:
    case DOM_NODE_DOCTYPE:
    case DOM_NODE_CDATA: {
      break;
    }
    case DOM_NODE_ROOT:
    case DOM_NODE_TAG: {
      if (node->GetDepth() > max_depth) {
        break;
      }
      for (int i = node->ChildrenCount() - 1; i >= 0; --i) {
        node_stack.push(node->Children()[i]);
      }
      if (node_stack.size() > 1024 * 64) {
        VLOG(2) << "stack size lager than 64K in GetContent, maybe the "
                   "size of html file is too large or"
                   " there too many unclosed tags in html file.";
      }
      break;
    }
    default:
      CHECK(false) << "not valid dom node type";
      break;
    }
  }
}

HtmlAttrIterator DomNode::GetAttrIter() const {
  InitAttrStrs();
  return HtmlAttrIterator(this);
}

bool DomNode::FetchAttributeToTail(const string& name, string* value) const {
  const string* found_value = GetAttribute(name);
  if (found_value != NULL) {
    value->append(*found_value);
    return true;
  }
  return false;
}

bool DomNode::GetAttribute(const string& name, string* value) const {
  const string* found_value = GetAttribute(name);
  if (found_value != NULL) {
    value->assign(*found_value);
    return true;
  }
  return false;
}

const string* DomNode::GetAttribute(const string& name) const {
  return GetAttribute(name.c_str(), name.size());
}

const string* DomNode::GetLowerAttribute(const string& name) const {
  HtmlAttrIterator it = GetAttrIter(name);
  if (it.NotEnd()) {
    return it.GetLowerValue();
  } else {
    return NULL;
  }
}

const string* DomNode::GetAttribute(const char* name, int name_len) const {
  if (!InitAttrStrs()) {
    return NULL;
  }

  const vector<HtmlAttrIndex>::const_iterator& it = GetAttr(name, name_len);
  if (it == attrs_->end()) {
    return NULL;
  }
  if (it->val_.GetLength() <= 0) {
    return &EMPTY;
  }
  HtmlAttr& attr_str = attr_strs_[it - attrs_->begin()];
  if (attr_str.val_str_ == NULL) {
    attr_str.val_str_ = string_pool_->GetString(it->val_);
  }
  return attr_str.val_str_;
}

const vector<HtmlAttrIndex>::const_iterator DomNode::GetAttr(
    const char* name, int name_len) const {
  for (vector<HtmlAttrIndex>::const_iterator it = attrs_->begin();
      it != attrs_->end(); ++it) {
    if (name_len == it->name_.GetLength() &&
        memcmp(name,
               string_pool_->GetLowerBuff() + it->name_.GetOffset(),
               it->name_.GetLength()) == 0) {
      return it;
    }
  }
  return attrs_->end();
}

HtmlAttrIterator DomNode::GetAttrIter(const string& name) const {
  return GetAttrIter(name.c_str(), name.size());
}

HtmlAttrIterator DomNode::GetAttrIter(const char* name, int name_len) const {
  if (!InitAttrStrs()) {
    return HtmlAttrIterator(this);
  }
  return HtmlAttrIterator(this, GetAttr(name, name_len) - attrs_->begin());
}

void DomNode::SetAttrs(vector<HtmlAttrIndex>* attrs) {
  attrs_ = attrs;
}

bool DomNode::InitAttrStrs() const {
  if (attrs_ == NULL || attrs_->empty()) {
    return false;
  }
  if (attr_strs_.empty()) {
    attr_strs_.resize(attrs_->size());
  }
  return true;
}

void DomNode::SetBeginPos(int begin_pos) {
  buff_index_.SetOffset(begin_pos);
}

void DomNode::SetEndPos(int end_pos) {
  buff_index_.SetEndPos(end_pos);
}
}

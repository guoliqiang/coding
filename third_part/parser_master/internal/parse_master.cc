// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <string>
#include <algorithm>
#include "third_part/protobuf/wrapper/public/proto_wrapper.h"
#include "../public/parse_master.h"
#include "base/public/hash_tables.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "third_part/html/public/html_unescape_char.h"
#include "../public/html_lexer.h"
#include "../public/parser_holder.h"
#include "third_part/parser_master/proto/dom_node_meta.pb.h"

using namespace proto_wrapper;  // NOLINT
using namespace base;  // NOLINT
using namespace std;  // NOLINT

//  if depth equal 600 can cover about 99.9902% documents
DEFINE_int32(default_parse_master_max_depth, 600,
             "if depth of DomNode is larger than it,"
             " callbacks won't be called");

//  Cache number of DomNode in factory is less than FLAGS_max_cache_domnode
DEFINE_int32(max_cache_domnode, 128 * 1024, "max cache domnode number");
DEFINE_int32(max_cache_string, 128 * 1024 * 8, "max cache string number");


namespace parse_master {
LexInfo::LexInfo() : custom_tag_open_count_(0), custom_tag_close_count_(0) {
  tag_type_count_ = new int32[TOKEN_TYPE_MAX];
  Reset();
}

void LexInfo::Reset() {
  memset(tag_type_count_, 0, TOKEN_TYPE_MAX * sizeof(*tag_type_count_));
  custom_tag_open_count_= 0;
  custom_tag_close_count_ = 0;
  custom_tags_.clear();
}

int32 LexInfo::GetDoctypeCount() const {
  return tag_type_count_[TOKEN_TYPE_DOCTYPE];
}

int32 LexInfo::GetCommentCount() const {
  return tag_type_count_[TOKEN_TYPE_COMMENT];
}

int32 LexInfo::GetScriptCount() const {
  return tag_type_count_[TOKEN_TYPE_SCRIPT];
}

int32 LexInfo::GetStyleCount() const {
  return tag_type_count_[TOKEN_TYPE_STYLE];
}

int32 LexInfo::GetOpenTagCount() const {
  return tag_type_count_[TOKEN_TYPE_OPEN_TAG];
}

int32 LexInfo::GetSelfCloseTagCount() const {
  return tag_type_count_[TOKEN_TYPE_SELF_CLOSE_TAG];
}

int32 LexInfo::GetTextBlockCount() const {
  return tag_type_count_[TOKEN_TYPE_TEXT_BLOCK];
}

int32 LexInfo::GetCloseTagCount() const {
  return tag_type_count_[TOKEN_TYPE_CLOSE_TAG];
}

int32 LexInfo::GetCDataCount() const {
  return tag_type_count_[TOKEN_TYPE_CDATA];
}

ParseMaster::ParseMaster() : node_pool_(&string_pool_) {
  Init(FLAGS_default_parse_master_max_depth, NULL);
}

ParseMaster::ParseMaster(int32 max_depth,
                         ResultCallback1<bool, const LexInfo&>* lex_callback)
                         : node_pool_(&string_pool_) {
  Init(max_depth, lex_callback);
}

ParseMaster::~ParseMaster() {
  dom_root_.Clear();
  delete parser_mgr_;
  delete lexer_;
  delete lex_callback_;
  delete[] opened_nodes_;
  delete[] lower_buff_;
}

void ParseMaster::Init(int32 max_depth,
                       ResultCallback1<bool, const LexInfo&>* lex_callback) {
  opened_nodes_ = new stack<DomNode*> [HtmlTag::GetTagsCount()];
  lexer_ = new HtmlLexer();
  dom_root_.string_pool_ = &string_pool_;
  parser_mgr_ = new ParsersManager(max_depth);
  lower_buff_ = NULL;
  lex_callback_ = lex_callback;
  max_buff_size_ = 0;
  SetMaxBuffSize(KDefaultMaxBuffSize/*=4M*/);
}

void ParseMaster::Clear() {
  for (int i = 0; i < HtmlTag::GetTagsCount(); ++i) {
    while (!opened_nodes_[i].empty()) {
      opened_nodes_[i].pop();
    }
  }
  node_pool_.Clear();
  string_pool_.Clear();
  opened_custom_nodes_.clear();
  tokens_.clear();
};

void ParseMaster::CreateParsers(const string& parser_name) {
  parser_mgr_->AddParser(parser_name);
}

bool ParseMaster::IsParserExist(const std::string& parser_name) {
  return parser_mgr_->HasParser(parser_name);
}

void ParseMaster::UnregistAllParsers() {
  parser_mgr_->default_parsers()->Clear();
}

bool ParseMaster::SerializeToString(bool text_mode, std::string* str) const {
  // TODO(guoliqiang) use queue instead of vector for nodes
  vector<const DomNode*> nodes;
  nodes.push_back(&dom_root_);
  int head = 0;
  DomNodeMetaList node_meta_list;
  // for debug
  // node_meta_list.set_html(buff_);
  node_meta_list.add_dom_nodes();
  node_meta_list.mutable_dom_nodes(0)->set_parent(-1);
  node_meta_list.mutable_dom_nodes(0)->set_type(0);
  node_meta_list.mutable_dom_nodes(0)->mutable_buff_index()->set_offset(0);
  node_meta_list.mutable_dom_nodes(0)->mutable_buff_index()->set_length(
                                                             buff_len_);
  node_meta_list.mutable_dom_nodes(0)->set_id(0);
  const DomNode* head_node = &dom_root_;
  while (head < node_meta_list.dom_nodes_size()) {
    head_node = nodes[head];
    for (vector<DomNode*>::const_iterator it = head_node->Children().begin();
         it != head_node->Children().end(); ++it) {
      nodes.push_back(*it);
      node_meta_list.add_dom_nodes();
      DomNodeMeta* node = node_meta_list.mutable_dom_nodes(
                                         node_meta_list.dom_nodes_size() -1);
      this->BuffIndexToBuffIndexMeta((*it)->GetBuffIndex(),
                                     node->mutable_buff_index());
      node->set_type((*it)->GetType());
      node->set_parent(head);
      node->set_id(nodes.size() - 1);
      const HtmlTag* tag = (*it)->GetTag();
      if (tag != NULL) {
        node->set_tag(tag->GetIndex());
      }
      if (!(*it)->tag_name_.Empty()) {
        BuffIndexToBuffIndexMeta((*it)->tag_name_, node->mutable_tag_name());
      }
      const vector<HtmlAttrIndex>* attrs = (*it)->attrs_;
      if (attrs != NULL) {
        for (vector<HtmlAttrIndex>::const_iterator a_it = attrs->begin();
             a_it != attrs->end(); ++a_it) {
          const HtmlAttrIndex& attr = *a_it;
          if (!attr.name_.Empty()) {
            node->add_attrs();
            BuffIndexPairMeta* attr_meta = node->mutable_attrs(
                                           node->attrs_size() - 1);
            BuffIndexToBuffIndexMeta(attr.name_, attr_meta->mutable_first());
            if (!attr.val_.Empty()) {
              BuffIndexToBuffIndexMeta(attr.val_, attr_meta->mutable_second());
            }  // if (!attr.val
          }  // if (!attr.name_
        }  // for
      }  // if (attrs !
    }  // for
    ++head;
  }
  if (text_mode) {
    google::protobuf::TextFormat::PrintToString(node_meta_list, str);
  } else {
    node_meta_list.SerializeToString(str);
  }
  return true;
}

bool ParseMaster::UnserializeFromString(bool text_mode,
                                        const std::string& str) {
  Clear();
  DomNodeMetaList node_meta_list;
  if (text_mode) {
    ParseFromASCII(str, &node_meta_list);
  } else {
    node_meta_list.ParseFromString(str);
  }
  if (node_meta_list.dom_nodes_size() <= 0) {
    return false;
  }
  dom_root_.SetBeginPos(0);
  dom_root_.SetEndPos(buff_len_);
  vector<DomNode*> nodes;
  nodes.push_back(&dom_root_);

  for (int i = 1; i < node_meta_list.dom_nodes_size(); ++i) {
    const parse_master::DomNodeMeta& node_meta = node_meta_list.dom_nodes(i);
    DomNode* parent = nodes[node_meta.parent()];

    BuffIndex buff_index =
              this->BuffIndexMetaToBuffIndex(node_meta.buff_index());
    DomNode* node =
             node_pool_.GetDomNode(buff_index, parent,
                                   static_cast<DomNodeType>(node_meta.type()));
    nodes.push_back(node);
    if (node_meta.has_tag()) {
      // HtmlTag structure is saved in HtmlTag.ALL
      node->SetTag(HtmlTag::GetTag(node_meta.tag()));
    }
    if (node_meta.has_tag_name()) {
      node->SetTagName(this->BuffIndexMetaToBuffIndex(node_meta.tag_name()));
    }
    if (node_meta.attrs_size() > 0) {
      // the attrs structure is saved in HtmlToken structure.
      vector<HtmlAttrIndex>* attrs = lexer_->GetAttrVector();
      node->SetAttrs(attrs);
      for (int j = 0; j < node_meta.attrs_size(); ++j) {
        HtmlAttrIndex attr;
        const BuffIndexPairMeta & attr_meta = node_meta.attrs(j);
        attr.name_ = this->BuffIndexMetaToBuffIndex(attr_meta.first());
        if (attr_meta.has_second()) {
          attr.val_= this->BuffIndexMetaToBuffIndex(attr_meta.second());
        }
        attrs->push_back(attr);
      }
    }
  }
  return true;
}

void ParseMaster::SetXmlMode(bool flag) {
  HtmlTag::SetSomeTagsSelfClose(!flag);
}

BuffIndex ParseMaster::BuffIndexMetaToBuffIndex(const BuffIndexMeta&
                                                buff_index_meta) {
  BuffIndex buff_index(buff_index_meta.offset());
  buff_index.SetEndPos(buff_index_meta.offset() + buff_index_meta.length());
  return buff_index;
}

void ParseMaster::BuffIndexToBuffIndexMeta(const BuffIndex& buff_index,
                                           BuffIndexMeta*
                                           buff_index_meta) const {
  if (buff_index.Empty()) {
    return;
  }
  buff_index_meta->set_offset(buff_index.GetOffset());
  buff_index_meta->set_length(buff_index.GetLength());
  // for debug
  // std::string foo;
  // buff_index.ToString(buff_, &foo);
  // buff_index_meta->set_value(foo);
}

void ParseMaster::SetBuff(const char *buff, const int len) {
  buff_ = buff;
  buff_len_ = min(len, max_buff_size_);
  if (len > max_buff_size_) {
    LOG(WARNING) << "buff length:" << len
                 << " max_size:" << max_buff_size_
                 << ", cut it off";
  }

  StringLowerCopy(lower_buff_, buff_, buff_len_);
  string_pool_.SetBuff(buff_, lower_buff_);
  //  reset root ,so if users call Parse more than one times,
  //  ParseMaster won't create dom tree again
  dom_root_.Clear();
}

void ParseMaster::SetBuff(const std::string& buff) {
  SetBuff(buff.c_str(), buff.length());
}

void ParseMaster::SetMaxBuffSize(int len) {
  if (len != max_buff_size_) {
    delete[] lower_buff_;
    max_buff_size_ = len;
    lower_buff_ = new char[max_buff_size_ + 1];
    lower_buff_[max_buff_size_] = '\0';
  }
}

void ParseMaster::RegistParserOnAllTagOpen(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->all_tag_open_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->all_tag_open_parsers_",
               parser_mgr_->default_parsers()->all_tag_open_parsers_.size());
}

void ParseMaster::RegistParserOnTagOpen(const HtmlTag& tag,
                                        Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->tag_open_parsers_[tag.GetIndex()].push_back(
               parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->tag_open_parsers_",
               parser_mgr_->default_parsers()->tag_open_parsers_[tag.GetIndex()].size());  // NOLINT
}

// please ensure the tag_name in lower case.
void ParseMaster::RegistParserOnCustomTagOpen(const string& tag_name,
                                              Callback1<const DomNode&>*
                                              parser) {
  const HtmlTag* tag = HtmlTag::FindTag(tag_name);
  if (tag == NULL) {
    parser_mgr_->default_parsers()->
                 custom_tag_open_parsers_[tag_name].push_back(parser);
    parser_mgr_->default_parsers()->CheckParserSize(
                 "default_parsers_->custom_tag_open_parsers_",
                 parser_mgr_->default_parsers()->custom_tag_open_parsers_[tag_name].size());  // NOLINT
  } else {
    RegistParserOnTagOpen(*tag, parser);
  }
};

void ParseMaster::RegistParserOnAllTagClose(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->all_tag_close_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->all_tag_close_parsers_",
               parser_mgr_->default_parsers()->all_tag_close_parsers_.size());
};

void ParseMaster::RegistParserOnTagClose(const HtmlTag& tag,
                                         Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->tag_close_parsers_[tag.GetIndex()].push_back(parser);  // NOLINT
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->tag_close_parsers_",
               parser_mgr_->default_parsers()->tag_close_parsers_[tag.GetIndex()].size());  // NOLINT
};

// please ensure the tag_name in lower case.
void ParseMaster::RegistParserOnCustomTagClose(const string& tag_name,
                                               Callback1<const DomNode&>* parser) {  // NOLINT
  const HtmlTag* tag = HtmlTag::FindTag(tag_name);
  if (tag == NULL) {
    parser_mgr_->default_parsers()->custom_tag_close_parsers_[tag_name].push_back(parser);  // NOLINT
    parser_mgr_->default_parsers()->CheckParserSize(
                 "default_parsers_->custom_tag_close_parsers_",
                 parser_mgr_->default_parsers()->custom_tag_close_parsers_[tag_name].size());  // NOLINT
  } else {
    RegistParserOnTagClose(*tag, parser);
  }
};

void ParseMaster::RegistParserOnText(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->text_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->text_parsers_",
               parser_mgr_->default_parsers()->text_parsers_.size());
};

void ParseMaster::RegistParserOnStyle(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->style_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->style_parsers_",
               parser_mgr_->default_parsers()->style_parsers_.size());
};

void ParseMaster::RegistParserOnScript(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->script_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->script_parsers_",
               parser_mgr_->default_parsers()->script_parsers_.size());
};

void ParseMaster::RegistParserOnComment(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->comment_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->comment_parsers_",
               parser_mgr_->default_parsers()->comment_parsers_.size());
};

void ParseMaster::RegistParserOnCDATA(Callback1<const DomNode&>* parser) {
  parser_mgr_->default_parsers()->cdata_parsers_.push_back(parser);
  parser_mgr_->default_parsers()->CheckParserSize(
               "default_parsers_->cdata_parsers_",
               parser_mgr_->default_parsers()->cdata_parsers_.size());
};


bool ParseMaster::UnregistParserOnAllTagClose(Callback1<const DomNode&>* parser) {  // NOLINT
  return parser_mgr_->default_parsers()->UnregistParsers(
      &parser_mgr_->default_parsers()->all_tag_close_parsers_, parser);
};

void ParseMaster::RegistNamedParserOnAllTagOpen(const string& parser_name,
                                                Callback1<const DomNode&>* parser) {  // NOLINT
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->all_tag_open_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize("all_tag_open_parsers_",
      parser_mgr_->parsers()[parser_name]->all_tag_open_parsers_.size());
}

void ParseMaster::RegistNamedParserOnTagOpen(const string& parser_name,
                                             const HtmlTag& tag,
                                             Callback1<const DomNode&>* parser) {  // NOLINT
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->tag_open_parsers_[tag.GetIndex()].push_back(parser);  // NOLINT
  parser_mgr_->parsers()[parser_name]->CheckParserSize("tag_open_parsers_",
               parser_mgr_->parsers()[parser_name]->tag_open_parsers_[tag.GetIndex()].size());  // NOLINT
}

// please ensure the tag_name in lower case.
void ParseMaster::RegistNamedParserOnCustomTagOpen(const string& parser_name,
                                                   const string& tag_name,
    Callback1<const DomNode&>* parser) {
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  const HtmlTag* tag = HtmlTag::FindTag(tag_name);
  if (tag == NULL) {
    parser_mgr_->parsers()[parser_name]->custom_tag_open_parsers_[tag_name].push_back(parser);  // NOLINT
    parser_mgr_->parsers()[parser_name]->CheckParserSize(
                 "custom_tag_open_parsers_",
                 parser_mgr_->parsers()[parser_name]->custom_tag_open_parsers_[tag_name].size());  // NOLINT
  } else {
    RegistNamedParserOnTagOpen(parser_name, *tag, parser);
  }
};

void ParseMaster::RegistNamedParserOnAllTagClose(const string& parser_name,
                                                 Callback1<const DomNode&>* parser) {  // NOLINT
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->all_tag_close_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "all_tag_close_parsers_",
               parser_mgr_->parsers()[parser_name]->all_tag_close_parsers_.size());  // NOLINT
};

void ParseMaster::RegistNamedParserOnTagClose(const string& parser_name,
                                              const HtmlTag& tag,
                                              Callback1<const DomNode&>* parser) {  // NOLINT
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->tag_close_parsers_[tag.GetIndex()].push_back(parser);  // NOLINT
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "tag_close_parsers_",
               parser_mgr_->parsers()[parser_name]->tag_close_parsers_[tag.GetIndex()].size());  // NOLINT
};

// please ensure the tag_name in lower case.
void ParseMaster::RegistNamedParserOnCustomTagClose(const string& parser_name,
                                                    const string& tag_name,
                                                    Callback1<const DomNode&>* parser) {  // NOLINT
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  const HtmlTag* tag = HtmlTag::FindTag(tag_name);
  if (tag == NULL) {
    parser_mgr_->parsers()[parser_name]->custom_tag_close_parsers_[tag_name].push_back(parser);  // NOLINT
    parser_mgr_->parsers()[parser_name]->CheckParserSize(
        "custom_tag_close_parsers_",
        parser_mgr_->parsers()[parser_name]->custom_tag_close_parsers_[tag_name].size());  // NOLINT
  } else {
    RegistNamedParserOnTagClose(parser_name, *tag, parser);
  }
};

void ParseMaster::RegistNamedParserOnText(const string& parser_name,
                                          Callback1<const DomNode&>* parser) {
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->text_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "text_parsers_",
               parser_mgr_->parsers()[parser_name]->text_parsers_.size());
};

void ParseMaster::RegistNamedParserOnStyle(const string& parser_name,
                                           Callback1<const DomNode&>* parser) {
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->style_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "style_parsers_",
               parser_mgr_->parsers()[parser_name]->style_parsers_.size());
};

void ParseMaster::RegistNamedParserOnScript(const string& parser_name,
                                            Callback1<const DomNode&>* parser) {
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->script_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "script_parsers_",
               parser_mgr_->parsers()[parser_name]->script_parsers_.size());
};

void ParseMaster::RegistNamedParserOnComment(const string& parser_name,
                                             Callback1<const DomNode&>* parser) {  // NOLINT
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->comment_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "comment_parsers_",
               parser_mgr_->parsers()[parser_name]->comment_parsers_.size());
};

void ParseMaster::RegistNamedParserOnCDATA(const string& parser_name,
                                           Callback1<const DomNode&>* parser) {
  CHECK(parser_mgr_->HasParser(parser_name)) << parser_name << " not exist";
  parser_mgr_->parsers()[parser_name]->cdata_parsers_.push_back(parser);
  parser_mgr_->parsers()[parser_name]->CheckParserSize(
               "cdata_parsers_",
               parser_mgr_->parsers()[parser_name]->cdata_parsers_.size());
};


const DomNode* ParseMaster::Parse() {
  return ParseInternal();
};

const DomNode* ParseMaster::ParseInternal() {
  if (dom_root_.GetBuffIndex().Empty()) {
    ParseToDom(&node_pool_);
  } else {
    DoTraverse(&dom_root_);
  }
  return &dom_root_;
};

void ParseMaster::EnableParser(const std::string& parser_name) {
  parser_mgr_->EnableParser(parser_name);
}

void ParseMaster::EnableParserOnly(const std::string& parser_name) {
  parser_mgr_->EnableParserOnly(parser_name);
}
void ParseMaster::DisableParserOnly(const std::string& parser_name) {
  parser_mgr_->DisableParserOnly(parser_name);
}

void ParseMaster::DisableParser(const std::string& parser_name) {
  parser_mgr_->DisableParser(parser_name);
}

void ParseMaster::ParseToDom(DomNodePool* node_factory) {
  Clear();
  lexer_->ParseToken(lower_buff_, buff_len_, &tokens_);
  if (lex_callback_) {
    lex_info_.Reset();
    for (vector<HtmlToken*>::const_iterator it = tokens_.begin();
        it != tokens_.end(); ++it) {
      HtmlToken* token = *it;
      ++lex_info_.tag_type_count_[token->GetType()];
      switch (token->GetType()) {
      case TOKEN_TYPE_OPEN_TAG:
        if (token->GetTag() == NULL) {
          string tagname;
          token->GetTagName().ToString(lower_buff_, &tagname);
          lex_info_.custom_tags_.insert(tagname);
          ++lex_info_.custom_tag_open_count_;
        }
        break;
      case TOKEN_TYPE_CLOSE_TAG:
        if (token->GetTag() == NULL) {
          string tagname;
          token->GetTagName().ToString(lower_buff_, &tagname);
          lex_info_.custom_tags_.insert(tagname);
          ++lex_info_.custom_tag_close_count_;
        }
        break;
      default:
        break;
      }
    }
    bool continue_parse = lex_callback_->Run(lex_info_);
    if (!continue_parse) return;
  }
  dom_root_.SetBeginPos(0);
  dom_root_.SetEndPos(buff_len_);

  curr_node_ = &dom_root_;
  for (vector<HtmlToken*>::iterator it = tokens_.begin();
      it != tokens_.end();
      ++it) {
    switch ((*it)->GetType()) {
// if need to process token type such as comment/script
// please modify this list
//    case TOKEN_TYPE_UNKNOW:
//    case TOKEN_TYPE_DOCTYPE:
//    case TOKEN_TYPE_COMMENT:
//      break;
//    case TOKEN_TYPE_DOCTYPE:
//      FillDomByDoctypeToken(*it);
//      break;
    case TOKEN_TYPE_OPEN_TAG:
      FillDomByOpenTagToken(*it);
      break;
    case TOKEN_TYPE_SELF_CLOSE_TAG:
      if ((*it)->GetTag() != NULL && (*it)->GetTag()->IsMustHasChild()) {
        FillDomByOpenTagToken(*it);
      } else {
        FillDomBySelfCloseTagToken(*it);
      }
      break;
    case TOKEN_TYPE_TEXT_BLOCK:
      FillDomByTextBlockToken(*it);
      break;
    case TOKEN_TYPE_SCRIPT:
      FillDomByScriptToken(*it);
      break;
    case TOKEN_TYPE_STYLE:
      FillDomByStyleToken(*it);
      break;
    case TOKEN_TYPE_COMMENT:
      FillDomByCommentToken(*it);
      break;
    case TOKEN_TYPE_CDATA:
      FillDomByCdataToken(*it);
      break;
    case TOKEN_TYPE_CLOSE_TAG:
      FillDomByCloseTagToken(*it);
      break;
    default:
      CHECK((*it)->GetType() > 0 && (*it)->GetType() <= TOKEN_TYPE_CLOSE_TAG);
      break;
    }
  }
  while (true) {
    if (curr_node_ == &dom_root_) {
      break;
    }
    curr_node_->SetEndPos(buff_len_);
    if (curr_node_->GetTag() != NULL) {
      parser_mgr_->CallTagCloseParsers(
          *(curr_node_->GetTag()), *curr_node_);
    } else {
      const string& tag_name = curr_node_->GetLowerTagName();
      parser_mgr_->CallTagCloseParsers(tag_name, *curr_node_);
    }
    curr_node_ = curr_node_->parent_;
  }
};

void ParseMaster::DoTraverse(DomNode* dom_root) {
  if (dom_root == NULL) {
    return;
  }
  stack<DomNode*> node_stack;
  DomNode* node = dom_root;
  DomNode* last_parent = node;
  for (int i = node->ChildrenCount() - 1; i >= 0; --i) {
    node_stack.push(node->Children()[i]);
  }
  while (node_stack.size() > 0) {
    node = node_stack.top();
    switch (node->GetType()) {
    case DOM_NODE_TEXT: {
      parser_mgr_->CallTextParsers(*node);
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    case DOM_NODE_SCRIPT: {
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      parser_mgr_->CallScriptParsers(*node);
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    case DOM_NODE_STYLE: {
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      parser_mgr_->CallStyleParsers(*node);
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    case DOM_NODE_COMMENT: {
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      parser_mgr_->CallCommentParsers(*node);
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    case DOM_NODE_DOCTYPE: {
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    case DOM_NODE_CDATA: {
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      parser_mgr_->CallCDATAParsers(*node);
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    default:
      break;
    }
    if (node == last_parent) {
      if (node->GetTag() == NULL) {
        parser_mgr_->CallTagCloseParsers(node->GetLowerTagName(), *node);
      } else {
        parser_mgr_->CallTagCloseParsers(*(node->GetTag()), *node);
      }
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
    if (node->GetTag() == NULL) {
      parser_mgr_->CallTagOpenParsers(node->GetLowerTagName(), *node);
    } else {
      parser_mgr_->CallTagOpenParsers(*(node->GetTag()), *node);
    }
    if (node->ChildrenCount() > 0) {
      for (int i = node->ChildrenCount() - 1; i >= 0; --i) {
        node_stack.push(node->Children()[i]);
      }
    } else {
      if (node->GetTag() == NULL) {
        parser_mgr_->CallTagCloseParsers(node->GetLowerTagName(), *node);
      } else {
        parser_mgr_->CallTagCloseParsers(*(node->GetTag()), *node);
      }
//      DCHECK_GT(node_stack.size(), 0) << "node_stack is empty";
      node_stack.pop();
      last_parent = node->parent_;
      continue;
    }
  }
}

inline void ParseMaster::FillDomByOpenTagToken(HtmlToken* token) {
  //  if (curr_node_->GetDepth() >= FLAGS_default_parse_master_max_depth) {
  //    curr_node_->SetEndPos(token->GetBeginPos());
  //    CloseTagsOpenAfter(curr_node_, token);
  //    LOG(INFO) << "depth overflow.";
  //  }
  const HtmlTag * tag = token->GetTag();
  if (tag != NULL) {
    const vector<const HtmlTag*>& tags = HtmlTagManagerInstance::get()->
          GetCloseBeforeTags(*tag);
    bool before_closed = false;
    for (vector<const HtmlTag*>::const_iterator it = tags.begin();
        it != tags.end(); ++it) {
      if (opened_nodes_[(*it)->GetIndex()].size() <= 0) {
        continue;
      }
      DomNode* last_opened = opened_nodes_[(*it)->GetIndex()].top();
      last_opened->SetEndPos(token->GetBeginPos());
      // close open tags between token~last_opend.
      CloseTagsOpenAfter(last_opened, token);
      before_closed = true;
    }

    if (!before_closed && curr_node_->GetTag() != NULL) {
      const base::hash_set<const HtmlTag*, HtmlTagHash>& pre_close_tags =
            HtmlTagManagerInstance::get()->GetClosePreTags(*tag);
      const base::hash_set<const HtmlTag*, HtmlTagHash>::iterator& find_tag =
            pre_close_tags.find(curr_node_->GetTag());
      if (find_tag != pre_close_tags.end()) {
        curr_node_->SetEndPos(token->GetBeginPos());
        opened_nodes_[curr_node_->GetTag()->GetIndex()].pop();
        parser_mgr_->CallTagCloseParsers(*(curr_node_->GetTag()), *curr_node_);
        curr_node_ = curr_node_->parent_;
      }
    }

    // set node.parent = curr_node_ here.
    DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                          DOM_NODE_TAG);
    node->tag_ = tag;
    node->tag_name_ = token->GetTagName();
    node->attrs_ = token->GetAttrs();

    if (tag->IsSelfClosed()) {
      parser_mgr_->CallTagOpenParsers(*tag, *node);
      parser_mgr_->CallTagCloseParsers(*tag, *node);
      return;
    } else {
      opened_nodes_[tag->GetIndex()].push(node);
      parser_mgr_->CallTagOpenParsers(*tag, *node);
      curr_node_ = node;
    }
  } else {
    DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                          DOM_NODE_TAG);
    node->tag_ = tag;
    node->tag_name_ = token->GetTagName();
    node->attrs_ = token->GetAttrs();
    const string& tag_name = node->GetLowerTagName();
    opened_custom_nodes_[tag_name].push(node);
    parser_mgr_->CallTagOpenParsers(tag_name, *node);
    curr_node_ = node;
  }
}

inline void ParseMaster::FillDomBySelfCloseTagToken(HtmlToken* token) {
  DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                        DOM_NODE_TAG);
  node->tag_ = token->GetTag();
  node->tag_name_ = token->GetTagName();
  node->attrs_ = token->GetAttrs();
  if (token->GetTag() != NULL) {
    parser_mgr_->CallTagOpenParsers(*(token->GetTag()), *node);
    parser_mgr_->CallTagCloseParsers(*(token->GetTag()), *node);
  } else {
    const string& tag_name = node->GetLowerTagName();
    parser_mgr_->CallTagOpenParsers(tag_name, *node);
    parser_mgr_->CallTagCloseParsers(tag_name, *node);
  }
}

inline void ParseMaster::FillDomByTextBlockToken(HtmlToken* token) {
  DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                        DOM_NODE_TEXT);
  if (node->GetDepth() > parser_mgr_->GetMaxDepth()) return;
  parser_mgr_->CallTextParsers(*node);
}

inline void ParseMaster::FillDomByScriptToken(HtmlToken* token) {
  DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                        DOM_NODE_SCRIPT);
  parser_mgr_->CallScriptParsers(*node);
  return;
}

inline void ParseMaster::FillDomByStyleToken(HtmlToken* token) {
  DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                        DOM_NODE_STYLE);
  parser_mgr_->CallStyleParsers(*node);
  return;
}

inline void ParseMaster::FillDomByCommentToken(HtmlToken* token) {
  DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                        DOM_NODE_COMMENT);
  parser_mgr_->CallCommentParsers(*node);
  return;
}

inline void ParseMaster::FillDomByDoctypeToken(HtmlToken* token) {
  node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                        DOM_NODE_DOCTYPE);
  return;
}

inline void ParseMaster::FillDomByCdataToken(HtmlToken* token) {
  DomNode* node = node_pool_.GetDomNode(token->GetBuffIndex(), curr_node_,
                                        DOM_NODE_CDATA);
  parser_mgr_->CallCDATAParsers(*node);
  return;
}

inline void ParseMaster::FillDomByCloseTagToken(HtmlToken* token) {
//  DCHECK(curr_node_ != NULL);
  if (curr_node_->GetTag() != NULL &&
      curr_node_->GetTag() == token->GetTag()) {
    curr_node_->SetEndPos(token->GetEndPos());
//    DCHECK_GT(opened_nodes_[token->GetTag()->GetIndex()].size(), 0) <<
//        "node_stack is empty";
    opened_nodes_[token->GetTag()->GetIndex()].pop();
    parser_mgr_->CallTagCloseParsers(*(token->GetTag()), *curr_node_);
    curr_node_ = curr_node_->parent_;
  } else {
    DomNode* last_opened = NULL;
    if (token->GetTag() != NULL) {
      if (opened_nodes_[token->GetTag()->GetIndex()].size() > 0) {
        last_opened = opened_nodes_[token->GetTag()->GetIndex()].top();
      } else {
        // throw close token which not match an open token away
        return;
      }
    } else {
      map<string, stack<DomNode*> >::iterator found_open =
          opened_custom_nodes_.find(
              string(lower_buff_ + token->GetTagName().GetOffset(),
                     token->GetTagName().GetLength()));
      if (found_open != opened_custom_nodes_.end()
          && found_open->second.size() > 0) {
        last_opened = found_open->second.top();
      } else {
        // throw close token which not match an open token away
        return;
      }
    }

    // last_opened must be not NULL here
    last_opened->SetEndPos(token->GetEndPos());
    CloseTagsOpenAfter(last_opened, token);
  }
}

// close open tags between last_opend and token

inline void ParseMaster::CloseTagsOpenAfter(DomNode* last_opened,
                                            HtmlToken* token) {
  // curr_node == last open node before token
  while (curr_node_->Parent() != NULL) {
    if (curr_node_ != last_opened) {
      curr_node_->SetEndPos(token->GetBeginPos());
      // std::string foo;
      // curr_node_->tag_name_.ToString(buff_, &foo);
      // LOG(INFO) << foo;
    }
    if (curr_node_->GetTag() != NULL) {
//      DCHECK_GT(opened_nodes_[curr_node_->GetTag()->GetIndex()].size(), 0) <<
//          "node_stack is empty";
      opened_nodes_[curr_node_->GetTag()->GetIndex()].pop();
      parser_mgr_->CallTagCloseParsers(*(curr_node_->GetTag()),
                                       *curr_node_);
    } else {
      map<string, stack<DomNode*> >::iterator found_open =
          opened_custom_nodes_.find(curr_node_->GetLowerTagName());
//      DCHECK_GT(found_open->second.size(), 0) <<
//          "node_stack is empty";
      CHECK(found_open != opened_custom_nodes_.end());
      found_open->second.pop();
      parser_mgr_->CallTagCloseParsers(curr_node_->GetLowerTagName(),
                                       *curr_node_);
    }
    if (curr_node_ == last_opened) {
      break;
    }
    curr_node_ = curr_node_->parent_;
//    DCHECK_NE(curr_node_, &dom_root_);
  }
  curr_node_ = curr_node_->parent_;
//  DCHECK(curr_node_);
}

StringPool::StringPool(): ObjPool<string>(FLAGS_max_cache_string),
                          buff_(NULL),
                          lower_buff_(NULL),
                          escaper_(Singleton<util_html::HtmlUtil>::get()) {}

DomNodePool::DomNodePool(StringPool* string_pool): ObjPool<DomNode>(FLAGS_max_cache_domnode),  // NOLINT
                                                   string_pool_(string_pool) {}

DomNode* DomNodePool::GetDomNode(const BuffIndex& buff_index,
                                 DomNode* parent,
                                 DomNodeType type) {
//  DCHECK(parent);
//  DCHECK_GE(buff_index.GetOffset(), 0);
//  DCHECK_GE(buff_index.GetLength(), 0);
  DomNode* node = Get();

  node->string_pool_ = string_pool_;
  node->parent_ = parent;
  node->type_ = type;
  node->buff_index_ = buff_index;
  node->depth_ = parent->depth_ + 1;
  node->index_ = parent->children_.size();
  parent->children_.push_back(node);
  return node;
}
}

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "../public/path_master.h"
#include <queue>
#include "third_part/gtl/public/stl_util-inl.h"
#include "../public/parser.h"
#include "../public/path_item.h"
#include "third_part/re2/include/re2/re2.h"

using std::string;
using std::vector;
using std::map;
using base::hash_map;
using base::NewPermanentCallback;

namespace path_master {

using parse_master::ParseMaster;
using parse_master::DomNode;

PathMaster::PathMaster(): parse_master_(NULL),
                          current_parser_(NULL),
                          callback_unregisted_(false) {
  callback_ = NewPermanentCallback(this,
              &PathMaster::RunCallbackOnCloseTag);
}

bool PathMaster::RegistParser(const string& parser,
                              const string& xpath,
                              base::ResultCallback1<bool, ParserParam*>*
                              callback,
                              CallBackBindInfo* bind_info) {
  Parser* tmp = FindParser(parser);
  return tmp->AddPath(xpath, callback, bind_info);
}

void PathMaster::UnregistAllParsers() {
  gtl::STLDeleteValues(&parsers_);
}

bool PathMaster::RegistParser(const string& parser,
                              const string& xpath,
                              base::ResultCallback1<bool, ParserParam*>*
                              callback) {
  return RegistParser(parser, xpath, callback, NULL);
}

void PathMaster::SetForceCheckAttrs(const string& parser,
                                     vector<string>& force_check_attrs) {
  map<string, Parser*>::iterator it = parsers_.find(parser);
  CHECK(it != parsers_.end()) << "no such parser:" << parser;
  it->second->set_force_check_attrs(force_check_attrs);
}

void PathMaster::RunCallbackOnCloseTag(const DomNode& node) {
  if (current_parser_ == NULL) {
    return;
  }
  vector<PathItem*>& paths = current_parser_->paths();

  for (unsigned int i = 0; i < paths.size(); ++i) {
    MatchPath(paths[i], node, current_parser_->force_check_attrs());
  }
}

void PathMaster::Init(ParseMaster* pm) {
  parse_master_ = pm;
  parse_master_->RegistParserOnAllTagClose(callback_);
}

void PathMaster::UnRegistCallback() {
  if (!callback_unregisted_) {
    CHECK(parse_master_->UnregistParserOnAllTagClose(callback_));
    callback_unregisted_ = true;
  }
}

void PathMaster::RegistCallback() {
  if (callback_unregisted_) {
    parse_master_->RegistParserOnAllTagClose(callback_);
    callback_unregisted_ = false;
  }
}

PathMaster::~PathMaster() {
  gtl::STLDeleteValues(&parsers_);
  gtl::STLDeleteValues(&regx_re2_map_);
  if (callback_unregisted_) {
    delete callback_;
  }
}

bool PathMaster::MatchPath(PathItem* item,
                           const DomNode& node,
                           const vector<string>& force_check_attrs) {
  const PathInfo& path = item->path_info();
  int path_depth = path.tags.size();

  DomNode* current = const_cast<DomNode*> (&node);
  // depth not match
  if (current->GetDepth() < path_depth) {
    return false;
  }
  int index = path_depth - 1;
  // index not match
  if ((path.tags[index].index != -1) &&
      (current->GetIndexOfParent() != path.tags[index].index)) {
      return false;
  }
  if (!MatchTag(path.tags[index--], current, force_check_attrs)) {
    return false;
  }

  int pass = false;
  if (index == -1) {
    pass = true;
  } else {
    const DomNode* parent = current->Parent();
    while (parent != parse_master_->GetDomRoot()) {
      if ((path.tags[index].index != -1) &&
          parent->GetIndexOfParent() != path.tags[index].index) {
          pass = false;
          break;
      }
      if (!MatchTag(path.tags[index--], parent, force_check_attrs)) {
        pass = false;
        break;
      }
      // NOTE: xpath can sub-path in the dom tree
      if (index == -1) {
        pass = true;
        break;
      }
      parent = parent->Parent();
    }
  }

  //  call hook functions
  if (pass) {
    ParserParam param(current, &item->path(), item->bind_info());
    item->RunCallBackFuntion(&param);
  }
  return true;
}

Parser* PathMaster::FindParser(const string& parser_name) {
  map<string, Parser*>::iterator i = parsers_.find(parser_name);
  if (i == parsers_.end()) {
    Parser* parser = new Parser;
    parsers_[parser_name] = parser;
    return parser;
  } else {
    return i->second;
  }
}

void PathMaster::Reset() {
  current_parser_ = NULL;
}

void PathMaster::ResetCurrentParser(string parser_name) {
  CHECK(parsers_.find(parser_name) != parsers_.end())
        << "no such parser_name, "
        << "check you have regist a parser with name:"
        << parser_name;
  current_parser_ = parsers_[parser_name];
}

bool PathMaster::MatchTag(const PathTag& tag,
                          const DomNode* node,
                          const vector<string>& force_check_attrs) {
  if (node->IsTextBlock() || node->IsRoot()) {
    return false;
  }
  //  Tag name not match
  const string& tagname = node->GetLowerTagName();
  if (tagname.compare(tag.name) != 0) {
    return false;
  }
  //  Compare all attributes
  for (size_t i = 0; i < tag.attributes.size(); ++i) {
    const string* v = node->GetAttribute(tag.attributes[i].first);
    if (!v) {
      return false;
    }
    string value;
    TrimWhitespace(*v, TRIM_ALL, &value);
    //   Attribute value no match
    if (!tag.attributes[i].second.value.empty()) {
      if (tag.attributes[i].second.is_regx) {
        re2::RE2* re2obj = GetRe2Object(tag.attributes[i].second.value);
        if (!re2::RE2::FullMatch(value, *re2obj)) {
          return false;
        } else {
          VLOG(4) << "match by pattern:" << re2obj->pattern()
                  << ", value:" << value;
        }
      } else if (tag.attributes[i].second.value != value) {
        return false;
      }
    }
  }
  //  Check for force_check_attrs, if config path does not contain it,
  //  target xpath should not contain it too.
  for (size_t i = 0; i < force_check_attrs.size(); ++i) {
    bool has_this_attr = false;
    for (int k = 0; k < tag.attributes.size(); ++k) {
      if (tag.attributes[k].first == force_check_attrs[i]) {
        has_this_attr = true;
        break;
      }
    }
    if (!has_this_attr) {
      const string* v = node->GetAttribute(force_check_attrs[i]);
      if (v) {
        return false;
      }
    }
  }
  return true;
}

re2::RE2* PathMaster::GetRe2Object(const string& regx) {
  hash_map<string, re2::RE2*>::iterator iter =
      regx_re2_map_.find(regx);
  if (iter == regx_re2_map_.end()) {
    re2::RE2* inst = new re2::RE2(regx);
    regx_re2_map_.insert(make_pair(regx, inst));
    return inst;
  } else {
    return iter->second;
  }
}
}

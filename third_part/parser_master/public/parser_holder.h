// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_INTERNAL_PARSER_HOLDER_H_
#define UTIL_PARSE_MASTER2_INTERNAL_PARSER_HOLDER_H_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "base/public/callback.h"
#include "base/public/logging.h"
#include "base/public/hash_tables.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "html_tag.h"
#include "dom_node.h"


namespace parse_master {
extern const std::string kDefaultParsers;


struct Parsers {
  Parsers();
  ~Parsers();

  inline void Clear() {
    for (int i = 0; i < HtmlTag::GetTagsCount(); ++i) {
      gtl::STLDeleteElements(tag_open_parsers_ + i);
      gtl::STLDeleteElements(tag_close_parsers_ + i);
    }
    for (std::map<std::string,
            std::vector<base::Callback1<const DomNode&>*> >::iterator it
        = custom_tag_open_parsers_.begin();
        it != custom_tag_open_parsers_.end();
        ++it) {
      gtl::STLDeleteElements(&(it->second));
    }
    for (std::map<std::string,
            std::vector<base::Callback1<const DomNode&>*> >::iterator it
        = custom_tag_close_parsers_.begin();
        it != custom_tag_close_parsers_.end();
        ++it) {
      gtl::STLDeleteElements(&(it->second));
    }
    gtl::STLDeleteElements(&all_tag_open_parsers_);
    gtl::STLDeleteElements(&all_tag_close_parsers_);
    gtl::STLDeleteElements(&(text_parsers_));
    gtl::STLDeleteElements(&script_parsers_);
    gtl::STLDeleteElements(&style_parsers_);
    gtl::STLDeleteElements(&comment_parsers_);
    gtl::STLDeleteElements(&cdata_parsers_);
  }

  inline void CallTagOpenParsers(const HtmlTag& tag, const DomNode& dom_node) {
    CallParsers(tag_open_parsers_[tag.GetIndex()], dom_node);
    CallParsers(all_tag_open_parsers_, dom_node);
  }

  inline void CallTagOpenParsers(const std::string& tag_name,
                                 const DomNode& dom_node) {
    if (custom_tag_open_parsers_.find(tag_name)
        != custom_tag_open_parsers_.end()) {
      CallParsers(custom_tag_open_parsers_[tag_name], dom_node);
    }
    CallParsers(all_tag_open_parsers_, dom_node);
  }

  inline void CallTagCloseParsers(const HtmlTag& tag, const DomNode& dom_node) {
    CallParsers(tag_close_parsers_[tag.GetIndex()], dom_node);
    CallParsers(all_tag_close_parsers_, dom_node);
  }

  inline void CallTagCloseParsers(const std::string& tag_name,
                                  const DomNode& dom_node) {
    if (custom_tag_close_parsers_.find(tag_name)
        != custom_tag_close_parsers_.end()) {
      CallParsers(custom_tag_close_parsers_[tag_name], dom_node);
    }
    CallParsers(all_tag_close_parsers_, dom_node);
  }

  inline void CallTextParsers(const DomNode& dom_node) {
    CallParsers(text_parsers_, dom_node);
  }

  inline void CallScriptParsers(const DomNode& dom_node) {
    CallParsers(script_parsers_, dom_node);
  }

  inline void CallStyleParsers(const DomNode& dom_node) {
    CallParsers(style_parsers_, dom_node);
  }

  inline void CallCommentParsers(const DomNode& dom_node) {
    CallParsers(comment_parsers_, dom_node);
  }

  inline void CallCDATAParsers(const DomNode& dom_node) {
    CallParsers(cdata_parsers_, dom_node);
  }


  inline void CallParsers(const std::vector<base::Callback1<const DomNode&>*>&
                          parsers,
                          const DomNode& dom_node) {
    for (std::vector<base::Callback1<const DomNode&>*>::const_iterator
        it = parsers.begin();
        it != parsers.end();
        ++it) {
      (*it)->Run(dom_node);
    }
  }

  inline bool UnregistParsers(std::vector<base::Callback1<const DomNode&>*>* parsers,
                              base::Callback1<const DomNode&>* parser) {
    for (std::vector<base::Callback1<const DomNode&>*>::iterator
        it = parsers->begin(); it != parsers->end(); ++it) {
      if (*it == parser) {
        parsers->erase(it);
        return true;
      }
    }
    return false;
  }

  void CheckParserSize(std::string msg, int size) {
    if (size > 1024) {
      LOG(WARNING) << msg << " size larger than: " << size;
    }
  }

  // the tag is specified by index.
  std::vector<base::Callback1<const DomNode&>*>* tag_open_parsers_;
  std::vector<base::Callback1<const DomNode&>*>* tag_close_parsers_;
  std::map<std::string, std::vector<base::Callback1<const DomNode&>*> >
       custom_tag_open_parsers_;
  std::map<std::string, std::vector<base::Callback1<const DomNode&>*> >
       custom_tag_close_parsers_;
  std::vector<base::Callback1<const DomNode&>*> all_tag_open_parsers_;
  std::vector<base::Callback1<const DomNode&>*> all_tag_close_parsers_;
  std::vector<base::Callback1<const DomNode&>*> text_parsers_;
  std::vector<base::Callback1<const DomNode&>*> script_parsers_;
  std::vector<base::Callback1<const DomNode&>*> style_parsers_;
  std::vector<base::Callback1<const DomNode&>*> comment_parsers_;
  std::vector<base::Callback1<const DomNode&>*> cdata_parsers_;
  bool active_;
 private:
  DISALLOW_COPY_AND_ASSIGN(Parsers);
};

//  a wrapper of Parser
class ParsersManager {
 public:
  explicit ParsersManager(int32 max_depth) : max_depth_(max_depth) {
    AddParser(kDefaultParsers);
  }
  ~ParsersManager() {
    gtl::STLDeleteValues(&parsers_);
  }

  inline void CallTagOpenParsers(const HtmlTag& tag,
                                 const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;
    
    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallTagOpenParsers(tag, dom_node);
      }
    }
  }

  inline void CallTagOpenParsers(const std::string& tag_name,
                                 const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallTagOpenParsers(tag_name, dom_node);
      }
    }
  }

  inline void CallTagCloseParsers(const HtmlTag& tag, const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallTagCloseParsers(tag, dom_node);
      }
    }
  }

  inline void CallTagCloseParsers(const std::string& tag_name,
      const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallTagCloseParsers(tag_name, dom_node);
      }
    }
  }

  inline void CallTextParsers(const DomNode& dom_node) {
    // max_depth_ is checked by ParserMaster::FillDomByTextBlockToken
    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallTextParsers(dom_node);
      }
    }
  }

  inline void CallScriptParsers(const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallScriptParsers(dom_node);
      }
    }
  }

  inline void CallStyleParsers(const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallStyleParsers(dom_node);
      }
    }
  }

  inline void CallCommentParsers(const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallCommentParsers(dom_node);
      }
    }
  }

  inline void CallCDATAParsers(const DomNode& dom_node) {
    if (dom_node.GetDepth() > max_depth_) return;

    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->second->active_) {
        iter->second->CallCDATAParsers(dom_node);
      }
    }
  }

  inline void EnableParser(const std::string& parser_name) {
    CHECK(parsers_.find(parser_name) != parsers_.end())
        << parser_name << " not exist!";
    parsers_[parser_name]->active_ = true;
  }

  inline void DisableParser(const std::string& parser_name) {
    CHECK(parsers_.find(parser_name) != parsers_.end())
        << parser_name << " not exist!";
    parsers_[parser_name]->active_ = false;
  }

  void EnableParserOnly(const std::string& parser_name) {
    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->first != parser_name) {
        iter->second->active_ = false;
      } else {
        iter->second->active_ = true;
      }
    }
  }

  void DisableParserOnly(const std::string& parser_name) {
    std::map< std::string, Parsers*>::iterator iter = parsers_.begin();
    for (; iter != parsers_.end(); ++iter) {
      if (iter->first != parser_name) {
        iter->second->active_ = true;
      } else {
        iter->second->active_ = false;
      }
    }
  }

  inline bool HasParser(const std::string& parser_name) const {
    return parsers_.find(parser_name) != parsers_.end();
  }

  inline bool AddParser(const std::string& parser_name) {
    CHECK(!HasParser(parser_name)) << parser_name << " has been exist!";
    parsers_[parser_name] = new Parsers;
    return true;
  }

  inline  Parsers* GetParser(const std::string& parser_name) {
    return parsers_[parser_name];
  }

  std::map<std::string, Parsers*>& parsers() {
    return parsers_;
  }
  Parsers* default_parsers() {
    return parsers_[kDefaultParsers];
  }

  int32 GetMaxDepth() {
    return max_depth_;
  }
 private:
  std::map<std::string, Parsers*> parsers_;
  int32 max_depth_;
  DISALLOW_COPY_AND_ASSIGN(ParsersManager);
};
}

#endif  // UTIL_PARSE_MASTER2_INTERNAL_PARSER_HOLDER_H_

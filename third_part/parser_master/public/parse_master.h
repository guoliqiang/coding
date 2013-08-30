// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_PUBLIC_PARSE_MASTER_H_
#define UTIL_PARSE_MASTER2_PUBLIC_PARSE_MASTER_H_

#include <map>
#include <stack>
#include <string>
#include <vector>

#include "base/public/callback.h"
#include "base/public/flags.h"
#include "base/public/hash_tables.h"
#include "third_part/testing/gtest/include/gtest/gtest_prod.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "html_tag.h"
#include "dom_node.h"

DECLARE_int32(default_parse_master_max_depth);

namespace util_html {
class HtmlUtil;
}

namespace path_master {
class PathMaster;
}


namespace parse_master {

class HtmlToken;
class HtmlTagType;
class HtmlLexer;
class ParseMaster;
class Parsers;
class ParsersManager;
class BuffIndexMeta;

// only one instance in parser master
class StringPool : public ObjPool<std::string> {
 public:
  StringPool();
  
  inline void SetBuff(const char* buff, const char* lower_buff) {
    buff_ = buff;
    lower_buff_ = lower_buff;
  }

  inline const std::string* GetString(BuffIndex buff_index) {
    return buff_index.ToString(buff_, Get());
  }

  inline const std::string* GetLowerString(BuffIndex buff_index) {
    return buff_index.ToString(lower_buff_, Get());
  }

  inline util_html::HtmlUtil* GetEscaper() const {
    return escaper_;
  }

  inline const char* GetBuff() {
    return buff_;
  }

  inline const char* GetLowerBuff() {
    return lower_buff_;
  }

 private:
  const char* buff_;              // html raw string
  const char* lower_buff_;        // lower html raw string
  util_html::HtmlUtil* escaper_;
};

// only one instance in parser master
class DomNodePool : public ObjPool<DomNode> {
 public:
  explicit DomNodePool(StringPool* string_pool);

  DomNode* GetDomNode(const BuffIndex& buff_index,
                      DomNode* parent,
                      DomNodeType type);
 private:
  StringPool* string_pool_;   // point to the sington instance
  DISALLOW_COPY_AND_ASSIGN(DomNodePool);
};

//  payload for defalut parsers, all callbacks registered to it will be called
//  when call Parse
static const std::string kDefaultParsers = "__default_parsers";
//  4M
static const int KDefaultMaxBuffSize = 1024* 1024 * 4;

class LexInfo {
 public:
  LexInfo();
  
  ~LexInfo() {
    delete[] tag_type_count_;
  }

  int32 GetCustomTagOpenCount() const {
    return custom_tag_open_count_;
  }

  int32 GetCustomTagCloseCount() const {
    return custom_tag_close_count_;
  }

  int32 GetDoctypeCount() const;
  int32 GetCommentCount() const;
  int32 GetScriptCount() const;
  int32 GetStyleCount() const;
  int32 GetOpenTagCount() const;
  int32 GetSelfCloseTagCount() const;
  int32 GetTextBlockCount() const;
  int32 GetCloseTagCount() const;
  int32 GetCDataCount() const;
  
  const base::hash_set<std::string>& custom_tags() const {
    return custom_tags_;
  }

 private:
  void Reset();
  int32* tag_type_count_;
  int32 custom_tag_open_count_;
  int32 custom_tag_close_count_;
  base::hash_set<std::string> custom_tags_;
  friend class ParseMaster;
};

class ParseMaster {
 public:
  ParseMaster();
  ParseMaster(int32 max_depth,
              base::ResultCallback1<bool, const LexInfo&>* lex_callback);
  ~ParseMaster();
  // parse given char buffer to dom tree and return root of the tree
  // should be called after SetBuff(const char*, int), if call more than one
  //  time, it won't re-create dom tree again.
  const DomNode* Parse();
  // Clear function will release domnode and string cached by parse master
  // call this function ONLY if the life cycle of the parse result.
  // it will be call automatic before parsing.
  void Clear();
  void EnableParser(const std::string& parser_name);
  void DisableParser(const std::string& parser_name);
  //  this function will disable all other parser besides parser_name.
  void EnableParserOnly(const std::string& parser_name);
  // This function will enable all other parsers and disable parser_name
  void DisableParserOnly(const std::string& parser_name);
//  void ResetCurrentParser(const std::string& parser_name);
  //  create a new parsers set, and then we can register callbacks to it.
  //  when Parse(parse_name), all callbacks will be called.
  void CreateParsers(const std::string& parser_name);
  bool IsParserExist(const std::string& parser_name);
  // unregister all registered parsers
  void UnregistAllParsers() YR_DEPRECATED;
  // serialize dom tree to string , note: call it after Parse()
  bool SerializeToString(bool text_mode, std::string* str) const;
  // unserialize dom tree from given string
  // note: call it after SetBuff(const char*, int)
  bool UnserializeFromString(bool text_mode, const std::string& str);
  void SetXmlMode(bool flag);
  // ======================== Parser Register Functions Begin ================
  // note: in this call back, parameter DomNode only contain information of
  // open tag, if you want get children and end tag, please register parser
  // on tag close
  void RegistParserOnAllTagOpen(base::Callback1<const DomNode&>* parser);
  void RegistParserOnTagOpen(const HtmlTag& tag,
      base::Callback1<const DomNode&>* parser);
  // please ensure the tag_name in lower case.
  void RegistParserOnCustomTagOpen(const std::string& tag_name,
      base::Callback1<const DomNode&>* parser);
  void RegistParserOnAllTagClose(base::Callback1<const DomNode&>* parser);
  void RegistParserOnTagClose(const HtmlTag& tag,
      base::Callback1<const DomNode&>* parser);
  // please ensure the tag_name in lower case.
  void RegistParserOnCustomTagClose(const std::string& tag_name,
      base::Callback1<const DomNode&>* parser);
  void RegistParserOnText(base::Callback1<const DomNode&>* parser);
  void RegistParserOnScript(base::Callback1<const DomNode&>* parser);
  void RegistParserOnStyle(base::Callback1<const DomNode&>* parser);
  void RegistParserOnComment(base::Callback1<const DomNode&>* parser);
  void RegistParserOnCDATA(base::Callback1<const DomNode&>* parser);

  // ========================================================================
  //  扩展的接口，可以注册到以parser_name为名词的回调函数集合
  void RegistNamedParserOnAllTagOpen(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnTagOpen(const std::string& parser_name,
      const HtmlTag& tag,
      base::Callback1<const DomNode&>* parser);
  // please ensure the tag_name in lower case.
  void RegistNamedParserOnCustomTagOpen(const std::string& parser_name,
      const std::string& tag_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnAllTagClose(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnTagClose(const std::string& parser_name,
      const HtmlTag& tag,
      base::Callback1<const DomNode&>* parser);
  // please ensure the tag_name in lower case.
  void RegistNamedParserOnCustomTagClose(const std::string& parser_name,
      const std::string& tag_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnText(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnScript(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnStyle(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnComment(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);
  void RegistNamedParserOnCDATA(const std::string& parser_name,
      base::Callback1<const DomNode&>* parser);

  //  NOTE: if len >= max_buff_size, we will cut it off to max_buff_size.
  //  default max_buff_size is 4M, if you want to parse larger file,
  //  you must call SetMaxBuffSize first.
  void SetBuff(const char *buff, const int len);
  void SetBuff(const std::string& buff);
  void SetMaxBuffSize(int len);
  inline const char* GetBuff() const {
    return buff_;
  }
  inline const char* GetLowerBuff() const {
    return lower_buff_;
  }


  inline const DomNode* GetDomRoot() {
    return &dom_root_;
  }

  template<class T>
  void RegistParserOnAllTagOpen(T *obj, void(T::*method)(const DomNode&)) {
    RegistParserOnAllTagOpen(base::NewPermanentCallback(obj, method));
  }

  template<class T>
  inline void RegistParserOnTagOpen(const HtmlTag& tag, T *obj,
      void(T::*method)(const DomNode&)) {
    RegistParserOnTagOpen(tag, base::NewPermanentCallback(obj, method));
  }

  // compatible with old parse_master
  // same as RegistParserOnTagOpen
  template<class T>
  inline void RegistOpenTagParser(const HtmlTag& tag, T *obj,
      void(T::*method)(const DomNode&)) {
    RegistParserOnTagOpen(tag, obj, method);
  }

  // please ensure the tag_name in lower case.
  template<class T>
  inline void RegistParserOnCustomTagOpen(const std::string& tag_name, T *obj,
      void(T::*method)(const DomNode&)) {
    RegistParserOnCustomTagOpen(tag_name,
        base::NewPermanentCallback(obj, method));
  };

  template<class T>
  void RegistParserOnAllTagClose(T *obj, void(T::*method)(const DomNode&)) {
    RegistParserOnAllTagClose(base::NewPermanentCallback(obj, method));
  };

  template<class T>
  inline void RegistParserOnTagClose(const HtmlTag& tag, T *obj,
      void(T::*method)(const DomNode&)) {
    RegistParserOnTagClose(tag, base::NewPermanentCallback(obj, method));
  };

  // compatible with old parse_master
  // same as RegistParserOnTagClose
  template<class T>
  inline void RegistCloseTagParser(const HtmlTag& tag, T *obj,
      void(T::*method)(const DomNode&)) {
    RegistParserOnTagClose(tag, obj, method);
  }

  // please ensure the tag_name in lower case.
  template<class T>
  inline void RegistParserOnCustomTagClose(const std::string& tag_name, T *obj,
      void(T::*method)(const DomNode&)) {
    RegistParserOnCustomTagClose(tag_name,
        base::NewPermanentCallback(obj, method));
  };

  template<class T>
  inline void RegistParserOnText(T *obj, void(T::*method)(const DomNode&)) {
    RegistParserOnText(base::NewPermanentCallback(obj, method));
  };

  // compatible with old parse_master
  // same as RegistParserOnText
  template<class T>
  inline void RegistTextParser(T *obj, void(T::*method)(const DomNode&)) {
    RegistParserOnText(obj, method);
  }

 protected:
  //  below function is used by PathMaster
  bool UnregistParserOnAllTagClose(base::Callback1<const DomNode&>* parser);

 private:
  const DomNode* ParseInternal();
  void Init(int32 max_depth,
            base::ResultCallback1<bool, const LexInfo&>* lex_callback);
  void ParseToDom(DomNodePool* node_factory);
  void FillDomByOpenTagToken(HtmlToken* token);
  void FillDomBySelfCloseTagToken(HtmlToken* token);
  void FillDomByTextBlockToken(HtmlToken* token);
  void FillDomByScriptToken(HtmlToken* token);
  void FillDomByStyleToken(HtmlToken* token);
  void FillDomByCommentToken(HtmlToken* token);
  void FillDomByDoctypeToken(HtmlToken* token);
  void FillDomByCdataToken(HtmlToken* token);
  void FillDomByCloseTagToken(HtmlToken* token);
  void CloseTagsOpenAfter(DomNode* last_opened, HtmlToken* token);
  BuffIndex BuffIndexMetaToBuffIndex(const BuffIndexMeta& buff_index_meta);
  void BuffIndexToBuffIndexMeta(const BuffIndex& buff_index,
                                BuffIndexMeta* buff_index_meta) const;
  // traverse a dom tree and call registered parsers_
  void DoTraverse(DomNode* dom_node);

  HtmlLexer* lexer_;
  LexInfo lex_info_;
  base::ResultCallback1<bool, const LexInfo&>* lex_callback_;
  mutable StringPool string_pool_;
  DomNodePool node_pool_;
  const char* buff_;
  char* lower_buff_;
  int buff_len_;
  std::vector<HtmlToken*> tokens_;
  ParsersManager* parser_mgr_;
  std::stack<DomNode*>* opened_nodes_;
  std::map<std::string, std::stack<DomNode*> > opened_custom_nodes_;
  DomNode dom_root_;
  DomNode* curr_node_;
  HtmlToken* curr_token_;

  int max_buff_size_;
  FRIEND_TEST(ParseMasterUnittest, ParseToDom);
  DISALLOW_COPY_AND_ASSIGN(ParseMaster);
  
  friend class path_master::PathMaster;
};
}  // namespace parse_master

#endif  // UTIL_PARSE_MASTER2_PUBLIC_PARSE_MASTER_H_

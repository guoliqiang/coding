// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_INTERNAL_HTML_LEXER_H_
#define UTIL_PARSE_MASTER2_INTERNAL_HTML_LEXER_H_

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "html_tag.h"
#include "html_token.h"
#include "obj_pool.h"
#include "base/public/rune.h"

namespace parse_master {
enum HtmlLexState {
  LEX_STATE_BEGIN,
  LEX_STATE_TAG_BEGIN,
  LEX_STATE_PROLOG_BEGIN,
  LEX_STATE_TAG_NAME,
  LEX_STATE_TAG_END_NAME,
  LEX_STATE_TAG_END_AFTER_NAME,
  LEX_STATE_CLOSE_TAG_BEGIN,
  LEX_STATE_TEXT_BEGIN,
  LEX_STATE_TEXT_BLOCK,
  LEX_STATE_COMMENT,
  LEX_STATE_DOCTYPE,
  LEX_STATE_ATTR_NAME_BEGIN,
  LEX_STATE_ATTR_NAME,
  LEX_STATE_ATTR_NAME_END,
  LEX_STATE_ATTR_VALUE_BEGIN,
  LEX_STATE_ATTR_QUOTED_VALUE,
  LEX_STATE_ATTR_VALUE,
  LEX_STATE_SCRIPT,
  LEX_STATE_SCRIPT_END_BEGIN,
  LEX_STATE_SCRIPT_END,
  LEX_STATE_STYLE,
  LEX_STATE_STYLE_END_BEGIN,
  LEX_STATE_STYLE_END,
  LEX_STATE_TEXTAREA,
  LEX_STATE_TEXTAREA_END_BEGIN,
  LEX_STATE_TEXTAREA_END,
  LEX_STATE_CDATA,
};

class HtmlTokenPool : public ObjPool<HtmlToken> {
 public:
  HtmlTokenPool() {}
  HtmlToken* GetToken(int begin_pos);
 private:
  DISALLOW_COPY_AND_ASSIGN(HtmlTokenPool);
};

class HtmlLexer {
 public:
  HtmlLexer() : xml_mode_(false),
                state_(LEX_STATE_BEGIN),
                last_token_(NULL),
                last_buff_index_(NULL) {}

  // pls keep char in buff all lower letter
  void ParseToken(const char* buff,
                  const int buff_len,
                  std::vector<HtmlToken*>* tokens);
  BuffIndex* GetBuffIndex(const char* buff, int begin_pos);
  std::string* GetString();
  std::vector<HtmlAttrIndex>* GetAttrVector() {
    return &GetToken(0)->attrs_;  // set token begin pos = 0
  }

  // this function is provided for debug and test
  // do NOT use it in production code.
  const std::string GetStringInBuff(const BuffIndex& buff_index) {
    return std::string(lower_buff_ + buff_index.GetOffset(),
                       buff_index.GetLength());
  }

 private:
  BuffIndex* GetLastBuffIndex();
  HtmlToken* GetToken(int begin_pos);

  void Clear();
  void ToTextBegin();
  void ToTagBegin();
  void ToTextOrTagBegin();
  void ToAttrName();
  void MergeLastTwoTextBlocks();
  void EndTagName();
  void ToAttrValue();
  void EndTagToTextBegin(const HtmlTag& tag);
  void ToNextStateOfEmbedLang(HtmlLexState to_state);
  void ToNextStateOfEmbedLangEndBegin(const HtmlTag& tag,
                                      HtmlLexState orig_state,
                                      HtmlLexState end_state,
                                      const char* end_str,
                                      int end_str_len);
  void ToNextStateOfEmbedLangEnd(const HtmlTag& tag);

  void ToNextStateOfBegin();
  void ToNextStateOfPrologBegin();
  void ToNextStateOfDoctype();
  void ToNextStateOfTagBegin();
  void ToNextStateOfTagName();
  void ToNextStateOfTagEndName();
  void ToNextStateOfTagEndAfterName();
  void ToNextStateOfCloseTagBegin();
  void ToNextStateOfAttrNameBegin();
  void ToNextStateOfAttrName();
  void ToNextStateOfAttrNameEnd();
  void ToNextStateOfAttrValueBegin();
  void ToNextStateOfAttrQuotedValue();
  void ToNextStateOfAttrValue();
  void ToNextStateOfTextBegin();
  void ToNextStateOfTextBlock();
  void ToNextStateOfComment();
  void ToNextStateOfCdata();
  void ToNextStateOfScript();
  void ToNextStateOfScriptEndBegin();
  void ToNextStateOfScriptEnd();
  void ToNextStateOfStyle();
  void ToNextStateOfStyleEndBegin();
  void ToNextStateOfStyleEnd();
  void ToNextStateOfTextarea();
  void ToNextStateOfTextareaEndBegin();
  void ToNextStateOfTextareaEnd();

  inline static bool IsLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  }

  // this function assume there is enough char in buffer before curr_ch
  // please check buffer length by yourself
  inline static bool IsEndWith(const char* curr_ch,
                               const char* postfix,
                               int postfix_len) {
    postfix += postfix_len - 1;
    for (int i = 0; i < postfix_len; ++i) {
      if (tolower(*(curr_ch - i)) != *(postfix - i)) {
        return false;
      }
    }
    return true;
  }

  inline bool IsEndWithSelfClose() {
    return *(curr_ch_ - 1) == '/';
  }

 private:
  bool xml_mode_;
  HtmlLexState state_;
  const char* lower_buff_;
  const char* curr_ch_;
  int char_len_;
  char quot_char_;
  int offset_;
  std::vector<HtmlToken*>* tokens_;
  HtmlToken* last_token_;
  BuffIndex* last_buff_index_;

  HtmlTokenPool token_pool_;

  static char COMMENT_POSTFIX[];
  static char CDATA_POSTFIX[];
  static char SCRIPT_END_STRING[];
  static char STYLE_END_STRING[];
  static char TEXTAREA_END_STRING[];
  static int COMMENT_POSTFIX_LEN;
  static int CDATA_POSTFIX_LEN;
  static int SCRIPT_END_STRING_LEN;
  static int STYLE_END_STRING_LEN;
  static int TEXTAREA_END_STRING_LEN;
  friend class HtmlLexerUnittest;
  DISALLOW_COPY_AND_ASSIGN(HtmlLexer);
};
}  // namespace parse_master
#endif  // UTIL_PARSE_MASTER2_INTERNAL_HTML_LEXER_H_

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include "../public/html_lexer.h"
#include "base/public/hash_tables.h"

using std::pair;
using std::string;
using std::vector;
using base::utfcharsize;

namespace parse_master {

HtmlToken* HtmlTokenPool::GetToken(int begin_pos) {
  HtmlToken* token = Get();
  token->SetBeginPos(begin_pos);
  return token;
}

void HtmlLexer::ParseToken(const char* buff,
                           const int buff_len,
                           vector<HtmlToken*>* tokens) {
  Clear();
  lower_buff_ = buff;
  curr_ch_ = buff;
  tokens_ = tokens;
  offset_ = 0;
  while (offset_ < buff_len) {
//    DCHECK_GE(buff_len, 0) << "buffer length cannot be negative.";
    char_len_ = utfcharsize(curr_ch_);
    switch (state_) {
    case LEX_STATE_BEGIN:
      ToNextStateOfBegin();
      break;
    case LEX_STATE_TAG_BEGIN:
      ToNextStateOfTagBegin();
      break;
    case LEX_STATE_PROLOG_BEGIN:
      ToNextStateOfPrologBegin();
      break;
    case LEX_STATE_TAG_NAME:
      ToNextStateOfTagName();
      break;
    case LEX_STATE_TAG_END_NAME:
      ToNextStateOfTagEndName();
      break;
    case LEX_STATE_TAG_END_AFTER_NAME:
      ToNextStateOfTagEndAfterName();
      break;
    case LEX_STATE_CLOSE_TAG_BEGIN:
      ToNextStateOfCloseTagBegin();
      break;
    case LEX_STATE_TEXT_BEGIN:
      ToNextStateOfTextBegin();
      break;
    case LEX_STATE_TEXT_BLOCK:
      ToNextStateOfTextBlock();
      break;
    case LEX_STATE_COMMENT:
      ToNextStateOfComment();
      break;
    case LEX_STATE_DOCTYPE:
      ToNextStateOfDoctype();
      break;
    case LEX_STATE_ATTR_NAME_BEGIN:
      ToNextStateOfAttrNameBegin();
      break;
    case LEX_STATE_ATTR_NAME:
      ToNextStateOfAttrName();
      break;
    case LEX_STATE_ATTR_NAME_END:
      ToNextStateOfAttrNameEnd();
      break;
    case LEX_STATE_ATTR_VALUE_BEGIN:
      ToNextStateOfAttrValueBegin();
      break;
    case LEX_STATE_ATTR_QUOTED_VALUE:
      ToNextStateOfAttrQuotedValue();
      break;
    case LEX_STATE_ATTR_VALUE:
      ToNextStateOfAttrValue();
      break;
    case LEX_STATE_SCRIPT:
      ToNextStateOfScript();
      break;
    case LEX_STATE_SCRIPT_END_BEGIN:
      ToNextStateOfScriptEndBegin();
      break;
    case LEX_STATE_SCRIPT_END:
      ToNextStateOfScriptEnd();
      break;
    case LEX_STATE_STYLE:
      ToNextStateOfStyle();
      break;
    case LEX_STATE_STYLE_END_BEGIN:
      ToNextStateOfStyleEndBegin();
      break;
    case LEX_STATE_STYLE_END:
      ToNextStateOfStyleEnd();
      break;
    case LEX_STATE_TEXTAREA:
      ToNextStateOfTextarea();
      break;
    case LEX_STATE_TEXTAREA_END_BEGIN:
      ToNextStateOfTextareaEndBegin();
      break;
    case LEX_STATE_TEXTAREA_END:
      ToNextStateOfTextareaEnd();
      break;
    case LEX_STATE_CDATA:
      ToNextStateOfCdata();
      break;
    default:
      CHECK(false) << "got an unknown lexer state";
      break;
    }
    curr_ch_ += char_len_;
    offset_ += char_len_;
  }

  if (last_token_ == NULL) {
    // VLOG(10) << "branch(001)";
    return;
  }

  last_token_->SetEndPos(buff_len);
  BuffIndex* last_buff = last_buff_index_;
  if (last_buff != NULL) {
    switch (state_) {
    case LEX_STATE_TAG_NAME:
    case LEX_STATE_TAG_END_NAME:
    case LEX_STATE_ATTR_NAME:
    case LEX_STATE_ATTR_QUOTED_VALUE:
    case LEX_STATE_ATTR_VALUE:
      last_buff->SetEndPos(buff_len);
      // VLOG(10) << "branch(002)";
      break;
    case LEX_STATE_SCRIPT_END_BEGIN:
      if (IsEndWith(buff + buff_len - 1,
                    SCRIPT_END_STRING, SCRIPT_END_STRING_LEN)) {
        last_buff->SetEndPos(buff_len);
        last_token_->SetTag(&HtmlTag::SCRIPT);
        // VLOG(10) << "branch(003)";
      } else {
        last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
        // VLOG(10) << "branch(008)";
      }
      break;
    case LEX_STATE_STYLE_END_BEGIN:
      if (IsEndWith(buff + buff_len - 1,
                    STYLE_END_STRING, STYLE_END_STRING_LEN)) {
        last_buff->SetEndPos(buff_len);
        last_token_->SetTag(&HtmlTag::STYLE);
      } else {
        last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
      }
      break;
    case LEX_STATE_TEXTAREA_END_BEGIN:
      if (IsEndWith(buff + buff_len - 1,
                    TEXTAREA_END_STRING, TEXTAREA_END_STRING_LEN)) {
        last_buff->SetEndPos(buff_len);
        last_token_->SetTag(&HtmlTag::TEXTAREA);
      } else {
        last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
      }
      break;
    default:
      // VLOG(10) << "branch(004)";
      break;
    }
  }

  if (last_token_->GetType() == TOKEN_TYPE_UNKNOW) {
    last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
    MergeLastTwoTextBlocks();
    tokens->push_back(last_token_);
    // VLOG(10) << "branch(005)";
  } else if (last_token_->GetType() == TOKEN_TYPE_TEXT_BLOCK) {
    MergeLastTwoTextBlocks();
    tokens->push_back(last_token_);
    // VLOG(10) << "branch(006)";
  } else if (last_token_->GetType() == TOKEN_TYPE_OPEN_TAG
      && last_token_->GetTag() == NULL) {
    tokens->push_back(last_token_);
    last_token_->SetTag(HtmlTag::FindTag(
        lower_buff_ + last_token_->tag_name_.GetOffset(),
        last_token_->tag_name_.GetLength()));
    // VLOG(10) << "branch(007)";
  } else if (last_token_->GetType() == TOKEN_TYPE_CLOSE_TAG
      && last_token_->GetTag() == NULL) {
    do {
      if (last_token_->tag_name_.GetOffset() > 0) {
        if (last_token_->tag_name_.GetLength() > 0) {
          tokens->push_back(last_token_);
          last_token_->SetTag(HtmlTag::FindTag(
              lower_buff_ + last_token_->tag_name_.GetOffset(),
              last_token_->tag_name_.GetLength()));
          // VLOG(10) << "branch(010)";
          break;
        }
      }
      last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
      MergeLastTwoTextBlocks();
      tokens->push_back(last_token_);
      // VLOG(10) << "branch(012)";
    } while (false);
  } else {
    tokens->push_back(last_token_);
    // VLOG(10) << "branch(013)";
  }
};

HtmlToken* HtmlLexer::GetToken(int begin_pos) {
  return token_pool_.GetToken(begin_pos);
}

void HtmlLexer::Clear() {
  last_token_ = NULL;
  last_buff_index_ = NULL;
  state_ = LEX_STATE_BEGIN;
  token_pool_.Clear();
}

inline void HtmlLexer::ToTextBegin() {
  last_token_->SetEndPos(offset_ + char_len_);
  state_ = LEX_STATE_TEXT_BEGIN;
  // VLOG(10) << "branch(014)";
}

inline void HtmlLexer::ToTagBegin() {
  last_token_->SetEndPos(offset_);
  tokens_->push_back(last_token_);
  last_token_ = this->GetToken(offset_);
  state_ = LEX_STATE_TAG_BEGIN;
  // VLOG(10) << "branch(015)";
}

inline void HtmlLexer::ToTextOrTagBegin() {
  last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
  MergeLastTwoTextBlocks();
  if (*curr_ch_ == '<') {
    last_token_->SetEndPos(offset_);
    tokens_->push_back(last_token_);
    last_token_ = GetToken(offset_);
    state_ = LEX_STATE_TAG_BEGIN;
    // VLOG(10) << "branch(016)";
  } else {
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(017)";
  }
}

inline void HtmlLexer::ToAttrName() {
  last_token_->attrs_.push_back(HtmlAttrIndex());
  BuffIndex& attr_name = last_token_->attrs_.back().name_;
  attr_name.SetOffset(offset_);
  last_buff_index_ = &attr_name;

  state_ = LEX_STATE_ATTR_NAME;
  // VLOG(10) << "branch(018)";
}

inline void HtmlLexer::EndTagToTextBegin(const HtmlTag& html_tag) {
  last_token_->SetType(TOKEN_TYPE_CLOSE_TAG);
  last_token_->SetTag(&html_tag);
  last_token_->SetEndPos(offset_ + 1);
  state_ = LEX_STATE_TEXT_BEGIN;
  // VLOG(10) << "branch(019)";
}

inline void HtmlLexer::MergeLastTwoTextBlocks() {
  if (tokens_->size() < 2) {
    // VLOG(10) << "branch(020)";
    return;
  }
  HtmlToken* before_last = tokens_->back();
  if (last_token_->GetType() == TOKEN_TYPE_TEXT_BLOCK &&
      (before_last->GetType() == TOKEN_TYPE_TEXT_BLOCK ||
       before_last->GetType() == TOKEN_TYPE_SCRIPT)) {
    tokens_->pop_back();
    before_last->SetEndPos(last_token_->GetEndPos());
    last_token_ = before_last;
    // VLOG(10) << "branch(021)";
  }
  // VLOG(10) << "branch(022)";
}

inline void HtmlLexer::EndTagName() {
  last_token_->tag_name_.SetEndPos(offset_);
  last_token_->SetTag(HtmlTag::FindTag(
      lower_buff_ + last_token_->tag_name_.GetOffset(),
      last_token_->tag_name_.GetLength()));
  // VLOG(10) << "branch(023)";
}

inline void HtmlLexer::ToAttrValue() {
  BuffIndex& attr_value = last_token_->attrs_.back().val_;
  attr_value.SetOffset(offset_);
  last_buff_index_ = &attr_value;
  state_ = LEX_STATE_ATTR_VALUE;
  // VLOG(10) << "branch(024)";
}

inline void HtmlLexer::ToNextStateOfBegin() {
  if (char_len_ > 1) {
    last_token_ = this->GetToken(offset_);
    last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(025)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    // VLOG(10) << "branch(026)";
    return;
  case '<': {
    last_token_ = this->GetToken(offset_);
    state_ = LEX_STATE_TAG_BEGIN;
    // VLOG(10) << "branch(027)";
    return;
  }
  default:
    last_token_ = this->GetToken(offset_);
    last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(028)";
  }
}

inline void HtmlLexer::ToNextStateOfPrologBegin() {
  if (char_len_ > 1) {
    last_token_->SetType(TOKEN_TYPE_DOCTYPE);
    state_ = LEX_STATE_DOCTYPE;
    // VLOG(10) << "branch(029)";
    return;
  }
  switch (*curr_ch_) {
  case '-':
    last_token_->SetType(TOKEN_TYPE_COMMENT);
    state_ = LEX_STATE_COMMENT;
    // VLOG(10) << "branch(030)";
    return;
  case '[':
    last_token_->SetType(TOKEN_TYPE_CDATA);
    state_ = LEX_STATE_CDATA;
    // VLOG(10) << "branch(030)";
    return;
  default:
    last_token_->SetType(TOKEN_TYPE_DOCTYPE);
    state_ = LEX_STATE_DOCTYPE;
    // VLOG(10) << "branch(031)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfDoctype() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(032)";
    return;
  }
  switch (*curr_ch_) {
  case '>':
    // VLOG(10) << "branch(033)";
    last_token_->SetEndPos(offset_ + 1);
    state_ = LEX_STATE_TEXT_BEGIN;
  }
  // VLOG(10) << "branch(034)";
}

inline void HtmlLexer::ToNextStateOfTagBegin() {
  if (char_len_ > 1) {
    last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
    MergeLastTwoTextBlocks();
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(035)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    last_token_->SetType(TOKEN_TYPE_TEXT_BLOCK);
    MergeLastTwoTextBlocks();
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(036)";
    return;
  case '!':
  case '?':
  case '#':
    state_ = LEX_STATE_PROLOG_BEGIN;
    // VLOG(10) << "branch(037)";
    return;
  case '/':
    last_token_->SetType(TOKEN_TYPE_CLOSE_TAG);  // NOTE: close
    state_ = LEX_STATE_CLOSE_TAG_BEGIN;
    // VLOG(10) << "branch(038)";
    return;
  default:
    if (IsLetter(*curr_ch_)) {
      last_token_->SetType(TOKEN_TYPE_OPEN_TAG);  // NOTE: open
      last_token_->tag_name_.SetOffset(offset_);
      last_buff_index_ = &last_token_->tag_name_;
      state_ = LEX_STATE_TAG_NAME;
      // VLOG(10) << "branch(040)";
    } else {
      ToTextOrTagBegin();
      // VLOG(10) << "branch(041)";
    }
    return;
  }
}

inline void HtmlLexer::ToNextStateOfTagName() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(042)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
  case '/': {
    EndTagName();
    state_ = LEX_STATE_ATTR_NAME_BEGIN;
    // VLOG(10) << "branch(043)";
    return;
  }
  case '<': {
    EndTagName();
    ToTagBegin();
    // VLOG(10) << "branch(044)";
    return;
  }
  case '>': {
    EndTagName();
    last_token_->SetEndPos(offset_ + 1);
    state_ = LEX_STATE_TEXT_BEGIN;
    // VLOG(10) << "branch(045)";
    return;
  }
  default:
    // VLOG(10) << "branch(046)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfTagEndName() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(047)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
  case '/': {
    last_token_->tag_name_.SetEndPos(offset_);
    last_token_->SetTag(HtmlTag::FindTag(
        lower_buff_ + last_token_->tag_name_.GetOffset(),
        last_token_->tag_name_.GetLength()));
    state_ = LEX_STATE_TAG_END_AFTER_NAME;
    // VLOG(10) << "branch(048)";
    return;
  }
  case '<': {
    last_token_->tag_name_.SetEndPos(offset_);
    last_token_->SetTag(HtmlTag::FindTag(
        lower_buff_ + last_token_->tag_name_.GetOffset(),
        last_token_->tag_name_.GetLength()));
    ToTagBegin();
    // VLOG(10) << "branch(049)";
    return;
  }
  case '>': {
    last_token_->tag_name_.SetEndPos(offset_);
    last_token_->SetTag(HtmlTag::FindTag(
        lower_buff_ + last_token_->tag_name_.GetOffset(),
        last_token_->tag_name_.GetLength()));
    last_token_->SetEndPos(offset_ + 1);
    state_ = LEX_STATE_TEXT_BEGIN;
    // VLOG(10) << "branch(050)";
    return;
  }
  default:
    // VLOG(10) << "branch(051)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfTagEndAfterName() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(052)";
    return;
  }
  BuffIndex& tag_name = last_token_->tag_name_;
  switch (*curr_ch_) {
  case '\n':
    tag_name.SetEndPos(offset_);
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(053)";
    return;
  case '<':
    tag_name.SetEndPos(offset_);
    ToTagBegin();
    // VLOG(10) << "branch(054)";
    return;
  case '>': {
    tag_name.SetEndPos(offset_);
    last_token_->SetEndPos(offset_ + 1);
    state_ = LEX_STATE_TEXT_BEGIN;
    // VLOG(10) << "branch(055)";
    return;
  }
  default:
    // VLOG(10) << "branch(056)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfCloseTagBegin() {
  if (*curr_ch_ == '<') {
    // throw </ before < away,
    // maybe use as an unknow token is better choice.
    last_token_ = GetToken(offset_);
    state_ = LEX_STATE_TAG_BEGIN;
    // VLOG(10) << "branch(057)";
  } else {
    last_token_->tag_name_.SetOffset(offset_);
    last_buff_index_ = &last_token_->tag_name_;
    state_ = LEX_STATE_TAG_END_NAME;
    // VLOG(10) << "branch(058)";
  }
}

inline void HtmlLexer::ToNextStateOfAttrNameBegin() {
  if (char_len_ > 1) {
    ToAttrName();
    // VLOG(10) << "branch(059)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
  case '/':
    // VLOG(10) << "branch(060)";
    return;
  case '>': {
    if (IsEndWithSelfClose()) {
      // VLOG(10) << "branch(061)";
      last_token_->SetType(TOKEN_TYPE_SELF_CLOSE_TAG);
    }
    ToTextBegin();
    // VLOG(10) << "branch(062)";
    return;
  }
  case '<':
    ToTagBegin();
    // VLOG(10) << "branch(063)";
    return;
  default:
    ToAttrName();
    // VLOG(10) << "branch(064)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfAttrName() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(065)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
  case '/': {
    last_token_->attrs_.back().name_.SetEndPos(offset_);
    state_ = LEX_STATE_ATTR_NAME_END;
    // VLOG(10) << "branch(066)";
    return;
  }
  case '=': {
    last_token_->attrs_.back().name_.SetEndPos(offset_);
    state_ = LEX_STATE_ATTR_VALUE_BEGIN;
    // VLOG(10) << "branch(067)";
    return;
  }
  case '>': {
    last_token_->attrs_.back().name_.SetEndPos(offset_);
    ToTextBegin();
    // VLOG(10) << "branch(069)";
    return;
  }
  case '<':
    last_token_->attrs_.back().name_.SetEndPos(offset_);
    ToTagBegin();
    // VLOG(10) << "branch(070)";
    return;
  default:
    // VLOG(10) << "branch(071)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfAttrNameEnd() {
  if (char_len_ > 1) {
    ToAttrName();
    // VLOG(10) << "branch(072)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    // VLOG(10) << "branch(073)";
    return;
  case '>': {
    ToTextBegin();
    // VLOG(10) << "branch(074)";
    return;
  }
  case '=': {
    state_ = LEX_STATE_ATTR_VALUE_BEGIN;
    // VLOG(10) << "branch(075)";
    return;
  }
  case '<':
    ToTagBegin();
    // VLOG(10) << "branch(076)";
    return;
  default:
    ToAttrName();
    // VLOG(10) << "branch(077)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfAttrValueBegin() {
  if (char_len_ > 1) {
    ToAttrValue();
    // VLOG(10) << "branch(078)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    // VLOG(10) << "branch(079)";
    return;
  case '>': {
    last_token_->SetEndPos(offset_ + char_len_);
    // VLOG(10) << "branch(081)";
    state_ = LEX_STATE_TEXT_BEGIN;
    return;
  }
  case '\'': {
    BuffIndex& attr_value = last_token_->attrs_.back().val_;
    attr_value.SetOffset(offset_ + 1);
    last_buff_index_ = &attr_value;
    quot_char_ = '\'';
    state_ = LEX_STATE_ATTR_QUOTED_VALUE;
    // VLOG(10) << "branch(082)";
    return;
  }
  case '"': {
    BuffIndex& attr_value = last_token_->attrs_.back().val_;
    attr_value.SetOffset(offset_ + 1);
    last_buff_index_ = &attr_value;
    quot_char_ = '"';
    state_ = LEX_STATE_ATTR_QUOTED_VALUE;
    // VLOG(10) << "branch(083)";
    return;
  }
  case '<': {
    ToTagBegin();
    // VLOG(10) << "branch(084)";
    return;
  }
  default: {
    ToAttrValue();
    // VLOG(10) << "branch(085)";
    return;
  }
  }
}

inline void HtmlLexer::ToNextStateOfAttrQuotedValue() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(086)";
    return;
  }
  if (*curr_ch_ == quot_char_) {
    last_token_->attrs_.back().val_.SetEndPos(offset_);
    state_ = LEX_STATE_ATTR_NAME_BEGIN;
    // VLOG(10) << "branch(087)";
    return;
  } else {
    // VLOG(10) << "branch(088)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfAttrValue() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(089)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    last_token_->attrs_.back().val_.SetEndPos(offset_);
    state_ = LEX_STATE_ATTR_NAME_BEGIN;
    // VLOG(10) << "branch(090)";
    return;
  case '>': {
    last_token_->attrs_.back().val_.SetEndPos(offset_);
    ToTextBegin();
    // VLOG(10) << "branch(093)";
    return;
  }
  case '<':
    last_token_->attrs_.back().val_.SetEndPos(offset_);
    ToTagBegin();
    // VLOG(10) << "branch(094)";
    return;
  default:
    // VLOG(10) << "branch(095)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfTextBegin() {
  HtmlToken* token = this->GetToken(offset_);
  if (last_token_->GetType() == TOKEN_TYPE_OPEN_TAG &&
      last_token_->GetTag() == &HtmlTag::SCRIPT) {
    token->SetType(TOKEN_TYPE_SCRIPT);
    state_ = LEX_STATE_SCRIPT;
    // VLOG(10) << "branch(096)";
  } else if (last_token_->GetType() == TOKEN_TYPE_OPEN_TAG &&
             last_token_->GetTag() == &HtmlTag::TEXTAREA) {
    token->SetType(TOKEN_TYPE_TEXT_BLOCK);
    state_ = LEX_STATE_TEXTAREA;
  } else if (last_token_->GetType() == TOKEN_TYPE_OPEN_TAG &&
             last_token_->GetTag() == &HtmlTag::STYLE) {
    token->SetType(TOKEN_TYPE_STYLE);
    state_ = LEX_STATE_STYLE;
  } else if (char_len_ > 1 || *curr_ch_ != '<') {
    token->SetType(TOKEN_TYPE_TEXT_BLOCK);
    state_ = LEX_STATE_TEXT_BLOCK;
    // VLOG(10) << "branch(097)";
  } else {
    state_ = LEX_STATE_TAG_BEGIN;
    // VLOG(10) << "branch(098)";
  }
  tokens_->push_back(last_token_);
  last_token_ = token;
}

inline void HtmlLexer::ToNextStateOfTextBlock() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(099)";
    return;
  }
  switch (*curr_ch_) {
  case '<': {
    ToTagBegin();
    // VLOG(10) << "branch(100)";
    return;
  }
  default:
    // VLOG(10) << "branch(101)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfComment() {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(102)";
    return;
  }
  switch (*curr_ch_) {
  case '>':
    if ((!xml_mode_ && *(curr_ch_ - 1) == '-') ||
        IsEndWith(curr_ch_ - 1, COMMENT_POSTFIX, COMMENT_POSTFIX_LEN)) {
      last_token_->SetEndPos(offset_ + 1);
      state_ = LEX_STATE_TEXT_BEGIN;
      // VLOG(10) << "branch(103)";
      return;
    }
    break;
    // VLOG(10) << "branch(104)";
  }
  // VLOG(10) << "branch(105)";
  return;
}

inline void HtmlLexer::ToNextStateOfCdata() {
  if (char_len_ > 1) {
    return;
  }
  switch (*curr_ch_) {
  case '>':
    if (!xml_mode_ || IsEndWith(curr_ch_ - 1,
                                CDATA_POSTFIX,
                                CDATA_POSTFIX_LEN)) {
      last_token_->SetEndPos(offset_ + 1);
      state_ = LEX_STATE_TEXT_BEGIN;
      return;
    }
  }
  return;
}

inline void HtmlLexer::ToNextStateOfScript() {
  ToNextStateOfEmbedLang(LEX_STATE_SCRIPT_END_BEGIN);
}

inline void HtmlLexer::ToNextStateOfStyle() {
  ToNextStateOfEmbedLang(LEX_STATE_STYLE_END_BEGIN);
}

inline void HtmlLexer::ToNextStateOfTextarea() {
  ToNextStateOfEmbedLang(LEX_STATE_TEXTAREA_END_BEGIN);
}

inline void HtmlLexer::ToNextStateOfEmbedLang(HtmlLexState to_state) {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(106)";
    return;
  }
  switch (*curr_ch_) {
  case '/': {
    if (*(curr_ch_ - 1) == '<') {
      if (last_token_->GetBeginPos() < offset_ - 1) {
        last_token_->SetEndPos(offset_ - 1);
        tokens_->push_back(last_token_);
        last_token_ = this->GetToken(offset_ -1);
      }
      last_token_->SetType(TOKEN_TYPE_CLOSE_TAG);

      last_token_->tag_name_.SetOffset(offset_ + 1);
      last_buff_index_ = &last_token_->tag_name_;

      state_ = to_state;
      // VLOG(10) << "branch(107)";
    }
    // VLOG(10) << "branch(108)";
  }
  }
  // VLOG(10) << "branch(109)";
  return;
}

inline void HtmlLexer::ToNextStateOfScriptEndBegin() {
  ToNextStateOfEmbedLangEndBegin(HtmlTag::SCRIPT,
                                 LEX_STATE_SCRIPT,
                                 LEX_STATE_SCRIPT_END,
                                 SCRIPT_END_STRING,
                                 SCRIPT_END_STRING_LEN);
}

inline void HtmlLexer::ToNextStateOfStyleEndBegin() {
  ToNextStateOfEmbedLangEndBegin(HtmlTag::STYLE,
                                 LEX_STATE_STYLE,
                                 LEX_STATE_STYLE_END,
                                 STYLE_END_STRING,
                                 STYLE_END_STRING_LEN);
}

inline void HtmlLexer::ToNextStateOfTextareaEndBegin() {
  ToNextStateOfEmbedLangEndBegin(HtmlTag::TEXTAREA,
                                 LEX_STATE_TEXTAREA,
                                 LEX_STATE_TEXTAREA_END,
                                 TEXTAREA_END_STRING,
                                 TEXTAREA_END_STRING_LEN);
}

inline void HtmlLexer::ToNextStateOfEmbedLangEndBegin(const HtmlTag& tag,
                                                      HtmlLexState orig_state,
                                                      HtmlLexState end_state,
                                                      const char* end_str,
                                                      int end_str_len) {
  if (char_len_ > 1) {
    state_ = orig_state;
    // VLOG(10) << "branch(110)";
    return;
  }
  switch (*curr_ch_) {
  case ' ':
  case '/':
  case '\r':
  case '\n':
  case '\t': {
    if (IsEndWith(curr_ch_ - 1, end_str, end_str_len)) {
      last_token_->tag_name_.SetEndPos(offset_);
      state_ = end_state;
      // VLOG(10) << "branch(111)";
      return;
    } else {
      last_token_ = tokens_->back();
      tokens_->pop_back();
      state_ = orig_state;
      // VLOG(10) << "branch(112)";
      return;
    }
  }
  case '>': {
    if (IsEndWith(curr_ch_ - 1, end_str, end_str_len)) {
      last_token_->tag_name_.SetEndPos(offset_);
      EndTagToTextBegin(tag);
      // VLOG(10) << "branch(113)";
      return;
    } else {
      last_token_ = tokens_->back();
      tokens_->pop_back();
      state_ = orig_state;
      // VLOG(10) << "branch(114)";
      return;
    }
  }
  default:
    // VLOG(10) << "branch(115)";
    return;
  }
}

inline void HtmlLexer::ToNextStateOfScriptEnd() {
  ToNextStateOfEmbedLangEnd(HtmlTag::SCRIPT);
}

inline void HtmlLexer::ToNextStateOfStyleEnd() {
  ToNextStateOfEmbedLangEnd(HtmlTag::STYLE);
}

inline void HtmlLexer::ToNextStateOfTextareaEnd() {
  ToNextStateOfEmbedLangEnd(HtmlTag::TEXTAREA);
}

inline void HtmlLexer::ToNextStateOfEmbedLangEnd(const HtmlTag& tag) {
  if (char_len_ > 1) {
    // VLOG(10) << "branch(116)";
    return;
  }
  switch (*curr_ch_) {
  case '<': {
    last_token_->SetType(TOKEN_TYPE_CLOSE_TAG);
    last_token_->SetTag(&tag);
    ToTagBegin();
    // VLOG(10) << "branch(117)";
    return;
  }
  case '>': {
    EndTagToTextBegin(tag);
    // VLOG(10) << "branch(118)";
    return;
  }
  default:
    // VLOG(10) << "branch(119)";
    return;
  }
}

char HtmlLexer::COMMENT_POSTFIX[] = "--";
char HtmlLexer::CDATA_POSTFIX[] = "]]";
char HtmlLexer::SCRIPT_END_STRING[] = "</script";
char HtmlLexer::STYLE_END_STRING[] = "</style";
char HtmlLexer::TEXTAREA_END_STRING[] = "</textarea";
int HtmlLexer::COMMENT_POSTFIX_LEN = strlen(HtmlLexer::COMMENT_POSTFIX);
int HtmlLexer::CDATA_POSTFIX_LEN = strlen(HtmlLexer::CDATA_POSTFIX);
int HtmlLexer::SCRIPT_END_STRING_LEN = strlen(HtmlLexer::SCRIPT_END_STRING);
int HtmlLexer::STYLE_END_STRING_LEN = strlen(HtmlLexer::STYLE_END_STRING);
int HtmlLexer::TEXTAREA_END_STRING_LEN = strlen(HtmlLexer::TEXTAREA_END_STRING);
}

// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "../public/utf8_converter.h"

#include <string>

#include "base/public/hash.h"
#include "base/public/string_util.h"
#include "base/public/time.h"
#include "../public/mozilla_encoding_detector.h"
#include "third_part/parser_master/public/parse_master.h"

using parse_master::DomNode;
using parse_master::HtmlTag;
using parse_master::ParseMaster;
using namespace std;   // namespace
using namespace base;   // base


namespace {
const char kRemoveChars[] = {' ', '\n', '\t', '\r', '\f', 0};
const int kMinContentSizeForDetect = 100;
}

namespace util {

Utf8Converter::Utf8Converter()
               : mzencoding_detector_(new i18n::MozillaEncodingDetector),
                 meta_encoding_(i18n::INVALID_ENCODING) {
  iconv_t cd;
  string encoding_str;
  for (int encoding = 0; encoding < i18n::ENCODING_TYPE_COUNT; ++encoding) {
    i18n::EncodingType2String(encoding, &encoding_str);
    StringToLowerASCII(&encoding_str);
    replace(encoding_str.begin(), encoding_str.end(), '_', '-');
    encoding_map_[encoding_str] = static_cast<i18n::Encoding>(encoding);
    if (encoding != 0) {
      // see http://www.cnblogs.com/mydomain/archive/2012/11/18/2776441.html
      // http://baike.baidu.com/link?url=tw6xta2hQJEeoe4zU03qrF9ATam
      // MdqUZ7sAhFgyY9sDDdhJHEqS0C1dYg_y-YXjY7gWMTnVQRdLRh_6fkTWO5K
      cd = iconv_open("utf-8//IGNORE", encoding_str.c_str());
      if (cd == (iconv_t)-1) {
        VLOG(3) << "Can't creat convert descriptor for " << encoding_str;
        continue;
      }
      encoding_cov_map_.insert(make_pair(static_cast<int>(encoding), cd));
    }
  }
  encoding_map_["utf-8"] = i18n::UTF8;

  pm_.RegistParserOnText(
      base::NewPermanentCallback(this, &Utf8Converter::ParseOnText));
  pm_.RegistParserOnTagClose(
      HtmlTag::META, this, &Utf8Converter::ParseMetaClose);
}

Utf8Converter::~Utf8Converter() {
  for (hash_map<int, iconv_t>::iterator it =
       encoding_cov_map_.begin(); it != encoding_cov_map_.end(); ++it) {
    iconv_close(it->second);
  }
}

bool Utf8Converter::ConvertHtmlToUtf8(const string& content, string* out) {
  Reset();
  Process(content);
  return DealWithPageContent(content, out);
}

void Utf8Converter::Process(const string& content) {
  pm_.SetBuff(content.data(), content.length());
  pm_.Parse();
}

bool Utf8Converter::DealWithPageContent(const string& content, string* out) {
  if (!CanProcessWithMetaEncoding(content, out)) {
    return ProcessWithEncodingDetector(content, out);
  }
  return true;
}

bool Utf8Converter::CanProcessWithMetaEncoding(const string& content,
                                               string* out) {
  i18n::Encoding encoding = meta_encoding_;
  if (encoding == i18n::GB2312 || encoding == i18n::GBK) {
    encoding = i18n::GB18030;
  }

  VLOG(3) << "meta encoding: " << i18n::EncodingType2String(encoding);
  if (encoding == i18n::INVALID_ENCODING) {
    return false;
  }

  if (ConvertToUTF8Internal(content, encoding, out)) {
    // The miscellaneous tags in html will interfere the invalid code detector.
    // So detecting invalid code on the text_ which is extracted from
    // the crawldoc's content will make more sense.
    string text;
    RemoveChars(text_, &text);
    string output;
    if (!ConvertToUTF8Internal(text, encoding, &output)) {
      // If converting text_ to utf8 failed, we can only detect on
      // crawldoc's whole content.
      output = *out;
    }
    if (!invalid_code_detector_.IsValidCode(output)) {
      VLOG(2) << "failed with invalid encoding check:" << encoding;
      out->clear();
      return false;
    }
  } else {
    VLOG(2) << "convert failed code detect:"
            << i18n::EncodingType2String(encoding);
    return false;
  }
  return true;
}

bool Utf8Converter::ProcessWithEncodingDetector(const string& content,
                                                string* out) {
  while (!text_.empty() && text_.size() < kMinContentSizeForDetect) {
    text_ += text_;
  }

  i18n::Encoding encoding = mzencoding_detector_->DetectEncoding(text_);
  if (ConvertToUTF8Internal(content, encoding, out)) {
    if (!invalid_code_detector_.IsValidCode(*out)) {
      VLOG(2) << "failed with invalid encoding check:" << encoding;

      out->clear();
      return false;
    }
  } else {
    VLOG(2) << "convert failed code detect:"
            << i18n::EncodingType2String(encoding);
    return false;
  }
  return true;
}

bool Utf8Converter::ConvertToUtf8(const string& content, string* out) {
  string temp = content;
  while (!temp.empty() && temp.size() < kMinContentSizeForDetect) {
    temp += temp;
  }

  i18n::Encoding encoding = mzencoding_detector_->DetectEncoding(temp);
  if (ConvertToUTF8Internal(content, encoding, out)) {
    return true;
  }
  return false;
}

bool Utf8Converter::ConvertToUTF8Internal(const string& content,
                                          i18n::Encoding orig_encoding,
                                          string* out) {
  i18n::Encoding encoding = orig_encoding;
  if (encoding == i18n::ASCII ||
      encoding == i18n::UTF8) {
    VLOG(3) << "not need to convert";
    out->assign(content);
    return true;
  }
  hash_map<int, iconv_t>::iterator it =
      encoding_cov_map_.find(static_cast<int>(encoding));
  if (it == encoding_cov_map_.end()) {
    VLOG(3) << "encoding not support:" << i18n::EncodingType2String(encoding);
    return false;
  }
  iconv_t cd = it->second;
  size_t input_size = content.size();
  char* input_buffer =  new char[input_size];
  CHECK(input_buffer != NULL);
  char* in_buffer = input_buffer;
  memcpy(input_buffer, content.data(), input_size);

  size_t result_buffer_size = input_size * 4;
  char* result_buffer = new char[result_buffer_size];
  CHECK(result_buffer != NULL);
  char* out_buffer = result_buffer;

  CHECK(iconv(cd, NULL, NULL, NULL, NULL) != size_t(-1));

  errno = 0;
  size_t rt = iconv(cd, &in_buffer, &input_size,
                    &out_buffer, &result_buffer_size);
  bool success = false;
  // We ignore invalid bytes.
  if (rt != size_t(-1) || errno == EILSEQ) {
    CHECK_EQ(0, input_size);
    out->assign(result_buffer, out_buffer - result_buffer);
    success = true;
  }

  delete[] input_buffer;
  delete[] result_buffer;
  return success;
}

void Utf8Converter::ParseMetaClose(const DomNode& node) {
  GetMetaEncoding(node);
}

void Utf8Converter::ParseOnText(const DomNode& node) {
  text_.append(node.GetContent());
}

void Utf8Converter::GetMetaEncoding(const DomNode& node) {
  if (meta_encoding_ != i18n::INVALID_ENCODING) {
    // We have detect the meta encoding before, thus return.
    return;
  }

  string content;
  string charset;
  if (node.FetchAttributeToTail("charset", &charset)) {
    StringToLowerASCII(&charset);
    hash_map<string, i18n::Encoding>::iterator it = encoding_map_.find(charset);
    if (it != encoding_map_.end()) {
      meta_encoding_ =  it->second;
      return;
    }
  } else if (node.FetchAttributeToTail("content", &content)) {
    StringToLowerASCII(&content);
    size_t start_pos = content.find("charset");
    start_pos += strlen("charset");
    if (start_pos == string::npos) {
      meta_encoding_ = i18n::INVALID_ENCODING;
      return;
    }

    size_t content_size = content.size();
    size_t charset_start = start_pos;
    while ((charset_start < content_size)  &&
           (content[charset_start] == '\n' ||
            content[charset_start] == '\r' ||
            content[charset_start] == ' '  ||
            content[charset_start] == '\t')) {
      charset_start++;
    }
    if (charset_start == string::npos || content[charset_start] != '=') {
      meta_encoding_ = i18n::INVALID_ENCODING;
      return;
    }
    charset_start += strlen("=");

    while ((charset_start < content_size)  &&
           (content[charset_start] == '\'' ||
            content[charset_start] == '\"' ||
            content[charset_start] == '/'  ||
            content[charset_start] == ' '  ||
            content[charset_start] == '\t')) {
      charset_start++;
    }
    if (charset_start == content_size) {
      meta_encoding_ = i18n::INVALID_ENCODING;
      return;
    }

    size_t charset_end = charset_start;
    while (content[charset_end] != '\'' &&
           content[charset_end] != '"'  &&
           content[charset_end] != '/'  &&
           content[charset_end] != ' '  &&
           content[charset_end] != '\t' &&
           content[charset_end] != '>'  &&
           content[charset_end] != '\n' &&
           charset_end < content_size) {
      charset_end++;
    }

    if (charset_start == charset_end) {
      meta_encoding_ = i18n::INVALID_ENCODING;
      return;
    }

    charset = content.substr(charset_start, charset_end - charset_start);

    hash_map<string, i18n::Encoding>::iterator it = encoding_map_.find(charset);
    if (it != encoding_map_.end()) {
      meta_encoding_ =  it->second;
     return;
    }
    meta_encoding_ = i18n::INVALID_ENCODING;
  } else {
    meta_encoding_ = i18n::INVALID_ENCODING;
  }
}

void Utf8Converter::Reset() {
  meta_encoding_ = i18n::INVALID_ENCODING;
  text_.clear();
}

void Utf8Converter::RemoveChars(const string& text, string* output) {
  const size_t size = text.size();
  for (size_t i = 0; i < size; ++i) {
    if (text[i] != kRemoveChars[0] &&
        text[i] != kRemoveChars[1] &&
        text[i] != kRemoveChars[2] &&
        text[i] != kRemoveChars[3] &&
        text[i] != kRemoveChars[4]) {
      *output += text[i];
    }
  }
}

}  // namespace crawl

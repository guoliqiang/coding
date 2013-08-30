// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#ifndef UTIL_UTF8_CONVERTER_UTF8_CONVERTER_H_
#define UTIL_UTF8_CONVERTER_UTF8_CONVERTER_H_

#include <iconv.h>
#include <string>

#include "base/public/basictypes.h"
#include "base/public/hash_tables.h"
#include "base/public/scoped_ptr.h"
#include "../public/encoding.h"
#include "../public/invalid_code_detector.h"
#include "third_part/parser_master/public/parse_master.h"

namespace i18n {
class MozillaEncodingDetector;
}

namespace util {

class Utf8Converter {
 public:
  Utf8Converter();
  ~Utf8Converter();
  bool ConvertToUtf8(const std::string& content, std::string* out);
  bool ConvertHtmlToUtf8(const std::string& content, std::string* out);

 private:
  void ParseOnText(const parse_master::DomNode& node);
  void ParseMetaClose(const parse_master::DomNode& node);

  void GetMetaEncoding(const parse_master::DomNode& node);
  void Process(const std::string& content);
  void RemoveChars(const std::string& text, std::string* output);
  bool ConvertToUTF8Internal(const std::string& content,
                             i18n::Encoding orig_encoding,
                             std::string* out);
  bool DealWithPageContent(const std::string& content, std::string* out);
  bool CanProcessWithMetaEncoding(const std::string& content, std::string* out);
  bool ProcessWithEncodingDetector(const std::string& content,
                                   std::string* out);
  void Reset();

  scoped_ptr<i18n::MozillaEncodingDetector> mzencoding_detector_;
  base::hash_map<std::string, i18n::Encoding> encoding_map_;
  base::hash_map<int, iconv_t> encoding_cov_map_;

  std::string text_;
  i18n::Encoding meta_encoding_;
  parse_master::ParseMaster pm_;
  i18n::InvalidCodeDetector invalid_code_detector_;

  std::string result_;

  DISALLOW_COPY_AND_ASSIGN(Utf8Converter);
};

}  // namespace crawl

#endif  // UTIL_UTF8_CONVERTER_UTF8_CONVERTER_H_

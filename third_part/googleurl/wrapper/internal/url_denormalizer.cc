// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "../public/url_denormalizer.h"

#include <stdlib.h>

#include <string>
#include <vector>

#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "third_part/punycode/public/punycoder.h"
#include "../public/url.h"

using namespace std;  // NOLINT
using namespace base;  // NOLINT

namespace {

const int kUnicodeMaxLength = 256;

}  // namespace

namespace util {
namespace url_parser {

UrlDenormalizer::UrlDenormalizer() {}

UrlDenormalizer::~UrlDenormalizer() {}

bool UrlDenormalizer::Denormalize(const string& original_url,
                                  string* denormalized_url) {
  string host;
  util::url::GetHost(original_url, &host);

  vector<string> host_parts;
  SplitString(host, '.', &host_parts);

  vector<string> host_parts_denormalized;
  for (size_t i = 0; i < host_parts.size(); i++) {
    string res;
    if (Punydecode(host_parts[i], &res) != true) {
      return false;
    }
    host_parts_denormalized.push_back(res);
  }

  string denormalized_host = JoinString(host_parts_denormalized, '.');

  size_t pos = original_url.find(host);
  CHECK_NE(string::npos, pos);

  denormalized_url->assign(original_url);
  denormalized_url->replace(pos, host.size(), denormalized_host);
  return true;
}

bool UrlDenormalizer::Punydecode(const string& src, string* result) {
  if (!StartsWithASCII(src, "xn--", true)) {
    result->assign(src);
    return true;
  }

  string new_src(src.substr(string("xn--").size()));
  enum punycode_status status;
  unsigned char case_flags[kUnicodeMaxLength];
  unsigned int output_length = kUnicodeMaxLength;
  memset(case_flags, 0, kUnicodeMaxLength);
  punycode_uint output[kUnicodeMaxLength];

  status = punycode_decode(new_src.size(), new_src.c_str(),
                           &output_length,
                           output, case_flags);
  if (status == punycode_bad_input) {
    LOG(ERROR) << "invalid input";
    return false;
  }

  if (status == punycode_big_output) {
    LOG(ERROR) << "input or output is too large, recompile with larger limits";
    return false;
  }

  if (status == punycode_overflow) {
    LOG(ERROR) << "arithmetic overflow";
    return false;
  }
  CHECK_EQ(punycode_success, status);

  result->clear();
  for (int i = 0; i < output_length; ++i) {
    string res;
    UniCodeStrToStr(UintToString(output[i]), &res);
    result->append(res);
  }
  return true;
}

}  // namespace url_parser
}  // namespace util

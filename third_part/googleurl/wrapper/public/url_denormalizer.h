// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#ifndef UTIL_URL_PARSER_URL_DENORMALIZER_H_
#define UTIL_URL_PARSER_URL_DENORMALIZER_H_

#include <string>

#include "base/public/basictypes.h"

namespace util {
namespace url_parser {

class UrlDenormalizer {
 public:
  UrlDenormalizer();
  ~UrlDenormalizer();

  bool Denormalize(const std::string& original_url,
                   std::string* denormalized_url);

 private:
  bool Punydecode(const std::string& src, std::string* output);

  DISALLOW_COPY_AND_ASSIGN(UrlDenormalizer);
};

}  // namespace url_parser
}  // namespace util

#endif  // UTIL_URL_PARSER_URL_DENORMALIZER_H_

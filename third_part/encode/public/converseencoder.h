// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#ifndef UTIL_ENCODE_CONVERSEENCODER_H_
#define UTIL_ENCODE_CONVERSEENCODER_H_

#include <string>
#include "encoderbase.h"

namespace util {

class ConverseEncoder : public EncoderBase {
 public:
  ConverseEncoder() {}
  virtual ~ConverseEncoder() {}

  virtual bool Encode(const std::string& src, std::string* des);
  virtual bool Decode(const std::string& src, std::string* des);
};

}  // namespace util

#endif  // UTIL_ENCODE_CONVERSEENCODER_H_


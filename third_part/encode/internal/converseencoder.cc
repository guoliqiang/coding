// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#include "../public/converseencoder.h"
#include "base/public/hash.h"

namespace util {

REGISTER_ENCODERBASE(ConverseEncoder);

bool ConverseEncoder::Encode(const std::string& src, std::string* des) {
  des->assign(src);
  for (int i = 0; i < des->size(); ++i) {
    (*des)[i] = ~(*des)[i];
  }
  return true;
}

bool ConverseEncoder::Decode(const std::string& src, std::string* des) {
  des->assign(src);
  for (int i = 0; i < des->size(); ++i) {
    (*des)[i] = ~(*des)[i];
  }
  return true;
}

}  // namespace util

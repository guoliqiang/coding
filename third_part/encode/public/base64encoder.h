// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#ifndef UTIL_ENCODE_BASE64ENCODER_H_
#define UTIL_ENCODE_BASE64ENCODER_H_

#include <string>
#include "encoderbase.h"

namespace util {

class Base64Encoder : public EncoderBase {
 public:
  Base64Encoder() {}
  virtual ~Base64Encoder() {}

  virtual bool Encode(const std::string& src, std::string* des);
  virtual bool Decode(const std::string& src, std::string* des);

  // Define the encode key, any string.
  // Default no encode key.
  virtual void SetEncodeKey(const std::string& key);
  virtual void GetEncodeKey(std::string* key) const;

 private:
  std::string caesar_enstr_uc_;
  std::string caesar_enstr_lc_;
  std::string caesar_destr_uc_;
  std::string caesar_destr_lc_;
  std::string key_;

  void CaesarEncrypt(std::string* buf);
  void CaesarDecrypt(std::string* buf);
};

}  // namespace util

#endif  // UTIL_ENCODE_BASE64ENCODER_H_


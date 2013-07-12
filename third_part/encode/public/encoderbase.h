// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#ifndef UTIL_ENCODE_ENCODERBASE_H_
#define UTIL_ENCODE_ENCODERBASE_H_

#include <string>
#include "../../registerer/public/registerer.h"

namespace util {

class EncoderBase {
 public:
  EncoderBase() {}
  virtual ~EncoderBase() {}

  // Define encode function.
  virtual bool Encode(const std::string& src, std::string* des) = 0;

  // Define decode function.
  virtual bool Decode(const std::string& src, std::string* des) = 0;

  // Encode key interface.
  virtual void SetEncodeKey(const std::string& key) {}
  virtual void GetEncodeKey(std::string* key ) const {}

 private:
  DISALLOW_COPY_AND_ASSIGN(EncoderBase);
};
REGISTER_REGISTERER(EncoderBase);
#define REGISTER_ENCODERBASE(name) REGISTER_CLASS(EncoderBase, name)

}  // namespace util

#endif  // UTIL_ENCODE_ENCODERBASE_H_

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#ifndef UTIL_COMPRESSION_COMPRESSION_H_
#define UTIL_COMPRESSION_COMPRESSION_H_

#include <string>

#include "base/public/basictypes.h"

namespace util {

// base class of a compress / uncompress algorithm
class Compression {
 public:
  // Factory method, now support types:
  // "zlib" / "lzo" / "gzip"
  static Compression *GetCompressionAlgorithm(const std::string &type);

  Compression() {}
  virtual ~Compression() {}

  virtual bool Compress(const char *buf, uint64 length, std::string *result) {
    result->assign(buf, length);
    return true;
  }
  virtual bool Uncompress(const char *buf, uint64 length, std::string *result) {
    result->assign(buf, length);
    return true;
  }

  bool Compress(const std::string &input, std::string *output);
  bool Uncompress(const std::string &input, std::string *output);

 private:
  DISALLOW_COPY_AND_ASSIGN(Compression);
};

}  // namespace util

#endif  // UTIL_COMPRESSION_COMPRESSION_H_

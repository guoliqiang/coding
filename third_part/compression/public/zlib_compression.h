// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#ifndef UTIL_COMPRESSION_ZLIB_COMPRESSION_H_
#define UTIL_COMPRESSION_ZLIB_COMPRESSION_H_

#include <string.h>
#include "base/public/scoped_ptr.h"
#include "third_part/compression/public/compression.h"

namespace util {

class ZlibCompression : public Compression {
 public:
  ZlibCompression();

  virtual bool Compress(const char *buf, uint64 length, std::string *result);
  virtual bool Uncompress(const char *buf, uint64 length, std::string *result);

 private:
  void AllocateBuffer(uint64 expected_buffer_size);
  void LogError(int ret);

  static const float kMaxUncompressFactor = 4.0;
  static const float kEnlargeFactor = 1.8;

  scoped_array<char> buffer_;
  uint64 buffer_size_;
  DISALLOW_COPY_AND_ASSIGN(ZlibCompression);
};
}

#endif  // UTIL_COMPRESSION_ZLIB_COMPRESSION_H_

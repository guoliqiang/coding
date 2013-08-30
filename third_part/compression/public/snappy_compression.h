// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#ifndef UTIL_COMPRESSION_SNAPPY_COMPRESSION_H_
#define UTIL_COMPRESSION_SNAPPY_COMPRESSION_H_

#include <string>
#include "base/public/scoped_ptr.h"
#include "third_part/compression/public/compression.h"

namespace util {

class SnappyCompression : public Compression {
 public:
  SnappyCompression();
  ~SnappyCompression() {}
  virtual bool Compress(const char *buf, uint64 length, std::string *result);
  virtual bool Uncompress(const char *buf, uint64 length, std::string *result);

 private:
  static const uint32 COMPRESS_BUF_LEN = 256 * 1024;
  static const uint32 UN_COMPRESS_BUF_LEN = COMPRESS_BUF_LEN -
                                            COMPRESS_BUF_LEN / 6 - 32;
  scoped_array<char> buffer_;
  int32 buffer_size_;

  DISALLOW_COPY_AND_ASSIGN(SnappyCompression);
};

}  // namespace util

#endif  // UTIL_COMPRESSION_SNAPPY_COMPRESSION_H_

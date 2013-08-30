// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (Bin Wang)

#ifndef UTIL_COMPRESSION_LZO_COMPRESSION_H_
#define UTIL_COMPRESSION_LZO_COMPRESSION_H_

#include <string>
#include "base/public/scoped_ptr.h"
#include "third_part/compression/public/compression.h"

namespace util {

class LzoCompression : public Compression {
 public:
  LzoCompression();
  ~LzoCompression() {}
  virtual bool Compress(const char *buf, uint64 length, std::string *result);
  virtual bool Uncompress(const char *buf, uint64 length, std::string *result);

 private:
  static const uint32 IN_LEN = 64*1024;
  static const uint32 OUT_LEN = (IN_LEN + IN_LEN / 16 + 64 + 3);

  scoped_array<unsigned char> buffer_;
  scoped_array<unsigned char> wkrmem_;
  bool init_suc_;

  DISALLOW_COPY_AND_ASSIGN(LzoCompression);
};

}  // namespace util

#endif  // UTIL_COMPRESSION_LZO_COMPRESSION_H_

// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#ifndef UTIL_COMPRESSION_GZIP_COMPRESSION_H_
#define UTIL_COMPRESSION_GZIP_COMPRESSION_H_

#include <string>

#include "base/public/basictypes.h"
#include "third_part/compression/public/compression.h"

namespace util {

// Gzip compresses or decompresses the input to the output.
// Returns true on success. If true, *out will point to a malloc()ed
// buffer, which must be free()d by the caller.
bool GzipCompressData(const char* const in, const size_t in_size,
                      char** out, size_t* out_size);
bool GzipDecompressData(const char* const in, const size_t in_size,
                        const size_t max_out_size, char** out,
                        size_t* out_size);

class GzipCompression : public Compression {
 public:
  GzipCompression() {}
  virtual ~GzipCompression() {}

  virtual bool Compress(const char* buf, uint64 length, std::string* result);
  virtual bool Uncompress(const char* buf, uint64 length, std::string* result);
  bool Uncompress(
      const std::string& input, uint64 max_result_size, std::string *result);

 private:
  DISALLOW_COPY_AND_ASSIGN(GzipCompression);
};

}  // namespace util

#endif  // UTIL_COMPRESSION_GZIP_COMPRESSION_H_

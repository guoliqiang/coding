// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "third_part/compression/public/compression.h"
#include "third_part/compression/public/gzip_compression.h"
#include "third_part/compression/public/lzo_compression.h"
#include "third_part/compression/public/snappy_compression.h"
#include "third_part/compression/public/zlib_compression.h"

using namespace std; // NOLINT
using namespace base;  // NOLINT

namespace util {

Compression *Compression::GetCompressionAlgorithm(const string &type) {
  if (type == "snappy") {
    return new SnappyCompression;
  } else if (type == "zlib") {
    return new ZlibCompression;
  } else if (type == "lzo") {
    return new LzoCompression;
  } else if (type == "gzip") {
    return new GzipCompression;
  }

  return NULL;
}

bool Compression::Compress(const string &input, string *output) {
  return Compress(input.c_str(), input.length(), output);
}

bool Compression::Uncompress(const string &input, string *output) {
  return Uncompress(input.c_str(), input.length(), output);
}

}  // namespace util

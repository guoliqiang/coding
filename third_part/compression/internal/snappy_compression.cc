// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "third_part/compression/public/snappy_compression.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "third_part/snappy/include/snappy.h"

using namespace std; // NOLINT
using namespace base;  // NOLINT

namespace util {

SnappyCompression::SnappyCompression() {
  buffer_.reset(new char[COMPRESS_BUF_LEN]);
  buffer_size_ = COMPRESS_BUF_LEN;
}

bool SnappyCompression::Compress(const char *buf, uint64 length,
                                 string *result) {
  result->clear();
  uint64 remain = length;
  uint64 uncompress_len = UN_COMPRESS_BUF_LEN;
  uint64 compress_len = COMPRESS_BUF_LEN;
  while (remain > 0) {
    if (remain <= uncompress_len) {
      uncompress_len = remain;
    }
    snappy::RawCompress(buf, uncompress_len, buffer_.get(), &compress_len);
    buf += uncompress_len;
    remain -= uncompress_len;
    result->append(IntToBytes(uncompress_len));
    result->append(IntToBytes(compress_len));
    result->append(buffer_.get(), compress_len);
  }
  return true;
}

bool SnappyCompression::Uncompress(const char *buf, uint64 length,
                                   string *result) {
  result->clear();
  const char *buf_end = buf + length;
  bool r = false;
  int32 compress_length = 0;
  int32 uncompress_length = 0;
  while (buf != buf_end) {
    uncompress_length = BytesToInt(&buf);
    compress_length = BytesToInt(&buf);
    if (buffer_size_ < uncompress_length) {
      buffer_.reset(new char[uncompress_length]);
      buffer_size_ = uncompress_length;
    }
    r = snappy::RawUncompress(buf, compress_length, buffer_.get());
    if (!r) {
      VLOG(4) << "internal error - uncompress failed:";
      return false;
    }
    buf += compress_length;
    result->append(buffer_.get(), uncompress_length);
  }
  return true;
}

}  // namespace util

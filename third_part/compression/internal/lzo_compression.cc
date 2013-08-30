// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (Bin Wang)

#include <stdio.h>
#include "third_part/compression/public/lzo_compression.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "third_part/minilzo/public/minilzo.h"

using namespace std; // NOLINT
using namespace base;  // NOLINT

namespace util {

LzoCompression::LzoCompression() {
  buffer_.reset(new unsigned char[OUT_LEN]);
  wkrmem_.reset(new unsigned char[LZO1X_1_MEM_COMPRESS]);
  CHECK_EQ(lzo_init(), LZO_E_OK)
      << "internal error - lzo_init() failed !!!\n"
      << "(this usually indicates a compiler bug - try recompiling "
      << "without optimizations, and enable `-DLZO_DEBUG'"
         " for diagnostics)";
}

bool LzoCompression::Compress(const char *buf, uint64 length, string *result) {
  result->clear();
  int r = -1;
  uint64 remain = length;
  uint64 in_len = IN_LEN;
  uint64 out_len = OUT_LEN;
  while (remain > 0) {
    if (remain <= in_len) {
      in_len = remain;
    }
    r = lzo1x_1_compress(reinterpret_cast<const lzo_bytep>(buf),
                         in_len,
                         reinterpret_cast<lzo_bytep>(buffer_.get()),
                         &out_len,
                         reinterpret_cast<lzo_bytep>(wkrmem_.get()));
    buf += in_len;
    if (r != LZO_E_OK) {
      VLOG(4) << "internal error - compression failed:" << r;
      return false;
    }
    result->append(IntToBytes(in_len));
    result->append(IntToBytes(out_len));
    result->append(reinterpret_cast<const char *>(buffer_.get()), out_len);
    remain -= in_len;
  }
  return true;
}

bool LzoCompression::Uncompress(const char *buf,
                                uint64 length, string *result) {
  result->clear();
  int r = 0;
  uint64 in_len = IN_LEN;
  uint64 out_len = OUT_LEN;
  uint64 compress_length = 0;
  uint64 uncompress_length = 0;
  uint64 in_len_count = 0;
  while (compress_length < length) {
    uncompress_length = BytesToInt(&buf);
    in_len_count = 0;
    while (in_len_count < uncompress_length) {
      out_len = BytesToInt(&buf);
      in_len = IN_LEN;
      r = lzo1x_decompress_safe(reinterpret_cast<const lzo_bytep>(buf),
                                out_len,
                                reinterpret_cast<lzo_bytep>(buffer_.get()),
                                &in_len,
                                NULL);
      if (r != LZO_E_OK) {
        VLOG(4) << "internal error - uncompress failed:" << r;
        return false;
      }
      in_len_count += in_len;
      buf += out_len;
      compress_length += sizeof(int32) * 2;  // add by 2 int32 length, NOLINT
      compress_length += out_len;
      result->append(reinterpret_cast<const char *>(buffer_.get()), in_len);
    }
  }
  return true;
}

}  // namespace util

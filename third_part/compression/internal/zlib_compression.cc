// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include <string>
#include "base/public/logging.h"
#include "third_part/compression/public/zlib_compression.h"
#include "third_part/zlib/include/zlib.h"


using namespace std; // NOLINT
using namespace base;  // NOLINT

namespace util {

ZlibCompression::ZlibCompression() : buffer_size_(0) {}

bool ZlibCompression::Compress(const char *buf, uint64 length, string *result) {
  uLong buffer_size = compressBound(length);
  AllocateBuffer(buffer_size);

  int ret = compress(reinterpret_cast<Bytef*>(buffer_.get()),
                     reinterpret_cast<uLongf*>(&buffer_size),
                     reinterpret_cast<const Bytef*>(buf),
                     length);
  if (Z_OK != ret) {
    LogError(ret);
    return false;
  }

  result->assign(buffer_.get(), buffer_size);
  return true;
}

bool ZlibCompression::Uncompress(const char *buf,
                                 uint64 length, string *result) {
  uLong buffer_size = length * kMaxUncompressFactor;
  AllocateBuffer(buffer_size);

  int ret = 0;
  while (true) {
    ret = uncompress(reinterpret_cast<Bytef*>(buffer_.get()),
                     reinterpret_cast<uLong*>(&buffer_size),
                     reinterpret_cast<const Bytef*>(buf),
                     length);
    if (Z_BUF_ERROR == ret) {
      buffer_size *= kEnlargeFactor;
      AllocateBuffer(buffer_size);
    } else {
      break;
    }
  }

  if (Z_OK != ret) {
    LogError(ret);
    return false;
  }

  result->assign(buffer_.get(), buffer_size);
  return true;
}

void ZlibCompression::AllocateBuffer(uint64 expected_buffer_size) {
  if (expected_buffer_size > buffer_size_) {
    buffer_.reset(new char[expected_buffer_size]);
    buffer_size_ = expected_buffer_size;
  }
}

void ZlibCompression::LogError(int ret) {
  switch (ret) {
    case Z_BUF_ERROR:
      LOG(ERROR) << "ZlibCompression: not enough memory in buffer";
      break;
    case Z_STREAM_ERROR:
      LOG(ERROR) << "invalid compression level";
      break;
    case Z_DATA_ERROR:
      LOG(ERROR) << "invalid or incomplete deflate data";
      break;
    case Z_MEM_ERROR:
      LOG(ERROR) << "out of memory";
      break;
    case Z_VERSION_ERROR:
      LOG(ERROR) << "zlib version mismatch!";
  }
}

}  // namespace util

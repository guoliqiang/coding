// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#include "third_part/compression/public/gzip_compression.h"

#include <stdlib.h>
#include <algorithm>

#include <string>

#include "base/public/logging.h"
#include "third_part/zlib/include/zlib.h"

using namespace std; // NOLINT
using namespace base;  // NOLINT

namespace util {

bool GzipDecompressData(const char* const in, const size_t in_size,
                        const size_t max_out_size, char** out,
                        size_t* out_size) {
  if (in_size == 0) {
    // malloc(0) may legally return NULL, so do malloc(1)
    *out = reinterpret_cast<char*>(malloc(1));
    *out_size = 0;
    return true;
  }
  CHECK(out);
  CHECK(out_size);
  z_stream stream;
  memset(&stream, 0, sizeof(stream));
  CHECK(inflateInit2(&stream, 16 + MAX_WBITS) == Z_OK);

  // guess that output will be roughly double the input size
  *out_size = in_size * 2;
  *out = reinterpret_cast<char*>(malloc(*out_size));
  CHECK(*out);

  // TODO(adlr): ensure that this const_cast is safe.
  stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(in));
  stream.avail_in = in_size;
  stream.next_out = reinterpret_cast<Bytef*>(*out);
  stream.avail_out = *out_size;
  for (;;) {
    int rc = inflate(&stream, Z_FINISH);
    switch (rc) {
      case Z_STREAM_END: {
        *out_size = reinterpret_cast<char*>(stream.next_out) - (*out);
        CHECK(inflateEnd(&stream) == Z_OK);
        return true;
      }
      case Z_OK:  // fall through
      case Z_BUF_ERROR: {
        // allocate more space
        ptrdiff_t out_length =
            reinterpret_cast<char*>(stream.next_out) - (*out);
        if (max_out_size != 0 && *out_size > max_out_size) {
          free(*out);
          CHECK(inflateEnd(&stream) == Z_OK);
          return false;
        }

        *out_size *= 2;
        char* new_out = reinterpret_cast<char*>(realloc(*out, *out_size));
        if (!new_out) {
          free(*out);
          CHECK(inflateEnd(&stream) == Z_OK);
          return false;
        }
        *out = new_out;
        stream.next_out = reinterpret_cast<Bytef*>((*out) + out_length);
        stream.avail_out = (*out_size) - out_length;
        break;
      }
      default:
        LOG(INFO) << "Unknown inflate() return value: " << rc;
        if (stream.msg)
          LOG(INFO) << " message: " << stream.msg;
        free(*out);
        CHECK(inflateEnd(&stream) == Z_OK);
        return false;
    }
  }
}

bool GzipCompressData(const char* const in, const size_t in_size,
                      char** out, size_t* out_size) {
  if (in_size == 0) {
    // malloc(0) may legally return NULL, so do malloc(1)
    *out = reinterpret_cast<char*>(malloc(1));
    *out_size = 0;
    return true;
  }
  CHECK(out);
  CHECK(out_size);
  z_stream stream;
  memset(&stream, 0, sizeof(stream));
  CHECK(deflateInit2(&stream,
                     Z_BEST_COMPRESSION,
                     Z_DEFLATED,
                     16 + MAX_WBITS,
                     9,  // most memory used/best compression
                     Z_DEFAULT_STRATEGY) == Z_OK);

  // guess that output will be roughly half the input size
  *out_size = max(static_cast<size_t>(1), in_size / 2);
  *out = reinterpret_cast<char*>(malloc(*out_size));
  CHECK(*out);

  // TODO(adlr): ensure that this const_cast is safe.
  stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(in));
  stream.avail_in = in_size;
  stream.next_out = reinterpret_cast<Bytef*>(*out);
  stream.avail_out = *out_size;
  for (;;) {
    int rc = deflate(&stream, Z_FINISH);
    switch (rc) {
      case Z_STREAM_END: {
        *out_size = reinterpret_cast<char*>(stream.next_out) - (*out);
        CHECK(deflateEnd(&stream) == Z_OK);
        return true;
      }
      case Z_OK:  // fall through
      case Z_BUF_ERROR: {
        // allocate more space
        ptrdiff_t out_length =
            reinterpret_cast<char*>(stream.next_out) - (*out);
        *out_size *= 2;
        char* new_out = reinterpret_cast<char*>(realloc(*out, *out_size));
        if (!new_out) {
          free(*out);
          CHECK(deflateEnd(&stream) == Z_OK);
          return false;
        }
        *out = new_out;
        stream.next_out = reinterpret_cast<Bytef*>((*out) + out_length);
        stream.avail_out = (*out_size) - out_length;
        break;
      }
      default:
        LOG(INFO) << "Unknown defalate() return value: " << rc;
        if (stream.msg)
          LOG(INFO) << " message: " << stream.msg;
        free(*out);
        CHECK(deflateEnd(&stream) == Z_OK);
        return false;
    }
  }
}

bool GzipCompression::Compress(const char *buf,
                               uint64 length,
                               string *result) {
  char* out_buf;
  size_t out_size;
  if (GzipCompressData(buf, length, &out_buf, &out_size)) {
    result->assign(out_buf, out_size);
    free(out_buf);
    return true;
  }
  return false;
}

bool GzipCompression::Uncompress(const char *buf,
                                 uint64 length,
                                 string *result) {
  char* out_buf;
  size_t out_size;
  if (GzipDecompressData(buf, length, 0, &out_buf, &out_size)) {
    result->assign(out_buf, out_size);
    free(out_buf);
    return true;
  }
  return false;
}

bool GzipCompression::Uncompress(const string& input,
                                 uint64 max_result_size,
                                 std::string *result) {
  char* out_buf;
  size_t out_size;
  if (GzipDecompressData(input.c_str(), input.size(),
      max_result_size, &out_buf, &out_size)) {
    result->assign(out_buf, out_size);
    free(out_buf);
    return true;
  }
  return false;
}
}  // namespace util

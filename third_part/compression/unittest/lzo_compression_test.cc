// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (Bin Wang)

#include <stdlib.h>
#include <stdio.h>
#include "base/yr.h"
#include "base/basictypes.h"
#include "base/string_util.h"
#include "base/logging.h"
#include "util/compression/lzo_compression.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "base/scoped_ptr.h"

namespace util {

TEST(LzoCompression, Compress) {
  const string test_data = "aaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbaaaaaaaaaaaaaa"
        "aaaaaaaaaabbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbb";
  LzoCompression compressor;
  string result;
  EXPECT_TRUE(compressor.Compress(test_data.c_str(),
                                  test_data.length(), &result));
  EXPECT_GT(test_data.length(), result.length());
}

TEST(LzoCompression, CompressAndUncompress) {
  string test_data;
  const int kTestNum = 1000;
  for (int j = 0; j < kTestNum; j++) {
    test_data += rand() % 26 + 'a';
  }
  LzoCompression compressor;
  for (int i = 0; i < kTestNum; ++i) {
    test_data += rand() % 26 + 'a';
    string result;
    EXPECT_TRUE(compressor.Compress(test_data.c_str(),
                                    test_data.length(),
                                    &result));
    string uncompressed;
    EXPECT_TRUE(compressor.Uncompress(result.c_str(),
                                      result.length(),
                                      &uncompressed));
    EXPECT_EQ(test_data, uncompressed);
  }
}

TEST(LzoCompression, SSTableCompressAndUncompress) {
  LzoCompression compressor;
  string compress = string("DATABLK\x2a");
  const int test_size = 1000;
  for (int i = 0; i < test_size; ++i) {
    char subfix = 'a' + i % 26;
    int length = i % 10000;
    string key = StringPrintf("%09d", i);
    for (int j = 0; j < length; ++j)
      key += subfix;
    string value = key + "_value";
    compress +=  IntToBytes(key.length()) + IntToBytes(value.length()) + key
        + value;
    if (compress.length() > 64 * 1024 || i == test_size - 1) {
      string result;
      EXPECT_TRUE(compressor.Compress(compress.c_str(),
                                      compress.length(),
                                      &result));
      string uncompressed;
      EXPECT_TRUE(compressor.Uncompress(result.c_str(),
                                        result.length(),
                                        &uncompressed));
      EXPECT_EQ(compress, uncompressed);
      compress = string("DATABLK\x2a");
    }
  }
}

}  // namespace util

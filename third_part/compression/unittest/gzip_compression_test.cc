// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (quj)

#include "util/compression/gzip_compression.h"

#include <string>

#include "base/flags.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/yr.h"
#include "file/file.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(GzipCompression, CompressAndUncompress) {
  string test_data;
  const int kTestNum = 1000;
  for (int j = 0; j < kTestNum; j++) {
    test_data += rand() % 26 + 'a';
  }
  util::GzipCompression compressor;
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

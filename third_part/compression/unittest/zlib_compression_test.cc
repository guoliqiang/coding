// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include <stdlib.h>  // for rand()
#include <string>

#include "base/yr.h"
#include "base/logging.h"
#include "util/compression/zlib_compression.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace util {

TEST(ZlibCompression, Compress) {
  const string test_data = "aaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbb";
  LOG(INFO) << "compress before: " << test_data;
  ZlibCompression compressor;
  string result;
  EXPECT_TRUE(compressor.Compress(test_data.c_str(), test_data.size(), &result));
  LOG(INFO) << "compressed str: " << result << endl;
  EXPECT_GT(test_data.size(), result.size());
}

TEST(ZlibCompression, CompressAndUncompress) {
  string test_data;
  const int kTestNum = 10000;
  ZlibCompression compressor;
  for (int i = 0; i < kTestNum; ++i) {
    test_data += rand() % 26 + 'a';
    string result;
    EXPECT_TRUE(compressor.Compress(test_data.c_str(),
                                    test_data.size(), &result));
    // EXPECT_GT(test_data.size(), result.size());
    string uncompressed;
    EXPECT_TRUE(compressor.Uncompress(result.c_str(),
                                      result.size(), &uncompressed));
    EXPECT_EQ(test_data, uncompressed);
  }
}

}  // namespace util

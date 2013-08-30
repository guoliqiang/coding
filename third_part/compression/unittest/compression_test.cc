// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (Bin Wang)

#include <stdlib.h>  // for rand()

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/scoped_ptr.h"
#include "base/string_util.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "util/compression/compression.h"

using std::string;

namespace util {

TEST(Compression, LzoCompress) {
  const string test_data = "aaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbb";
  LOG(INFO) << "compress before: " << test_data;
  scoped_ptr<Compression>
    compressor(Compression::GetCompressionAlgorithm("lzo"));
  string result;
  EXPECT_TRUE(compressor->Compress(test_data.c_str(),
      test_data.size(), &result));
  LOG(INFO) << "compressed str: " << result << endl;
  EXPECT_GT(test_data.size(), result.size());
}

TEST(Compression, LzoCompressAndUncompress) {
  string test_data;
  const int kTestNum = 1000;
  scoped_ptr<Compression>
    compressor(Compression::GetCompressionAlgorithm("lzo"));
  for (int i = 0; i < kTestNum; ++i) {
    test_data += rand() % 26 + 'a';
    string result;
    EXPECT_TRUE(compressor->Compress(test_data.c_str(),
                                    test_data.size(), &result));
    string uncompressed;
    EXPECT_TRUE(compressor->Uncompress(result.c_str(),
                                      result.size(), &uncompressed));
    EXPECT_EQ(test_data, uncompressed);
  }
}

TEST(Compression, ZlibCompress) {
  const string test_data = "aaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbb";
  LOG(INFO) << "compress before: " << test_data;
  scoped_ptr<Compression>
      compressor(Compression::GetCompressionAlgorithm("zlib"));
  string result;
  EXPECT_TRUE(compressor->Compress(test_data.c_str(),
      test_data.size(), &result));
  LOG(INFO) << "compressed str: " << result << endl;
  EXPECT_GT(test_data.size(), result.size());
}

TEST(Compression, ZlibCompressAndUncompress) {
  string test_data;
  const int kTestNum = 1000;
  scoped_ptr<Compression>
      compressor(Compression::GetCompressionAlgorithm("zlib"));
  for (int i = 0; i < kTestNum; ++i) {
    test_data += rand() % 26 + 'a';
    string result;
    EXPECT_TRUE(compressor->Compress(test_data.c_str(),
                                    test_data.size(), &result));
    string uncompressed;
    EXPECT_TRUE(compressor->Uncompress(result.c_str(),
                                      result.size(), &uncompressed));
    EXPECT_EQ(test_data, uncompressed);
  }
}

}  // namespace util

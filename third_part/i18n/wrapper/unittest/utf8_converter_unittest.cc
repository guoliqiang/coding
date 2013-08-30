// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include <stdio.h>

#include "base/public/logging.h"
#include "../public/utf8_converter.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "file/public/file.h"

TEST(IconvWrapperTest, simple_test) {
  string file = "./testdata2/input.html";
  string expect_file = "./testdata2/expect.html";
  string content;
  file::File::ReadFileToStringOrDie(file, &content);
  string output;
  util::Utf8Converter converter;
  CHECK(converter.ConvertToUtf8(content, &output));
  string expect_result;
  file::File::ReadFileToStringOrDie(expect_file, &expect_result);
  EXPECT_EQ(expect_result, output);

  output.clear();
  CHECK(converter.ConvertHtmlToUtf8(content, &output));
  EXPECT_EQ(expect_result, output);
}

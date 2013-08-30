// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)

#include <string>

#include "base/logging.h"
#include "file/file.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "util/html/html_unescape_char.h"

namespace util_html {

TEST(HtmlUtilTest, UnescapeSingleEntity) {
  HtmlUtil util;
  std::string s;
  util.Unescape("&#913;", &s);
  EXPECT_EQ("Α", s);

  util.Unescape("&lt;", &s);
  EXPECT_EQ("<", s);

  util.Unescape("&copy;", &s);
  EXPECT_EQ("©", s);
}

TEST(HtmlUtilTest, UnescapeString) {
  HtmlUtil util;
  std::string s;
  util.UnescapeString("&lt;good&gt;something good 我们&amp;", &s);
  EXPECT_EQ("<good>something good 我们&", s);
}

TEST(HtmlUtilTest, FullToHalfWidth) {
  HtmlUtil util;
  std::string s;
  util.FullWidthToHalfWidth(
      "０１２３４５６７８９"
      "ＡＢＣＤＥＦＧＨＩＪＫＬＭＮ"
      "ＯＰＱＲＳＴＵＶＷＸＹＺ"
      "ａｂｃｄｅｆｇｈｉｊｋｌｍｎ"
      "ｏｐｑｒｓｔｕｖｗｘｙｚ",
      &s);
  EXPECT_EQ(
      "0123456789"
      "ABCDEFGHIJKLMN"
      "OPQRSTUVWXYZ"
      "abcdefghijklmn"
      "opqrstuvwxyz", s);
  util.FullWidthToHalfWidth("s我们在ABC!!!,这是个ｑｕｅｓｔｉｏｎ.", &s);
  EXPECT_EQ("s我们在ABC!!!,这是个question.", s);
}

};  // namespace util_html

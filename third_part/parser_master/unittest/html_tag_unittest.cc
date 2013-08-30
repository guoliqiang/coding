// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <map>
#include <set>

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/parser_master/public/html_tag.h"

namespace parse_master {
class HtmlTagUnittest: public testing::Test {
 public:
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

TEST_F(HtmlTagUnittest, InitTest) {
  EXPECT_EQ(HtmlTag::GetTagsCount(), 120);
  string a("a");
  EXPECT_EQ(HtmlTag::FindTag(a), &HtmlTag::A);
  HtmlTag("c");
}

TEST_F(HtmlTagUnittest, InitTest2) {
  HtmlTag("b");
  EXPECT_EQ(HtmlTag::GetTagsCount(), 122);
  EXPECT_FALSE(HtmlTag::A.IsSelfClosed());
  EXPECT_TRUE(HtmlTag::BR.IsSelfClosed());
}
}

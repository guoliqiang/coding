// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <map>
#include <set>

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "third_part/parser_master/public/buff_index.h"

namespace parse_master {
class DomNodeUnittest: public testing::Test {
 public:
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

TEST_F(DomNodeUnittest, TestBuffIndexGetString) {
  string str("AbCDefg");

  BuffIndex bi(0);
  bi.SetEndPos(str.size());
  string tmp_str;
  EXPECT_EQ(string("abcdefg"),
            *bi.ToString(StringToLowerASCII(str).c_str(), &tmp_str));

  str.clear();

  BuffIndex bi2(0);
  bi2.SetEndPos(str.size());
  EXPECT_EQ(str, *bi2.ToString(str.c_str(), &tmp_str));
}
}

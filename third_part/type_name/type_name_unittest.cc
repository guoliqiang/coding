// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)
#include "../testing/gtest/include/gtest/gtest.h"
#include "type_name.h"
#include <string>
#include <vector>
#include "../../base/public/logging.h"

class TypeNameTest : public testing::Test {
};

namespace Type {
class A {};
class B : public A {};
}  // namespace Type

using namespace Type;

TEST_F(TypeNameTest, normalTest) {
  A a;
  LOG(INFO) << GetTypeName(a);
  B b;
  LOG(INFO) << GetTypeName(b);
  a = b;
  LOG(INFO) << GetTypeName(a);

  EXPECT_EQ(GetTypeName(1), "int");
  EXPECT_EQ(GetTypeName(1.0), "double");
  EXPECT_EQ(GetTypeName<int>(), "int");
  EXPECT_EQ(GetTypeName<TypeNameTest>(), "TypeNameTest");
  EXPECT_EQ(GetTypeName<std::string>(), "std::string");
  EXPECT_EQ(GetTypeName<std::vector<std::string> >(), "std::vector<std::string, std::allocator<std::string> >");
}

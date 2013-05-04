// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "../public/registerer.h"
#include "../../testing/gtest/include/gtest/gtest.h"

namespace base {
class Base {
 public:
  virtual int Foo() { return 0; }
  virtual ~Base() { LOG(INFO) << "Destruct Base"; }
};
REGISTER_REGISTERER(Base);
#define REGISTER_BASE(name) REGISTER_CLASS(Base, name)
}  // namespace base

namespace sub1 {
class Sub1 : public base::Base {
 public:
  Sub1() {
    LOG(INFO) << "Sub1";
  }
  virtual ~Sub1() { LOG(INFO) << "Destruct Sub1"; }
  virtual int Foo() { return 1; }
};
REGISTER_BASE(Sub1);
}  // namespace sub1

namespace sub2 {
class Sub2 : public base::Base {
 public:
  Sub2() {
    LOG(INFO) << "Sub2";
  }
  virtual ~Sub2() { LOG(INFO) << "Destruct Sub2"; }
  virtual int Foo() { return 2; }
};
REGISTER_BASE(Sub2);
}  // namespace sub2

namespace registerer {

TEST(RegistererTest, GetInstanceByName) {
  LOG(INFO) << "begin test";
  base::Base *sub = base::BaseRegisterer::GetInstanceByName("Sub1");
  EXPECT_TRUE(sub != NULL);
  EXPECT_EQ(1, sub->Foo());
  delete sub;
  sub = base::BaseRegisterer::GetInstanceByName("Sub2");
  EXPECT_TRUE(sub != NULL);
  EXPECT_EQ(2, sub->Foo());
  delete sub;
  EXPECT_EQ(static_cast<base::Base*>(NULL),
            base::BaseRegisterer::GetInstanceByName("Sub9"));
  EXPECT_EQ(static_cast<base::Base*>(NULL),
            base::BaseRegisterer::GetInstanceByName("Base"));
}

TEST(RegistererTest, GetSingltonInstanceByName) {
  base::Base* sub = base::BaseRegisterer::GetSingletonInstanceByName("Sub1");
  EXPECT_TRUE(sub != NULL);
  EXPECT_EQ(1, sub->Foo());

  base::Base* sub1 = base::BaseRegisterer::GetSingletonInstanceByName("Sub1");
  EXPECT_TRUE(sub1 != NULL);
  EXPECT_EQ(1, sub1->Foo());
  EXPECT_EQ(sub, sub1);

  sub = base::BaseRegisterer::GetSingletonInstanceByName("Sub2");
  EXPECT_EQ(2, sub->Foo());

  EXPECT_EQ(static_cast<base::Base*>(NULL),
            base::BaseRegisterer::GetSingletonInstanceByName("Sub9"));
  EXPECT_EQ(static_cast<base::Base*>(NULL),
            base::BaseRegisterer::GetSingletonInstanceByName("Base"));
}

class Base2 {
 public:
  virtual ~Base2() {}
  virtual int Foo() = 0;
};
REGISTER_REGISTERER(Base2);
#define REGISTER_BASE2(name) REGISTER_CLASS(Base2, name)

class Sub3 : public Base2 {
 public:
  virtual int Foo() { return 1; }
};
REGISTER_BASE2(Sub3);

TEST(RegisterTest, GetUniqInstance) {
  Base2 *base = Base2Registerer::GetUniqInstance();
  CHECK(base != NULL);
  delete base;
}

class Base3 {
 public:
  virtual ~Base3() {}
  virtual int Foo() = 0;
};
REGISTER_REGISTERER(Base3);
#define REGISTER_BASE3(name) REGISTER_CLASS(Base3, name)

class Sub4 : public Base3 {
 public:
  virtual int Foo() { return 4; }
};
REGISTER_BASE3(Sub4);

class Sub5 : public Base3 {
 public:
  virtual int Foo() { return 5; }
};
REGISTER_BASE3(Sub5);

// death test
TEST(RegisterTest, GetUniqInstanceDeath) {
  EXPECT_DEATH(Base3Registerer::GetUniqInstance(), "");
}

}  // namespace registerer

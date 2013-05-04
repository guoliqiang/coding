// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "../public/registerer.h"
using namespace std;
using namespace base;

class Base {
 public:
  virtual int Foo() { return 0; }
  virtual ~Base() { LOG(INFO) << "Destruct Base"; }
};
REGISTER_REGISTERER(Base);
#define REGISTER_BASE(name) REGISTER_CLASS(Base, name)

class Sub1 : public Base {
 public:
  Sub1() {
    LOG(INFO) << "Sub1";
  }
  virtual ~Sub1() { LOG(INFO) << "Destruct Sub1"; }
  virtual int Foo() { return 1; }
};
REGISTER_BASE(Sub1);

class Sub2 : public Base {
 public:
  Sub2() {
    LOG(INFO) << "Sub2";
  }
  virtual ~Sub2() { LOG(INFO) << "Destruct Sub2"; }
  virtual int Foo() { return 2; }
};
REGISTER_BASE(Sub2);


void TEST0() {
  LOG(INFO) << "begin test";
  Base *sub = BaseRegisterer::GetInstanceByName("Sub1");
  CHECK(sub != NULL);
  CHECK_EQ(1, sub->Foo());
  delete sub;
  sub = BaseRegisterer::GetInstanceByName("Sub2");
  CHECK(sub != NULL);
  CHECK_EQ(2, sub->Foo());
  delete sub;
  CHECK_EQ(static_cast<Base*>(NULL),
           BaseRegisterer::GetInstanceByName("Sub9"));
  CHECK_EQ(static_cast<Base*>(NULL),
           BaseRegisterer::GetInstanceByName("Base"));
}

void TEST1() {
  Base* sub = BaseRegisterer::GetSingletonInstanceByName("Sub1");
  CHECK(sub != NULL);
  CHECK_EQ(1, sub->Foo());

  Base* sub1 = BaseRegisterer::GetSingletonInstanceByName("Sub1");
  CHECK(sub1 != NULL);
  CHECK_EQ(1, sub1->Foo());
  CHECK_EQ(sub, sub1);

  sub = BaseRegisterer::GetSingletonInstanceByName("Sub2");
  CHECK_EQ(2, sub->Foo());

  CHECK_EQ(static_cast<Base*>(NULL),
            BaseRegisterer::GetSingletonInstanceByName("Sub9"));
  CHECK_EQ(static_cast<Base*>(NULL),
            BaseRegisterer::GetSingletonInstanceByName("Base"));
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

void TEST2() {
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
void TEST3() {
  Base3Registerer::GetUniqInstance();
}

int main(int argc, char** argv) {
  AtExitManager  exit_manager;
  TEST1();
  TEST2();
  TEST3();
  return 0;
}

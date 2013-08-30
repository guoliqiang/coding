// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include <string>
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/mempool/masf_mempool/public/masf.h"

using namespace util::mempool;  // NOLINT

namespace util {
namespace masf_mempool_test {

class MasfMemUnittest: public testing::Test {
  public:
  protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

struct SimpleType {
  SimpleType(int int1 = 1, int int2 = 2)
      : int1_(int1),
        int2_(int2) {
  }
  explicit SimpleType(int *ptr)
      : ptr_(ptr) {
  }
  explicit SimpleType(const std::string &str)
      : str_(str) {
  }

  explicit SimpleType(int a, int b, const int& c, uint32_t d) {
  }
  int int1_;
  int int2_;

  int *ptr_;
  std::string str_;
};

// define obj pool of SimpleType
REGIST_OBJPOOL(SimpleType, 4, 0.75, 2);

TEST_F(MasfMemUnittest, GlobePoolTest) {
  SimpleType *obj = New<SimpleType>();
  EXPECT_EQ(obj->int1_, 1);
  EXPECT_EQ(obj->int2_, 2);
  Delete(obj);

  obj = New<SimpleType>(11, 22);
  EXPECT_EQ(obj->int1_, 11);
  EXPECT_EQ(obj->int2_, 22);
  int data = 33;
  Delete(obj);

  obj = New<SimpleType>(&data);
  EXPECT_EQ(*obj->ptr_, 33);
  Delete(obj);

  string str("abc");
  obj = New<SimpleType>(str);
  EXPECT_EQ(obj->str_, "abc");
  Delete(obj);

  obj = New<SimpleType>(1, 2, 3, 4);
  Delete(obj);

  Recycle<SimpleType>();
  RecycleAll();
}

TEST_F(MasfMemUnittest, SinglePoolTest) {
  MasfObjPool<SimpleType> pool(4,     // block_size
                               0.75,  // balance_factor
                               2);    // balance_period
  vector<SimpleType*> objs;
  // first period
  // 分配40个obj
  EXPECT_EQ(pool.GetTotalSize(), sizeof(SimpleType) * 4);
  int num_item = 0;
  int block = 0;
  for (; block < 10; ++block) {
    for (int item = 0 ; item < 4; ++item) {
      objs.push_back(pool.New());
      ++num_item;
      EXPECT_EQ(pool.GetTotalSize(), sizeof(SimpleType) * 4 * (block+1));
      EXPECT_EQ(pool.GetUsedSize(), sizeof(SimpleType) * num_item);
    }
  }
  for (int i = 0; i < objs.size(); ++i) {
    pool.Delete(objs[i]);
  }
  objs.clear();
  HERE(INFO);
  pool.Recycle();
  EXPECT_EQ(pool.GetTotalSize(), sizeof(SimpleType) * 4 * (block));
  EXPECT_EQ(pool.GetUsedSize(), 0);
  HERE(INFO);
  pool.Recycle();
  // second period
  // 分配12个obj
  block = 0;
  num_item = 0;
  for (; block < 3; ++block) {
    for (int item = 0 ; item < 4; ++item) {
      objs.push_back(pool.New());
      ++num_item;
      EXPECT_EQ(pool.GetUsedSize(), sizeof(SimpleType) * num_item);
    }
  }
  for (int i = 0; i < objs.size(); ++i) {
    pool.Delete(objs[i]);
  }
  objs.clear();
  HERE(INFO);
  pool.Recycle();
  EXPECT_EQ(pool.GetTotalSize(), sizeof(SimpleType) * 4 * 10);
  EXPECT_EQ(pool.GetUsedSize(), 0);
  HERE(INFO);
  pool.Recycle();
  // 检查balance
  // balance后剩下的块的个数应为3/0.75=4
  EXPECT_EQ(pool.GetTotalSize(), sizeof(SimpleType) * 4 * 4);
  EXPECT_EQ(pool.GetUsedSize(), 0);
}



}  // namespace masf_mempool_test
}  // namespace util

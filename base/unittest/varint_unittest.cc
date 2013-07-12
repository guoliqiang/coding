// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#include "base/varint.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace base {

TEST(GetLength, VarInt32) {
  uint8 data[6] = {0};
  for (int i = 0; i < 31; ++i) {
    uint32 value = (1U << i);
    uint8 *p = WriteVarint32(value, data);
    EXPECT_EQ((p - data), GetVarint32Length(value));
  }
}

TEST(Encoding, VarInt64SimpleFast) {
  uint64 value = 123456789123456789UL;
  uint8 data[10] = {0};
  WriteVarint64Fast(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64Fast(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt64MinFast) {
  uint64 value = 0UL;
  uint8 data[10] = {0};
  WriteVarint64Fast(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64Fast(data, &result));
  EXPECT_EQ(value, result);
}

/*
TEST(Encoding, VarInt64MaxFast) {
  uint64 value = kuint64max;
  uint8 data[10] = {0};
  WriteVarint64Fast(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64Fast(data, &result));
  EXPECT_EQ(value, result);
}
*/

TEST(GetLength, VarInt64) {
  uint8 data[11] = {0};
  for (int i = 0; i < 63; ++i) {
    uint64 value = (1UL << i);
    uint8 *p = WriteVarint64(value, data);
    EXPECT_EQ((p - data), GetVarint64Length(value)) << value;
  }
}

TEST(Encoding, VarInt64Simple) {
  uint64 value = 123456789123456789UL;
  uint8 data[10] = {0};
  WriteVarint64(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt64Min) {
  uint64 value = 0UL;
  uint8 data[10] = {0};
  WriteVarint64(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt64Max) {
  uint64 value = kuint64max;
  uint8 data[10] = {0};
  WriteVarint64(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt32Simple) {
  uint32 value = 123456789;
  uint8 data[10] = {0};
  WriteVarint32(value, data);
  uint32 result = 0;
  EXPECT_TRUE(ReadVarint32(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt32Min) {
  uint32 value = 0;
  uint8 data[10] = {0};
  WriteVarint32(value, data);
  uint32 result = 0;
  EXPECT_TRUE(ReadVarint32(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt32Max) {
  uint32 value = kuint32max;
  uint8 data[10] = {0};
  WriteVarint32(value, data);
  uint32 result = 0;
  EXPECT_TRUE(ReadVarint32(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, RiceInt64Simple) {
  uint64 value = 123489UL;
  uint8 data[10] = {0};
  WriteRiceInt64(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadRiceInt64(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt64In2BytesSimple) {
  uint64 value1 = 1234567891234567UL;
  uint64 value2 = 8888888UL;
  uint8 data[20] = {0};
  uint8* ptr = WriteVarint64In2Bytes(value1, data);
  WriteVarint64In2Bytes(value2, ptr);
  uint64 result = 0UL;
  const uint8* ptr_rt = ReadVarint64In2Bytes(data, &result);
  EXPECT_EQ(value1, result);
  ReadVarint64In2Bytes(ptr_rt, &result);
  EXPECT_EQ(value2, result);
}

TEST(Encoding, VarInt64In2BytesMin) {
  uint64 value = 0UL;
  uint8 data[10] = {0};
  WriteVarint64In2Bytes(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64In2Bytes(data, &result));
  EXPECT_EQ(value, result);
}

TEST(Encoding, VarInt64In2BytesMax) {
  uint64 value = kuint64max;
  uint8 data[10] = {0};
  WriteVarint64In2Bytes(value, data);
  uint64 result = 0UL;
  EXPECT_TRUE(ReadVarint64In2Bytes(data, &result));
  EXPECT_EQ(value, result);
}

}  // namespace base

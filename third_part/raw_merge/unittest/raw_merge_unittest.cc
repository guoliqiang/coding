// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 21:16:48
// File  : raw_merge_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/raw_merge/public/merger.h"

namespace raw_merge {

TEST(RwaMerge, Serialize) {
  Serializer writer("./data", 5 * 1000);
  for (int i = 0; i < 1500; i++) {
    writer.Serialize("Hello World");
  }
}

TEST(RwaMerge, MergeNormal) {
  Merger merger("./data");
  EXPECT_TRUE(merger.Init(2 * 1000));
  std::string record;
  std::string scrach;
  while (merger.Deserialize(&record, &scrach)) {
    merger.Serialize(record);
  }
  Merger merger2("./data");
  EXPECT_FALSE(merger2.Init(2 * 1000));
}

TEST(RwaMerge, DeserializeNormal) {
  Deserializer reader("./data");
  EXPECT_FALSE(reader.HasRawFiles());
  EXPECT_TRUE(reader.HasMergeFiles());
  std::string record;
  std::string scrach;
  while (reader.Deserialize(&record, &scrach)) { }
}

TEST(RwaMerge, DeserializeReadLastRaw) {
  Deserializer reader("./data", true);
  EXPECT_TRUE(reader.HasRawFiles());
  EXPECT_TRUE(reader.HasMergeFiles());
  std::string record;
  std::string scrach;
  while (reader.Deserialize(&record, &scrach)) { }
}

TEST(RwaMerge, MergeReadlastRaw) {
  Merger merger("./data");
  EXPECT_TRUE(merger.Init(2 * 1000, true));
  std::string record;
  std::string scrach;
  while (merger.Deserialize(&record, &scrach)) {
    merger.Serialize(record);
  }
  Merger merger2("./data");
  EXPECT_FALSE(merger2.Init(2 * 1000, true));
}
}  // namespace raw_merge

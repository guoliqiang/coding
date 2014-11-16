// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-16 22:28:39
// File  : level_db_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/leveldb/include/leveldb/db.h"

TEST(LevelDB, Normal) {
  leveldb::DB * db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options,"/tmp/testdb", &db);
  EXPECT_TRUE(status.ok());

  std::string key="key";
  std::string value = "value";
  status = db->Put(leveldb::WriteOptions(), key,value);
  EXPECT_TRUE(status.ok());

  status = db->Get(leveldb::ReadOptions(), key, &value);
  EXPECT_TRUE(status.ok());

  std::string key2 = "key2";
  //move the value under key to key2
  status = db->Put(leveldb::WriteOptions(), key2, value);
  EXPECT_TRUE(status.ok());
  status = db->Delete(leveldb::WriteOptions(), key);
  EXPECT_TRUE(status.ok());
  status = db->Get(leveldb::ReadOptions(),key2, &value);
  EXPECT_TRUE(status.ok());
  status = db->Get(leveldb::ReadOptions(),key, &value);
  EXPECT_FALSE(status.ok());
  delete db;
}

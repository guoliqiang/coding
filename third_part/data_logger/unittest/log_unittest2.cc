// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-12-01 01:09:45
// File  : log_unittest2.cc
// Brief :

#include "third_part/data_logger/public/log_reader.h"
#include "third_part/data_logger/public/log_writer.h"
#include "third_part/fast_file_rw/public/env.h"
#include "third_part/crypto/public/crc32c.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/random.h"

namespace data_logger {

TEST(LogWriteRead, Normal) {
  fast_file_rw::Env * env = fast_file_rw::Env::Default();
  fast_file_rw::WritableFile * write = NULL;
  EXPECT_TRUE(env->NewWritableFile("/tmp/test", &write).ok());
  Writer log_writer(write);
  EXPECT_TRUE(log_writer.AddRecord("foo").ok());
  EXPECT_TRUE(log_writer.AddRecord("bar").ok());

  DefaultReport reporter;
  fast_file_rw::SequentialFile * read = NULL;
  EXPECT_TRUE(env->NewSequentialFile("/tmp/test", &read).ok());
  Reader log_reader(read, &reporter, true, 0);
  std::string record;
  std::string scratch;
  EXPECT_TRUE(log_reader.ReadRecord(&record, &scratch));
  EXPECT_EQ(record, "foo");
  EXPECT_TRUE(log_reader.ReadRecord(&record, &scratch));
  EXPECT_EQ(record, "bar");
}

}  // namespace data_logger

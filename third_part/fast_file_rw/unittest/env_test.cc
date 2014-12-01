// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <string>
#include <vector>
#include "third_part/fast_file_rw/public/env.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "file/public/file.h"

namespace fast_file_rw {

class MemEnvTest : public testing::Test {
 public:
  Env* env_;

  MemEnvTest()
      : env_(Env::Default()) {
  }
  ~MemEnvTest() { }
};

TEST_F(MemEnvTest, Basics) {
  uint64_t file_size;
  WritableFile* writable_file;
  std::vector<std::string> children;

  EXPECT_TRUE(file::File::CreateDir("/tmp/dir", 0755));

  // Check that the directory is empty.
  EXPECT_TRUE(!file::File::Exists("/tmp/dir/non_existent"));
  EXPECT_TRUE(!file::File::FileSize("/tmp/dir/non_existent", &file_size));
  EXPECT_TRUE(file::File::GetFilesInDir("/tmp/dir", &children));
  EXPECT_EQ(0, children.size());

  // Create a file.
  EXPECT_TRUE(env_->NewWritableFile("/tmp/dir/f", &writable_file).ok());
  delete writable_file;

  // Check that the file exists.
  EXPECT_TRUE(file::File::Exists("/tmp/dir/f"));
  EXPECT_TRUE(file::File::FileSize("/tmp/dir/f", &file_size));
  EXPECT_EQ(0, file_size);
  EXPECT_TRUE(file::File::GetFilesInDir("/tmp/dir", &children));
  EXPECT_EQ(1, children.size());
  EXPECT_EQ("/tmp/dir/f", children[0]);

  // Write to the file.
  EXPECT_TRUE(env_->NewWritableFile("/tmp/dir/f", &writable_file).ok());
  EXPECT_TRUE(writable_file->Append("abc").ok());
  delete writable_file;

  // Check for expected size.
  EXPECT_TRUE(file::File::FileSize("/tmp/dir/f", &file_size));
  EXPECT_EQ(3, file_size);

  // Check that opening non-existent file fails.
  SequentialFile* seq_file = NULL;
  RandomAccessFile* rand_file = NULL;
  EXPECT_TRUE(!env_->NewSequentialFile("/tmp/dir/non_existent",
      &seq_file).ok());
  EXPECT_TRUE(seq_file == NULL);
  EXPECT_TRUE(!env_->NewRandomAccessFile("/tmp/dir/non_existent",
      &rand_file).ok());
  EXPECT_TRUE(rand_file == NULL);

  file::File::DeleteRecursively("/tmp/dir");
}


TEST_F(MemEnvTest, ReadWrite) {
  WritableFile* writable_file;
  SequentialFile* seq_file;
  RandomAccessFile* rand_file;
  std::string result;
  char scratch[100];

  EXPECT_TRUE(file::File::CreateDir("/tmp/dir", 0755));

  EXPECT_TRUE(env_->NewWritableFile("/tmp/dir/f", &writable_file).ok());
  EXPECT_TRUE(writable_file->Append("hello ").ok());
  EXPECT_TRUE(writable_file->Append("world").ok());
  delete writable_file;

  // Read sequentially.
  EXPECT_TRUE(env_->NewSequentialFile("/tmp/dir/f", &seq_file).ok());
  EXPECT_TRUE(seq_file->Read(5, &result, scratch).ok());  // Read "hello".
  EXPECT_EQ(0, result.compare("hello"));
  EXPECT_TRUE(seq_file->Skip(1).ok());
  EXPECT_TRUE(seq_file->Read(1000, &result, scratch).ok());  // Read "world".
  EXPECT_EQ(0, result.compare("world"));
  // Try reading past EOF.
  EXPECT_TRUE(seq_file->Read(1000, &result, scratch).ok());
  EXPECT_EQ(0, result.size());
  EXPECT_TRUE(seq_file->Skip(100).ok());  // Try to skip past end of file.
  EXPECT_TRUE(seq_file->Read(1000, &result, scratch).ok());
  EXPECT_EQ(0, result.size());
  delete seq_file;

  // Random reads.
  EXPECT_TRUE(env_->NewRandomAccessFile("/tmp/dir/f", &rand_file).ok());
  EXPECT_TRUE(rand_file->Read(6, 5, &result, scratch).ok());  // Read "world".
  EXPECT_EQ(0, result.compare("world"));
  EXPECT_TRUE(rand_file->Read(0, 5, &result, scratch).ok());  // Read "hello".
  EXPECT_EQ(0, result.compare("hello"));
  EXPECT_TRUE(rand_file->Read(10, 100, &result, scratch).ok());  // Read "d".
  EXPECT_EQ(0, result.compare("d"));

  // Too high offset.
  EXPECT_TRUE(!rand_file->Read(1000, 5, &result, scratch).ok());
  delete rand_file;
  file::File::DeleteRecursively("/tmp/dir");
}

TEST_F(MemEnvTest, LargeWrite) {
  EXPECT_TRUE(file::File::CreateDir("/tmp/dir", 0755));
  const size_t kWriteSize = 300 * 1024;
  char* scratch = new char[kWriteSize * 2];

  std::string write_data;
  for (size_t i = 0; i < kWriteSize; ++i) {
    write_data.append(1, static_cast<char>(i));
  }

  WritableFile* writable_file;
  EXPECT_TRUE(env_->NewWritableFile("/tmp/dir/f", &writable_file).ok());
  EXPECT_TRUE(writable_file->Append("foo").ok());
  EXPECT_TRUE(writable_file->Append(write_data).ok());
  delete writable_file;

  SequentialFile* seq_file;
  std::string result;
  EXPECT_TRUE(env_->NewSequentialFile("/tmp/dir/f", &seq_file).ok());
  EXPECT_TRUE(seq_file->Read(3, &result, scratch).ok());  // Read "foo".
  EXPECT_EQ(0, result.compare("foo"));

  size_t read = 0;
  std::string read_data;
  while (read < kWriteSize) {
    EXPECT_TRUE(seq_file->Read(kWriteSize - read, &result, scratch).ok());
    read_data.append(result.data(), result.size());
    read += result.size();
  }
  EXPECT_TRUE(write_data == read_data);
  delete seq_file;
  delete [] scratch;
  file::File::DeleteRecursively("/tmp/dir");
}

}  // namespace fast_file_rw

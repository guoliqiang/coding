// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-17 23:57:35
// File  : large_file_reader_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/large_file_reader/public/large_file_reader.h"

DEFINE_string(path, "/home/guoliqiang/large_file", "");

using large_file_reader::LargeFileReader;

TEST(LargeFileReaderTest, MMAP) {
  LargeFileReader reader(FLAGS_path);
  int offset = 0;
  int size = 1024 * 1024;
  std::string rs;
  rs.resize(size);
  while(reader.Read(offset, size, &rs)) {
    offset += size;
  }
}

TEST(LargeFileReaderTest, CAPI) {
  FILE * fd = fopen(FLAGS_path.c_str(), "rb");
  size_t size = 0;
  file::File::FileSize(FLAGS_path.c_str(), &size);
  char buff[1024 * 1024];
  while (fread(buff, sizeof(char), sizeof(buff), fd) != 0) {}
  fclose(fd);
}



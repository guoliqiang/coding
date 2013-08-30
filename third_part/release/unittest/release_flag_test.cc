// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#include <vector>

#include "util/release/release_flag.h"
#include "util/release/release_path.h"
#include "base/env.h"
#include "testing/gtest/include/gtest/gtest.h"

using std::string;
using std::vector;

DEFINE_string(release_flag_test_flag,
              "default value",
              "test flag");

DEFINE_string(flagfile1_path,
              "util/release/testdata/test_flagfile",
              "test flag file 1");

namespace release {

TEST(ParseFlags, PrivateFlagFile) {
  vector<string> argv_array;
  argv_array.push_back("release_flag_test");
  argv_array.push_back(
      "--flagfile=" + GetReleaseYrRoot() + FLAGS_flagfile1_path);
  int argc = argv_array.size();
  char **argv = new char*[argc];
  for (int i = 0; i < argc; i ++) {
    argv[i] = const_cast<char*>(argv_array[i].data());
  }
  EXPECT_STREQ(FLAGS_release_flag_test_flag.c_str(), "default value");
  ParseFlags(&argc, &argv);
  EXPECT_STREQ(FLAGS_release_flag_test_flag.c_str(), "from file");
  delete []argv;
}
}  // namespace release

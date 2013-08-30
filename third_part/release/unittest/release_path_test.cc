// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#include "util/release/release_path.h"
#include "base/env.h"
#include "testing/gtest/include/gtest/gtest.h"

using std::string;

namespace release {

TEST(GetDevEnvYrRoot, GetDevEnvYrRoot) {
  string path = GetReleaseYrRoot();
  ASSERT_GT(path.size(), 4);
  string path_suffix = path.substr(path.size() - 4);
  ASSERT_STREQ("/yr/", path_suffix.c_str());
}

TEST(GetDevEnvYrDataPath, GetDevEnvYrDataPath) {
  string path = GetReleaseYrDataPath("a");
  ASSERT_GT(path.size(), 12);
  string path_suffix = path.substr(path.size() - 12);
  ASSERT_STREQ("/yr/yrdata/a", path_suffix.c_str());
}

}  // namespace release

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-06 17:06:10
// File  : notify_unittest.cc
// Brief :


#include <sys/inotify.h>
#include <string>
#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

int Init() {
  return inotify_init();
}

int AddMonitor(const std::string& path, int fd) {
  int wd(-1);
  if (!path.empty()) {
    wd = inotify_add_watch(fd,
                           path.c_str(),
                           IN_CLOSE_WRITE | IN_DELETE | IN_CREATE);
  }
  return wd;
}

class NotifyTest : public testing::Test {
 public:
  NotifyTest() {
    fd_ = Init();
  }
  int fd_;
};

TEST_F(NotifyTest, AddMonitor) {
  int tmp = AddMonitor("./test.txt", fd_);
  EXPECT_EQ(AddMonitor("./test.txt", fd_), tmp);
}

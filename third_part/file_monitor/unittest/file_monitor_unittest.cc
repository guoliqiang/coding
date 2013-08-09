// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)
#include <sys/time.h>
#include <string>
#include "base/public/callback.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "file/public/file_posix.h"
#include "third_part/file_monitor/public/monitor_handler.h"
#include "file/public/file.h"

namespace util {

class YRFileMonitorTest : public testing::Test {
  public:
    void GetValue(std::string) {
    }
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
  static int file_event_count;
  static int dir_event_count;
};

int YRFileMonitorTest::file_event_count = 0;
int YRFileMonitorTest::dir_event_count = 0;

void foo() {
  YRFileMonitorTest::file_event_count++;
}
void bar() {
  YRFileMonitorTest::dir_event_count++;
}

TEST_F(YRFileMonitorTest, normalTest) {
  char content[] = "YRFileMonitorUNITTest";
  std::string filepath1("/tmp/yrunittest/yrdata_monitor_test_file1");
  std::string filepath2("/tmp/yrunittest/yrdata_monitor_test_file2");
  std::string filepath3("/tmp/yrunittest/yrdata_monitor_test_file3");
  std::string filepath4("/tmp/yrunittest/yrdata_monitor_test_file4");
  std::string dirpath("/tmp/yrunittest/");
  std::string pdirpath("/tmp/fm_yrunittest");
  std::string sdirpath("/tmp/fm_yrunittest/sub");

  if (!file::FileBase::Exists(dirpath)) {
    file::FileBase::CreateDir(dirpath);
  }
  if (file::FileBase::Exists(filepath1)) {
    file::FileBase::DeleteRecursively(filepath1);
  }
  if (file::FileBase::Exists(filepath2)) {
    file::FileBase::DeleteRecursively(filepath2);
  }
  if (file::FileBase::Exists(filepath3)) {
    file::FileBase::DeleteRecursively(filepath3);
  }
  if (file::FileBase::Exists(filepath4)) {
    file::FileBase::DeleteRecursively(filepath4);
  }
  HERE(3);
  FileMonitor* monitor = util::FileMonitor::GetInstance();
  bool ret = monitor->Register(filepath1, NULL);
  EXPECT_EQ(ret, false);
  HERE(3);

  base::Closure* callback1 = base::NewPermanentCallback(&foo);
  ret = monitor->Register(filepath1, callback1);
  EXPECT_EQ(ret, false);
  file::FileBase* tmpfile1 =  file::FileBase::Open(filepath1,
                                       file::FileBase::kWrite);
  callback1 = base::NewPermanentCallback(&foo);
  ret = monitor->Register(filepath1, callback1);
  EXPECT_EQ(ret, true);
  HERE(3);

  base::Closure* callback2 = base::NewPermanentCallback(&bar);
  HERE(3);
  ret = monitor->Register(dirpath, callback2);
  EXPECT_EQ(ret, true);
  HERE(3);
  file::FileBase* tmpfile2 = file::FileBase::Open(filepath2,
                                                file::FileBase::kWrite);
  HERE(3);
  sleep(5);
  HERE(3);
  tmpfile1->Write(content, sizeof(content));
  delete tmpfile1;
  tmpfile2->Write(content, sizeof(content));
  delete tmpfile2;
  HERE(3);
  sleep(5);
  HERE(3);
  // tmpfile1 & tmpfil2 writing operations are closer
  // so dir call back will be taken only once;
  EXPECT_EQ(YRFileMonitorTest::file_event_count, 1);
  EXPECT_EQ(YRFileMonitorTest::dir_event_count, 2);
  YRFileMonitorTest::file_event_count = 0;
  YRFileMonitorTest::dir_event_count = 0;
  // tmpfile3 & tmpfile4 creating operations are closer
  // so dir call back will be taken only once;
  file::FileBase* tmpfile3 = file::FileBase::Open(filepath3,
                                                file::FileBase::kWrite);
  file::FileBase* tmpfile4 = file::FileBase::Open(filepath4,
                                                file::FileBase::kWrite);
  // tmpfile1 & tmpfile2 deleting operations are closer
  // so dir call back will be taken only once;
  file::FileBase::DeleteRecursively(filepath1);  // file_event_count++
  file::FileBase::DeleteRecursively(filepath2);  // dir_event_count++
  base::Closure* callback3 = base::NewPermanentCallback(&foo);
  ret = monitor->Register(filepath3, callback3);
  EXPECT_EQ(ret, true);
  base::Closure* callback4 = base::NewPermanentCallback(&foo);
  ret = monitor->Register(filepath4, callback4);
  EXPECT_EQ(ret, true);
  sleep(5);
  // write nothing, trigger an system modify event,
  // but won't be really handled
  tmpfile3->Write(NULL, 0);
  // dir_event_count&file_event_count++
  tmpfile4->Write(content, sizeof(content));
  delete tmpfile3;
  delete tmpfile4;
  sleep(5);
  // tmpfile1 & tmpfile2 deleting &tmpfile1 operations are occurs closely
  // so dir call back will be taken only once;
  // dir_event_count++
  // file_event_count++
  file::FileBase::DeleteRecursively(filepath3);
  // file_event_count++
  file::FileBase::DeleteRecursively(filepath4);
  // revive filepath1
  // file_event_count++
  tmpfile1 = file::FileBase::Open(filepath1,
                                  file::FileBase::kWrite);
  tmpfile1->Write(content, sizeof(content));
  sleep(5);
  // modify will merge into a create event
  // file_event_count++ & dir_event_count++
  tmpfile1->Write(content, sizeof(content));
  tmpfile1->Write(content, sizeof(content));
  delete tmpfile1;
  sleep(5);
  EXPECT_EQ(YRFileMonitorTest::file_event_count, 6);
  EXPECT_EQ(YRFileMonitorTest::dir_event_count, 4);
  if (!file::FileBase::Exists(pdirpath)) {
    file::FileBase::CreateDir(pdirpath);
  }
  if (!file::FileBase::Exists(sdirpath)) {
    file::FileBase::CreateDir(sdirpath);
  }
  base::Closure* callback5 = base::NewPermanentCallback(&foo);
  ret = monitor->Register(pdirpath, callback5);
  EXPECT_EQ(ret, true);
}
}

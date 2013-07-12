// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: jiqinhong@yunrang.com (Hong Jiqin)

#include "util/yrfs/yrfs_manager.h"

#include <string>

#include "base/mutex.h"
#include "base/scoped_ptr.h"
#include "base/shared_ptr.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace util {
DECLARE_bool(use_mock_yrfs_object);
const std::string kTestingServer
  = "10.100.24.104:2181,10.100.24.105:2181,10.100.24.106:2181";
const std::string kTestingDataCenter = "testing";

class YRFSManagerTest: public testing::Test {
 public:
  void SetUp() {
    callback_counter = 0;
  }
  void TearDown() {
  }
  void Callback(YRFSManager *manager, std::string path) {
    base::MutexLock l(&callback_counter_mu);
    callback_counter++;
    manager->GetData(path);
  }

  uint32 callback_counter;
  base::Mutex callback_counter_mu;
};

TEST_F(YRFSManagerTest, CreatePath) {
  static const std::string kPath = "/YRFS/testing/unittest/yrfs";
  static const std::string kPath2 = "/YRFS/testing/unittest/yrfs/path";

    YRFSManager manager(kTestingServer, kTestingDataCenter);
    LOG(INFO) << "init end";
    if (manager.Exists(kPath)) {
      LOG(INFO) << "delete begin";
      manager.Delete(kPath, true);
    }
    LOG(INFO) << kPath;
    EXPECT_TRUE(manager.CreatePath(kPath, "test", false, false));

    LOG(INFO) << kPath2;
    EXPECT_TRUE(manager.CreatePath(kPath2, "test sub", false, true));
    LOG(INFO) << "path9k";
    sleep(5);

    if (manager.Exists(kPath)) {
      manager.Delete(kPath, false);
    }
    LOG(INFO) << "path del";
    sleep(5);
}

TEST_F(YRFSManagerTest, GetDataCallback1) {
  static const std::string kPath = "yrfs_testing";
  static const std::string kValue = "value";

  bool use_mock_object[] = {false};
  for (int i = 0; i < arraysize(use_mock_object); ++i) {
    FLAGS_use_mock_yrfs_object = use_mock_object[i];
    callback_counter = 0;

    YRFSManager manager(kTestingServer, kTestingDataCenter);

    bool exists = false;
    if (!manager.Exists(kPath, &exists)) {
      LOG(WARNING) << "may connection to zookeeper fail, stop test";
      return;
    }
    if (!exists) {
      LOG(INFO) << "create path " << kPath
          << ":" << manager.CreatePath(kPath, "", true, false);
      sleep(1);
    }

    base::shared_ptr<base::Closure> callback(
        base::NewPermanentCallback(
          this,
          &YRFSManagerTest::Callback,
          &manager,
          kPath));
    manager.GetData(kPath, callback);

    manager.SetData(kPath, kValue);
    sleep(1);
    EXPECT_EQ(callback_counter, 1);

    for (int i = 0; i < 10; ++i) {
      manager.GetData(kPath);
    }

    for (int i = 0; i < 5; ++i) {
      manager.SetData(kPath, kValue);
      sleep(1);
      LOG(INFO) << i << " " << callback_counter;
    }
  }
}
}  // namespace util

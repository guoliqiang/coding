// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-07-11 10:19:27
// File  : zk_manager_unittest.cc
// Brief :
#include <vector>
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "../public/zk_manager.h"
#include "base/public/string_util.h"

namespace util {
class ZKManagerTest : public testing::Test {
};

void Print(const std::vector<std::pair<std::string, std::string> > &rs) {
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].first << ":" << rs[i].second;
  }
}

TEST_F(ZKManagerTest, CreatePath) {
  ZKManager manager("127.0.0.1:2181");
  std::vector<std::pair<std::string, std::string> > rs;

  if (manager.Exists("/coding")) manager.Delete("/coding", true);
  CHECK(manager.CreatePath("/coding/zk_manager", "zk_manager"));
  // auto delete and sequence
  CHECK(manager.CreatePath("/coding/zk_manager", "zk_manager", true, false));
  // only auto delete
}

TEST_F(ZKManagerTest, SetData) {
  ZKManager manager("127.0.0.1:2181");
  if (!manager.Exists("/coding/zk_manager")) {
    CHECK(manager.CreatePath("/coding/zk_manager", "zk_manager", true, false));
  }
  CHECK(manager.SetData("/coding/zk_manager", "hello zk"));
  std::string temp;
  CHECK(manager.GetData("/coding/zk_manager", &temp));
  LOG(INFO) << temp;
}

TEST_F(ZKManagerTest, GetChildren) {
  ZKManager manager("127.0.0.1:2181");
  std::vector<std::string> ch;
  CHECK(manager.GetChildren("/", &ch));  // only trigger by created operator
  LOG(INFO) << JoinVector(ch, ':');
  if (!manager.Exists("/coding/zk_manager")) {
    CHECK(manager.CreatePath("/coding/zk_manager", "zk_manager", true, false));
  }
  CHECK(manager.SetData("/", "hello zk"));
  std::string temp;
  CHECK(manager.GetData("/", &temp));
  LOG(INFO) << temp;
}

void CallBack() {
  LOG(INFO) << "call back";
}

TEST_F(ZKManagerTest, CallBack) {
  ZKManager manager("127.0.0.1:2181");
  if (!manager.Exists("/coding/zk_manager")) {
    CHECK(manager.CreatePath("/coding/zk_manager", "zk_manager", true, false));
    base::shared_ptr<base::Closure>
          closure(base::NewPermanentCallback(CallBack));
    manager.RegisterCallBack("/coding/zk_manager", closure);
  }
  CHECK(manager.SetData("/coding/zk_manager", "hello zk"));
}


}  // namespace

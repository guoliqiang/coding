// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-07-04 16:50:51
// File  : zookeeper_unittest.cc
// Brief :
#include <string>
#include <vector>
#include "base/public/logging.h"
#include "third_part/zookeeper/include/zookeeper/zookeeper.h"
#include "third_part/zookeeper/include/zookeeper/zookeeper_log.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

const int ZOO_CLOSED_STATE = 0;
static std::string state2String(int state) {
  if (state == ZOO_CLOSED_STATE) {
    return "ZOO_CLOSED_STATE";
  } else if (state == ZOO_CONNECTING_STATE) {
    return "ZOO_CONNECTING_STATE";
  } else if (state == ZOO_ASSOCIATING_STATE) {
    return "ZOO_ASSOCIATING_STATE";
  } else if (state == ZOO_CONNECTED_STATE) {
    return "ZOO_CONNECTED_STATE";
  } else if (state == ZOO_EXPIRED_SESSION_STATE) {
    return "ZOO_EXPIRED_SESSION_STATE";
  } else if (state == ZOO_AUTH_FAILED_STATE) {
    return "ZOO_AUTH_FAILED_STATE";
  }
  return "INVALID_STATE";
}

static std::string watcherEvent2String(int ev) {
  if (ev == 0) {
    return "ZOO_ERROR_EVENT";
  } else if (ev == ZOO_CREATED_EVENT) {
    return "ZOO_CREATED_EVENT";
  } else if (ev == ZOO_DELETED_EVENT) {
    return "ZOO_DELETED_EVENT";
  } else if (ev == ZOO_CHANGED_EVENT) {
    return "ZOO_CHANGED_EVENT";
  } else if (ev == ZOO_CHILD_EVENT) {
    return "ZOO_CHILD_EVENT";
  } else if (ev == ZOO_SESSION_EVENT) {
    return "ZOO_SESSION_EVENT";
  } else if (ev == ZOO_NOTWATCHING_EVENT) {
    return "ZOO_NOTWATCHING_EVENT";
  }
  return "INVALID_EVENT";
}

static std::string ErrorString(int e) {
  if (e == ZNONODE) {
    return "Node does not exist";
  } else if (e == ZNOAUTH) {
    return "Not authenticated";
  } else if (e == ZBADVERSION) {
    return "Version conflict";
  } else if (e == ZNOCHILDRENFOREPHEMERALS) {
    return "Ephemeral nodes may not have children";
  } else if (e == ZNODEEXISTS) {
    return "The node already exists";
  } else if (e == ZNOTEMPTY) {
    return "The node has children";
  } else if (e == ZSESSIONEXPIRED) {
    return "The session has been expired by the server";
  } else if (e == ZINVALIDCALLBACK) {
    return "Invalid callback specified";
  } else if (e == ZINVALIDACL) {
    return "Invalid ACL specified";
  } else if (e == ZAUTHFAILED) {
    return "Client authentication failed";
  } else if (e == ZCLOSING) {
    return "ZooKeeper is closing";
  } else if (e == ZNOTHING) {
    return "(not error) no server responses to process";
  } else if (e == ZSESSIONMOVED) {
    return "session moved to another server, so operation is ignored";
  } else if (e == ZBADARGUMENTS) {
    return "invalid input parameters";
  } else if (e == ZINVALIDSTATE) {
    return "zhandle state is either ZOO_SESSION_EXPIRED_STATE"
           "or ZOO_AUTH_FAILED_STATE";
  } else if (e == ZMARSHALLINGERROR) {
    return "failed to marshall a request; possibly, out of memory";
  }
  return "";
}
void zktest_watcher(zhandle_t* zh, int type, int state,
                    const char* path, void* watcherCtx) {
  LOG(INFO) << "Something happened.";
  LOG(INFO) << "type:" << watcherEvent2String(type).c_str();
  LOG(INFO) << "state:" << state2String(state).c_str();
  LOG(INFO) << "path:" << path;
  LOG(INFO) << "watcherCtx:" << watcherCtx;
}

class ZkTest: public testing::Test {
 public:
  ZkTest():zkhandle(NULL) {}

  bool InitZk() {
    if (zkhandle != NULL) return true;
    LOG(INFO) << "initial zk";
    const char* host = "127.0.0.1:2181";
    int timeout = 30000;
    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
    zkhandle = zookeeper_init(host,
                                         zktest_watcher,
                                         timeout,
                                         0,
                                         (void *)"hello zookeeper.",  // NOLINT
                                         0);
    if (zkhandle == NULL) return false;
    return true;
  }

  zhandle_t* zkhandle;
};

/*
TEST_F(ZkTest, CreatePath) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  int ret = 0;
  ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                   real_path, 1024);
  if (ret) {
    LOG(ERROR) << "ErrorCode:" << ret << " " << ErrorString(ret);
    CHECK(false);
  }
  LOG(INFO) << "real path:" << real_path;

  ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | ZOO_SEQUENCE,  // auto delete and sequence
                   real_path, 1024);
  if (ret) {
    LOG(ERROR) << "ErrorCode:" << ret << " " << ErrorString(ret);
    CHECK(false);
  }
  LOG(INFO) << "real path:" << real_path;

  ret = zoo_create(zkhandle, "/coding2", "/coding2", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   0 | 0,  // not auto delete , not sequence
                   real_path, 1024);
  if (ret) {
    LOG(ERROR) << "ErrorCode:" << ret << " " << ErrorString(ret);
    CHECK(false);
  }
  LOG(INFO) << "real path:" << real_path;

  ret = zoo_create(zkhandle, "/coding2", "/coding2", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   0 | ZOO_SEQUENCE,  // not auto delete and sequence
                   real_path, 1024);
  if (ret) {
    LOG(ERROR) << "ErrorCode:" << ret << " " << ErrorString(ret);
    CHECK(false);
  }
  LOG(INFO) << "real path:" << real_path;
  sleep(10);
}
*/

/*
TEST_F(ZkTest, Exist) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  int ret = 0;
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/coding", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                   real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  }

  ret = zoo_exists(zkhandle, "/coding", 1, &stat);
  // set watch
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  ret = zoo_set(zkhandle, "/coding", "content", 7, -1);
  // will trigger callback
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  sleep(3);
}
*/
TEST_F(ZkTest, Delete) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  int ret = 0;
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/coding", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                   real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  }

  // ret = zoo_exists(zkhandle, "/coding", 1, &stat);
  // set watch
  // if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  ret = zoo_delete(zkhandle, "/coding", -1);
  // will trigger callback
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  sleep(3);
}

/*
TEST_F(ZkTest, Get) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  int len = sizeof(real_path);
  int ret = 0;
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/coding", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                   real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  }

  ret = zoo_get(zkhandle, "/coding", 1, real_path, &len, &stat);
  // add watch
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  LOG(INFO) << "Get data:" << real_path;
  ret = zoo_set(zkhandle, "/coding", "content", 7, -1);
  // will trigger callback
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);

  ret = zoo_set(zkhandle, "/coding", "content2", 7, -1);
  // will not trigger callback
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  sleep(3);
}

TEST_F(ZkTest, GetChildren) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  int ret = 0;
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/coding", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                   real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  }

  struct String_vector strings;
  ret = zoo_get_children(zkhandle, "/", 1, &strings);
  // add watch
  if (ret != ZOK) {
    LOG(ERROR) << ErrorString(ret);
  } else {
    for (int i = 0; i < strings.count; i++) {
      LOG(INFO) << "children:" << strings.data[i];
    }
  }
  ret = zoo_exists(zkhandle, "/coding33", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding33", "/coding33", 5,
                   &ZOO_OPEN_ACL_UNSAFE,
                   ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                   real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
    // will  trigger callback
  }
  sleep(3);
}


TEST_F(ZkTest, GetAcl) {
  CHECK(InitZk());
  int ret = 0;
  ACL_vector acl;
  struct Stat stat;
  ret = zoo_get_acl(zkhandle, "/", &acl, &stat);
  if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  LOG(INFO) << acl.count;
  LOG(INFO) << acl.data->id.scheme << " "
            << acl.data->id.id << " " << acl.data->perms;
}


TEST_F(ZkTest, SetAcl) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  ACL_vector acl;  // invalid acl
  int ret = 0;
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/coding", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                     &ZOO_OPEN_ACL_UNSAFE,
                     ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                     real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  }
  ret = zoo_set_acl(zkhandle, "/coding", -1, &acl);
  if (ret != ZOK) {
    LOG(ERROR) << ErrorString(ret);
  }
}


TEST_F(ZkTest, AddAuth) {
  CHECK(InitZk());
  // will trigger callback when the connecting is established.
  char real_path[1024];
  ACL_vector acl;  // invalid acl
  int ret = 0;
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/coding", 0, &stat);
  if (ret == ZNONODE) {
    ret = zoo_create(zkhandle, "/coding", "/coding", 5,
                     &ZOO_OPEN_ACL_UNSAFE,
                     ZOO_EPHEMERAL | 0,   // auto delete, not sequence
                     real_path, 1024);
    if (ret != ZOK) LOG(ERROR) << ErrorString(ret);
  }
  ret = zoo_set_acl(zkhandle, "/coding", -1, &acl);
  if (ret != ZOK) {
    LOG(ERROR) << ErrorString(ret);
  }
}
*/

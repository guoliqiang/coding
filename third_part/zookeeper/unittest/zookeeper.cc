// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-07-03 16:29:59
// File  : zookeepre.cc
// Brief :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <vector>
#include "third_part/zookeeper/include/zookeeper/zookeeper.h"
#include "third_part/zookeeper/include/zookeeper/zookeeper_log.h"

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

void zktest_watcher_g(zhandle_t* zh, int type, int state,
                      const char* path, void* watcherCtx) {
  printf("Something happened.\n");
  printf("type: %s\n", watcherEvent2String(type).c_str());
  printf("state: %s\n", state2String(state).c_str());
  printf("path: %s\n", path);
  printf("watcherCtx: %s\n", (char *)watcherCtx);
}

void zktest_dump_stat(const struct Stat *stat) {
  char tctimes[40];
  char tmtimes[40];
  time_t tctime;
  time_t tmtime;

  if (!stat) {
    fprintf(stderr,"null\n");
    return;
  }
  tctime = stat->ctime/1000;
  tmtime = stat->mtime/1000;
  ctime_r(&tmtime, tmtimes);
  ctime_r(&tctime, tctimes);
  fprintf(stderr, "\tctime = %s\tczxid=%lx\n"
          "\tmtime=%s\tmzxid=%lx\n"
          "\tversion=%x\taversion=%x\n"
          "\tephemeralOwner = %lx\n",
          tctimes, stat->czxid,
          tmtimes, stat->mzxid,
          (unsigned int)stat->version, (unsigned int)stat->aversion,
          stat->ephemeralOwner);
}

void zktest_stat_completion(int rc, const struct Stat *stat,
                            const void *data) {
  fprintf(stderr, "%s: rc = %d Stat:\n", (char*)data, rc);
  zktest_dump_stat(stat);
}

void zktest_void_completion(int rc, const void *data) {
  fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
}

void zktest_string_completion(int rc, const char *name,
                              const void *data) {
  fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
  if (!rc) {
    fprintf(stderr, "\tname = %s\n", name);
  }
}

int main(int argc, const char *argv[]) {
  const char* host = "127.0.0.1:2181";
  int timeout = 30000;
  zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
  zhandle_t* zkhandle = zookeeper_init(host,
                                       zktest_watcher_g,
                                       timeout,
                                       0,
                                       (void *)"hello zookeeper.",
                                       0);
  if (zkhandle == NULL) {
    fprintf(stderr, "Error when connecting to zookeeper servers...\n");
    exit(EXIT_FAILURE);
  }

  getchar();
  // struct ACL ALL_ACL[] = {{ZOO_PERM_ALL, ZOO_ANYONE_ID_UNSAFE}};
  // struct ACL_vector ALL_PERMS = {1, ALL_ACL};
  char real_path[1024];
  int ret = zoo_create(zkhandle, "/xy", "hello", 5,
                       &ZOO_OPEN_ACL_UNSAFE, 0,
                       real_path, 1024);
  if (ret) {
    fprintf(stderr, "Error %d for %s\n", ret, "create");
    exit(EXIT_FAILURE);
  }
  printf("realpath:%s\n", real_path);
  ret = 0;

  getchar();
  getchar();
  /*
  struct Stat stat;
  ret = zoo_exists(zkhandle, "/xy", 1, &stat);
  if (ret) {
    fprintf(stderr, "Error %d for %s\n", ret, "exists");
    exit(EXIT_FAILURE);
  }
  */
  ret = 0;
  struct String_vector children;
  ret = zoo_get_children(zkhandle, "/xy", 1, &children);
  if (ret) {
    fprintf(stderr, "Error %d for %s\n", ret, "get children");
  }
  for (int i = 0; i < children.count; i++) {
    printf("child:%s\n", children.data[i]);
  }
  ret = zoo_set(zkhandle, "/xy", "guo", 3, -1);
  if (ret) {
    fprintf(stderr, "Error %d for %s\n", ret, "zoo set");
  }

  ret = zoo_create(zkhandle, "/xy/guo", "hello", 5,
                   &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,
                   real_path, 1024);
  if (ret) {
    fprintf(stderr, "Error %d for %s\n", ret, "create");
    exit(EXIT_FAILURE);
  }
  // Wait for asynchronous zookeeper call done.
  getchar();
  getchar();
  return 0;
  ret = zoo_adelete(zkhandle, "/xyz", -1, zktest_void_completion, "adelete");
  if (ret) {
    fprintf(stderr, "Error %d for %s\n", ret, "adelete");
    exit(EXIT_FAILURE);
  }
  // Wait for asynchronous zookeeper call done.
  getchar();

  zookeeper_close(zkhandle);
}

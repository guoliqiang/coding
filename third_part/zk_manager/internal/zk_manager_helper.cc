// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)
//
#include "../public/zk_manager_helper.h"

#include <unistd.h>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include "base/public/flags.h"
#include "base/public/scoped_ptr.h"
#include "base/public/callback.h"
#include "third_part/zookeeper/include/zookeeper/zookeeper.h"
#include "third_part/crypto/public/sha1.h"
#include "third_part/encode/public/base64encoder.h"

namespace util {

ZKManagerHelper::ZKManagerHelper() {
}

ZKManagerHelper::~ZKManagerHelper() {
  ClearCallbacks();
  ZooClose(zh_);
}

zhandle_t* ZKManagerHelper::ZooInit(const std::string &server,
                                    watcher_fn fn,
                                    void *context) {
  return zookeeper_init(server.c_str(), fn, 10000,
                        0, context, 0);
}

int ZKManagerHelper::ZooClose(zhandle_t *zh) {
  if (zh) {
    return zookeeper_close(zh);
  } else {
    return 0;
  }
}

// real_path may not equal with full_path when sequence = true
int ZKManagerHelper::ZooCreate(zhandle_t *zh,
                               const std::string &full_path,
                               const std::string &value,
                               bool auto_delete,
                               bool sequence,
                               std::string *real_path) {
  static const int kSize = 1024 * 4;
  char buffer[kSize + 1];
  int rc = zoo_create(zh, full_path.c_str(), value.c_str(),
                      value.size(), &ZOO_OPEN_ACL_UNSAFE,
                      (auto_delete ? ZOO_EPHEMERAL : 0) | (sequence ? ZOO_SEQUENCE : 0),  // NOLINT
                      buffer,
                      kSize);
  if (rc == ZOK && real_path) {  // real_path may be NULL
    real_path->assign(buffer);
  }
  return rc;
}

int ZKManagerHelper::ZooDelete(zhandle_t *zh,
                               const std::string &full_path,
                               int version) {
  return zoo_delete(zh, full_path.c_str(), version);
}

int ZKManagerHelper::ZooExists(zhandle_t *zh,
                               const std::string &full_path,
                               bool watch,
                               struct Stat *stat) {
  return zoo_exists(zh, full_path.c_str(),
                    watch ? 1 : 0, stat);
}

int ZKManagerHelper::ZooGetChildren(zhandle_t *zh,
                                    const std::string &full_path,
                                    bool watch,
                                    std::vector<std::string>* children) {
  CHECK(children);
  children->clear();
  struct String_vector strings;
  int rc = zoo_get_children(zh, full_path.c_str(),
                            watch ? 1 : 0, &strings);
  if (rc == ZOK) {
    for (int i = 0; i < strings.count; i++) {
      children->push_back(strings.data[i]);  // children path
    }
    deallocate_String_vector(&strings);
  }
  return rc;
}

int ZKManagerHelper::ZooSet(zhandle_t *zh,
                            const std::string &full_path,
                            const std::string &value,
                            int version) {
  return zoo_set(zh, full_path.c_str(),
                 value.c_str(), value.size(),
                 version);
}

int ZKManagerHelper::ZooGet(zhandle_t *zh,
                            const std::string &full_path,
                            bool watch,
                            std::string *value,
                            struct Stat *stat) {
  CHECK(value);
  Stat tmp_stat;
  static const int32 kSize = 1024 * 1024 * 4;
  scoped_ptr<char> buffer(new char[kSize + 1]);
  int buf_len = kSize;
  int rc = zoo_get(zh, full_path.c_str(),
                   watch ? 1 : 0,
                   buffer.get(), &buf_len,
                   &tmp_stat);
  if (rc == ZOK) {
    uint32 len = std::min(tmp_stat.dataLength, kSize);
    buffer.get()[len] = '\0';
    value->assign(buffer.get(), len);
    if (stat) {
      *stat = tmp_stat;
    }
  }
  return rc;
}

int ZKManagerHelper::ZooAddAuth(zhandle_t *zh,
                                const std::string &scheme,
                                const std::string &cert) {
  return zoo_add_auth(zh, scheme.c_str(), cert.c_str(),
                      cert.size(), NULL, NULL);
}

int ZKManagerHelper::ZooSetAcl(zhandle_t *zh,
                               const std::string &full_path,
                               int version,
                               const struct ACL_vector *acl) {
  return zoo_set_acl(zh, full_path.c_str(), version, acl);
}

int ZKManagerHelper::ZooGetAcl(zhandle_t *zh,
                               const std::string &full_path,
                               struct ACL_vector *acl,  // NOLINT
                               struct Stat *stat) {
  return zoo_get_acl(zh, full_path.c_str(), acl, stat);
}

}  // namespace util

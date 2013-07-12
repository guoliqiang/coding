// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#ifndef MANAGER_HELPER_H_
#define MANAGER_HELPER_H_

#include <string>
#include <vector>
#include <map>
#include <utility>
#include "base/public/flags.h"
#include "base/public/mutex.h"
#include "base/public/time.h"
#include "base/public/string_util.h"
#include "base/public/callback.h"
#include "base/public/shared_ptr.h"
#include "zk_manager_base.h"

namespace util {

class ZKManagerHelper : public ZKManagerBase {
public:
  ZKManagerHelper();

  virtual ~ZKManagerHelper();

protected:
  virtual zhandle_t* ZooInit(const std::string &server,
                             watcher_fn fn,
                             void *context);

  virtual int ZooClose(zhandle_t *zh);

  virtual int ZooCreate(zhandle_t *zh,
                        const std::string &full_path,
                        const std::string &value,
                        bool auto_delete,
                        bool sequence,
                        std::string *real_path);

  virtual int ZooDelete(zhandle_t *zh,
                        const std::string &full_path,
                        int version);

  virtual int ZooExists(zhandle_t *zh,
                        const std::string &full_path,
                        bool watch,
                        struct Stat *stat);

  virtual int ZooGetChildren(zhandle_t *zh,
                             const std::string &full_path,
                             bool watch,
                             std::vector<std::string>* children);

  virtual int ZooSet(zhandle_t *zh,
                     const std::string &full_path,
                     const std::string &value,
                     int version);

  virtual int ZooGet(zhandle_t *zh,
                     const std::string &full_path,
                     bool watch,
                     std::string *value,
                     struct Stat *stat);

  virtual int ZooAddAuth(zhandle_t *zh,
                         const std::string &scheme,
                         const std::string &cert);

  virtual int ZooSetAcl(zhandle_t *zh,
                        const std::string &full_path,
                        int version,
                        const struct ACL_vector *acl);

  virtual int ZooGetAcl(zhandle_t *zh,
                        const std::string &full_path,
                        struct ACL_vector *acl,
                        struct Stat *stat);
};

}  // namespace util

#endif  // MANAGER_HELPER_H_

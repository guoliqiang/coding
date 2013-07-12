// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#ifndef MANAGER_H_
#define MANAGER_H_

#include <sys/ioctl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "base/public/flags.h"
#include "base/public/mutex.h"
#include "base/public/thread_pool.h"
#include "base/public/time.h"
#include "base/public/string_util.h"
#include "base/public/callback.h"
#include "base/public/shared_ptr.h"
#include "base/public/scoped_ptr.h"
#include "third_part/zookeeper/include/zookeeper/zookeeper.h"
#include "third_part/boost/include/boost/shared_ptr.hpp"
#include "zk_manager_base.h"

namespace util {
class ZKManagerBase;

struct ZKHelpObject {
  enum Status { kNone, kReconnecting, kRecoverPath};
  scoped_ptr<ZKManagerBase> manager;
  Status status;
  mutable base::Mutex mu;
};

class ZKManager {
public:
  static std::string GetDefaultServer();

  static std::string GetDefaultDataCenter();

  static std::string GetDefaultUserID();

  static void RunAllCallbacks(boost::shared_ptr<ZKHelpObject>
                              manager_helper);

  static void Callback(const std::string& path,
                       boost::shared_ptr<ZKHelpObject> manager_helper);

  ZKManager();

  ZKManager(const std::string& zk_server);

  ZKManager(const std::string& zk_server,
            const std::string& zk_userid);

  virtual ~ZKManager();

  bool List(const std::string& path,
            std::vector<std::pair<std::string, std::string> >* results);

  bool Delete(const std::string& path,
              bool recursive);

  std::string GetData(const std::string& path,
                      const base::shared_ptr<base::Closure>& closure =
                      base::shared_ptr<base::Closure>(NULL));

  bool GetData(const std::string& path,
               std::string* result,
               const base::shared_ptr<base::Closure>& closure =
               base::shared_ptr<base::Closure>(NULL));

  bool SetData(const std::string& path, const std::string& value);

  bool Exists(const std::string& path, bool* result);

  bool Exists(const std::string& path);

  bool CreatePath(const std::string& path,
                  const std::string& value,
                  bool auto_delete = true,
                  bool sequence = true);

  bool CreateSequencePath(const std::string& path,
                          const std::string& value,
                          bool auto_delete,
                          std::string *real_path);

  void RegisterCallBack(const std::string& path,
                        const base::shared_ptr<base::Closure>& closure);

  bool GetChildren(const std::string& path,
                   std::vector<std::string>* children,
                   bool watch = true);

  void OnZookeeperClosed();

  bool SetAcl(const std::string& path,
              AclType acl_type);

  std::string GetFullPath(const std::string& path);

  bool AddAutoRecoverPath(const std::string& path,
                          const std::string& value,
                          bool auto_delete);

private:
  void Init(const std::string& zk_server,
            const std::string& zk_userid);

  void Init();

  void RecoverConnection();

private:
  bool is_running_;
  base::Mutex mu_is_running_;
  base::CondVar cond_is_running_;
  scoped_ptr<base::ThreadPool> thread_pool_;
  boost::shared_ptr<ZKHelpObject> manager_helper_;
};

}  // namespace util

#endif  // MANAGER_H_

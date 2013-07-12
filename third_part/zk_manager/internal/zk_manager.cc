// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)
//
#include "../public/zk_manager.h"

#include <unistd.h>
#include <string>
#include <utility>
#include <vector>
#include "base/public/flags.h"
#include "base/public/scoped_ptr.h"
#include "base/public/callback.h"
#include "third_part/zookeeper/include/zookeeper/zookeeper.h"
#include "third_part/crypto/public/sha1.h"
#include "third_part/encode/public/base64encoder.h"
#include "third_part/boost/include/boost/weak_ptr.hpp"

#include "../public/zk_manager_helper.h"

namespace util {
DEFINE_string(zk_server, "", "hostname:port[,host2:port2]");
DEFINE_string(zk_userid, "", "username:password");

using std::string;
using std::vector;
using std::pair;
using base::Closure;
using base::shared_ptr;

namespace {

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

void watcher(zhandle_t *zh,
             int type,
             int state,
             const char *path,
             void *watcherCtx) {
  VLOG(3) << "zk watcher, type:" << watcherEvent2String(type)
          << ",stat:" << state2String(zoo_state(zh));
  boost::weak_ptr<ZKHelpObject> *manager =
      static_cast<boost::weak_ptr<ZKHelpObject>*>(watcherCtx);
  boost::shared_ptr<ZKHelpObject> manager_helper = manager->lock();
  if (!manager_helper) {
    return;
  }
  if (type == ZOO_SESSION_EVENT) {
    if (ZOO_EXPIRED_SESSION_STATE == zoo_state(zh)) {
      LOG(WARNING) << "Session expired, reconnect";
      base::MutexLock l(&manager_helper->mu);
      manager_helper->status = ZKHelpObject::kReconnecting;
    } else if (ZOO_CLOSED_STATE == zoo_state(zh)) {
      base::MutexLock l(&manager_helper->mu);
      manager_helper->manager->OnZookeeperClosed();
    } else if (ZOO_CONNECTED_STATE == zoo_state(zh)) {
      {
        base::MutexLock l(&manager_helper->mu);
        manager_helper->status = ZKHelpObject::kRecoverPath;
      }
      ZKManager::RunAllCallbacks(manager_helper);
    }
  } else {
    if (strlen(path) == 0) {
      VLOG(3) << "ZK gets updated, path is empty";
      return;
    }
    VLOG(3) << "ZK " << path << " gets updated";
    string p(path);
    ZKManager::Callback(p, manager_helper);
  }
}
}  // namespace


std::string ZKManager::GetDefaultServer() {
  return FLAGS_zk_server;
}


std::string ZKManager::GetDefaultUserID() {
  return FLAGS_zk_userid;
}


ZKManager::ZKManager() {
  Init(GetDefaultServer(), GetDefaultUserID());
}


ZKManager::ZKManager(const string& zk_server) {
  Init(zk_server, FLAGS_zk_userid);
}


ZKManager::ZKManager(const std::string& zk_server,
                     const std::string& zk_userid) {
  Init(zk_server, zk_userid);
}


void ZKManager::RegisterCallBack(const std::string& path,
                                 const shared_ptr<Closure>& closure) {
  base::MutexLock l(&manager_helper_->mu);
  manager_helper_->manager->RegisterCallBack(path, closure);
}



void ZKManager::OnZookeeperClosed() {
  base::MutexLock l(&manager_helper_->mu);
  manager_helper_->manager->OnZookeeperClosed();
}


void ZKManager::RunAllCallbacks(boost::shared_ptr<ZKHelpObject>
                                manager_helper) {
  std::vector<base::Closure*> callbacks;
  {
    base::MutexLock l(&manager_helper->mu);
    manager_helper->manager->GetAllCallbacks(&callbacks);
  }
  for (int i = 0; i < callbacks.size(); ++i) {
    callbacks[i]->Run();
  }
}

void ZKManager::Init(const string& zk_server,
                     const std::string& zk_userid) {
  LOG(INFO) << "zookeeper server=" << zk_server;
  if (zk_userid.length())
    LOG(INFO)<< "zookeeper userid=" << zk_userid;

  if (!manager_helper_) {
    manager_helper_.reset(new ZKHelpObject);
    manager_helper_->status = ZKHelpObject::kNone;
    manager_helper_->manager.reset(new ZKManagerHelper);
    boost::weak_ptr<ZKHelpObject> *manager =
        new boost::weak_ptr<ZKHelpObject>(manager_helper_);
    manager_helper_->manager->Init(zk_server, zk_userid,
                                   watcher, manager);
  }
  is_running_ = true;
  thread_pool_.reset(new base::ThreadPool(1));
  thread_pool_->ThreadPool::StartWorkers();
  thread_pool_->Add(base::NewOneTimeCallback(
        this,
        &ZKManager::RecoverConnection));
}

ZKManager::~ZKManager() {
  is_running_ = false;
  {
    base::MutexLock l(&mu_is_running_);
    cond_is_running_.SignalAll();
  }
  thread_pool_.reset(NULL);
  VLOG(4) << "destruct ZKManager";
}

bool ZKManager::GetChildren(const string& path,
                            std::vector<std::string>* children,
                            bool watch) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->GetChildren(path, children, watch);
}

void ZKManager::Callback(const std::string& full_path,
                         boost::shared_ptr<ZKHelpObject> manager_helper) {
  std::vector<base::Closure*> callbacks;
  {
    base::MutexLock l(&manager_helper->mu);
    manager_helper->manager->GetCallbacks(full_path, &callbacks);
  }
  for (int i = 0; i < callbacks.size(); ++i) {
    callbacks[i]->Run();
  }
}

bool ZKManager::Delete(const string& path,
                       bool recursive) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->Delete(path, recursive);
}

bool ZKManager::List(const string& path,
                     vector<pair<string, string> >* results) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->List(path, results);
}

bool ZKManager::SetData(const string& path,
                        const string& value) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->SetData(path, value);
}

string ZKManager::GetData(const string& path,
                          const shared_ptr<Closure>& closure /*=NULL*/) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->GetData(path, closure);
}

bool ZKManager::GetData(const string& path,
                        std::string* result,
                        const shared_ptr<Closure>& closure /*=NULL*/) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->GetData(path, result, closure);
}

bool ZKManager::Exists(const string& path) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->Exists(path);
}

bool ZKManager::Exists(const string& path, bool* result) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->Exists(path, result);
}

bool ZKManager::CreatePath(const string& path,
                           const string& value,
                           bool auto_delete,  /*=true*/
                           bool sequence  /*=true*/) {
  base::MutexLock l(&manager_helper_->mu);
  std::string full_path = path;
  return manager_helper_->manager->CreatePath(full_path, value,
                                              auto_delete, sequence);
}

bool ZKManager::CreateSequencePath(const std::string& path,
                                   const std::string& value,
                                   bool auto_delete,
                                   std::string *real_path) {
  CHECK(real_path);
  base::MutexLock l(&manager_helper_->mu);
  std::string full_path = path;
  return manager_helper_->manager->CreateSequencePath(full_path, value,
                                                      auto_delete, real_path);
}

bool ZKManager::SetAcl(const std::string& path,
                       AclType acl_type) {
  base::MutexLock l(&manager_helper_->mu);
  return manager_helper_->manager->SetAcl(path, acl_type);
}

bool ZKManager::AddAutoRecoverPath(const std::string& path,
                                   const std::string& value,
                                   bool auto_delete) {
  base::MutexLock l(&manager_helper_->mu);
  if (manager_helper_->status == ZKHelpObject::kNone) {
    manager_helper_->status = ZKHelpObject::kRecoverPath;
  }
  return manager_helper_->manager->AddAutoRecoverPath(path,
                                   value, auto_delete);
}

void ZKManager::RecoverConnection() {
  static const uint32 kSleepTimeMs = 10 * 1000;
  base::MutexLock l(&mu_is_running_);
  while (is_running_) {
    cond_is_running_.WaitWithTimeout(&mu_is_running_, kSleepTimeMs);
    if (!is_running_) {
      return;
    }
    if (!manager_helper_.get()) {
      continue;
    }
    base::MutexLock l(&manager_helper_->mu);
    if (manager_helper_->status == ZKHelpObject::kReconnecting ||
        manager_helper_->manager->NeedReconnect()) {
      LOG(INFO) << "reconnecting zookeeper...";
      if (manager_helper_->manager->Reconnect()) {
        manager_helper_->status = ZKHelpObject::kRecoverPath;
      }
      LOG(INFO) << "reconnect zookeeper result " << manager_helper_->status;
    }
    if (manager_helper_->status == ZKHelpObject::kRecoverPath) {
      LOG(INFO) << "recovering zookeeper path...";
      if (manager_helper_->manager->RecoverAutoDeletedPathValue()) {
        manager_helper_->status = ZKHelpObject::kNone;
      }
      LOG(INFO) << "recovery zookeeper path result " << manager_helper_->status;
    }
  }
}

}  // namespace util

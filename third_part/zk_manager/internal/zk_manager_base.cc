// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)
//
#include "../public/zk_manager_base.h"
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
#include "third_part/openssl/include/openssl/sha.h"

namespace util {

using std::string;
using std::vector;
using std::pair;
using base::Closure;
using base::shared_ptr;

const std::string ZKManagerBase::digest_scheme = "digest";
const std::string ZKManagerBase::default_scheme = "world";
const std::string ZKManagerBase::default_id = "anyone";

static std::string zooRc2String(int rc) {
  if (rc == ZOK) {
    return "ZOK";
  } else if (rc == ZNONODE) {
    return "ZNONODE";
  } else if (rc == ZNOAUTH) {
    return "does not have permission";
  } else if (rc == ZINVALIDACL) {
    return "invalid ACL specified";
  } else if (rc == ZBADVERSION) {
    return "ZBADVERSION";
  } else if (rc == ZBADARGUMENTS) {
    return "ZBADARGUMENTS";
  } else if (rc == ZINVALIDSTATE) {
    return "ZINVALIDSTATE";
  } else if (rc == ZMARSHALLINGERROR) {
    return "ZMARSHALLINGERROR";
  }
  return StringPrintf("%d", rc);
}

ZKManagerBase::ZKManagerBase() {
}

void ZKManagerBase::RegisterCallBack(const std::string& full_path,
                                     const shared_ptr<Closure>& closure) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  VLOG(4) << "RegisterCallBack:" << full_path;

  std::vector<base::shared_ptr<base::Closure> > &callbacks =
      callbacks_[full_path];
  bool find = false;
  for (int i = 0; i < callbacks.size(); ++i) {
    if (callbacks[i].get() == closure.get()) {
      find = true;
    }
  }
  if (!find) {
    callbacks.push_back(closure);
  }
}

bool ZKManagerBase::NeedReconnect() {
  return zh_ == NULL;
}

bool ZKManagerBase::Reconnect() {
  if (zh_) {
    ZooClose(zh_);
    zh_ = NULL;
  }
  Init();
  if (zh_) {
    VLOG(0) << "Reconnect success";
  } else {
    LOG(ERROR) << "Reconnect ZK failed, try latter...";
  }
  unrecovered_paths_.assign(auto_recover_paths_.begin(),
                            auto_recover_paths_.end());
  return zh_ != NULL;
}

bool ZKManagerBase::RecoverAutoDeletedPathValue() {
  if (!zh_) {
    LOG(ERROR) << "zh is NULL";
    return false;
  }

  std::vector<AutoRecoverPathInfo>::iterator
      it_path_value = unrecovered_paths_.begin();
  while (it_path_value != unrecovered_paths_.end()) {
    const std::string &path = it_path_value->path;
    const std::string &value = it_path_value->value;
    bool exists = false;
    bool check_status = false;
    check_status = Exists(path, &exists);
    // When recreate the service, the registered node may still exist when
    // regisering. So we could wait several seconds here
    if (!check_status) {
      LOG(WARNING) << "check existence failed, retry later : " << path;
      it_path_value++;
      continue;
    }
    bool success = false;
    if (it_path_value->is_auto_deleted) {
      if (exists) {
        LOG(INFO) << "path exists, retry later : " << path;
        it_path_value++;
        continue;
      }
      VLOG(0) << "Begin recover path : " << path;
      success = CreatePath(path,
                           value,
                           it_path_value->is_auto_deleted,
                           false,
                           false);
    } else {
      VLOG(0) << "Begin recover path : " << path;
      if (exists) {
        success = SetData(path, value);
      } else {
        success = CreatePath(path,
                             value,
                             it_path_value->is_auto_deleted,
                             false,
                             false);
      }
    }

    if (success) {
      VLOG(0) << "Recover path success : " << path;
      it_path_value = unrecovered_paths_.erase(it_path_value);
    } else {
      it_path_value++;
      LOG(WARNING) << "Recover path fail, retry later : " << path;
    }
  }
  return unrecovered_paths_.empty();
}

void ZKManagerBase::OnZookeeperClosed() {
}

bool ZKManagerBase::AddAutoRecoverPath(const std::string& full_path,
                                       const std::string& value,
                                       bool auto_delete) {
  for (int i = 0; i < auto_recover_paths_.size(); ++i) {
    if (auto_recover_paths_[i].path == full_path) {
      LOG(ERROR) << "auto recovery path exist : " << full_path;
      return false;
    }
  }
  auto_recover_paths_.push_back(AutoRecoverPathInfo(full_path,
                                value, auto_delete));
  unrecovered_paths_.push_back(AutoRecoverPathInfo(full_path,
                               value, auto_delete));
  return true;
}

void ZKManagerBase::Init(const string& zk_server,
                         const std::string& zk_userid,
                         Watcher watcher,
                         void* watcher_ctx) {
  zk_server_ = zk_server;
  watcher_ = watcher;
  watcher_ctx_ = watcher_ctx;
  zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
  // zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
  if (!zk_userid.empty()) {
    std::vector<std::string> s;
    SplitString(zk_userid, ':', &s);
    CHECK_EQ(s.size(), 2);
    auth_username_ = s[0];
    auth_password_ = s[1];
    GetDigestID(auth_username_, auth_password_, &digest_id_);
  }
  Init();
}

void ZKManagerBase::Init() {
  zh_ = ZooInit(zk_server_, watcher_, watcher_ctx_);
  if (!zh_) {
    PLOG(ERROR) << "init zookeeper failed";
    return;
  }
  if (zh_ && !auth_username_.empty() && !auth_password_.empty()) {
    /*
     * A trick code here to wait for connection been created
     * or else AddSelfAuth may fail due to conenction not done
     */
    GetData("/");
    AddSelfAuth(auth_username_, auth_password_);
  }
}

ZKManagerBase::~ZKManagerBase() {
  VLOG(4) << "destruct ZKManagerBase";
  ClearCallbacks();
  int rc = ZooClose(zh_);
  LOG_IF(ERROR, rc != ZOK) << "close zookeeper fails, rc = " << rc;
}

void ZKManagerBase::GetCallbacks(const std::string& full_path,
                                 std::vector<base::Closure*> *callbacks) const {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  std::map<std::string,
      std::vector<base::shared_ptr<base::Closure> > >::const_iterator it =
          callbacks_.find(full_path);
  if (it != callbacks_.end()) {
    std::vector<base::shared_ptr<base::Closure> >::const_iterator it2;
    for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      callbacks->push_back(it2->get());
    }
  }
}


void ZKManagerBase::GetAllCallbacks(std::vector<base::Closure*> *
                                    callbacks) const {
  std::map<std::string,
      std::vector<base::shared_ptr<base::Closure> > >::const_iterator it;
  for (it = callbacks_.begin(); it != callbacks_.end(); ++it) {
    std::vector<base::shared_ptr<base::Closure> >::const_iterator it2;
    for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      callbacks->push_back(it2->get());
    }
  }
}

void ZKManagerBase::ClearCallbacks() {
  callbacks_.clear();
}

bool ZKManagerBase::GetChildren(const string& full_path,
                                std::vector<std::string>* children,
                                bool watch) {
  if (!zh_) {
    return false;
  }

  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  VLOG(4) << "GetChildren " << full_path << " with watch=" << watch;
  children->clear();
  int rc = ZooGetChildren(zh_, full_path, watch, children);

  if (rc != ZOK) {
    LOG(ERROR) << "cannot get path:" << full_path << ",rc=" << rc
               << " " << zooRc2String(rc);
  }
  if (rc == ZOK || rc == ZNONODE) {
    if (rc == ZOK && watch) VLOG(3) << "add watch for " << full_path;
    return true;
  }
  return false;
}

// Will set watch = true for full_path and all its children.
bool ZKManagerBase::List(const string& full_path,
                         vector<pair<string, string> >* results) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (!zh_) {
    return false;
  }
  VLOG(3) << "List: " << full_path;
  string result = GetData(full_path);  // will add watch to full_path
  results->push_back(pair<string, string>(full_path, result));

  vector<string> childrens;
  if (!GetChildren(full_path, &childrens, false)) {
    LOG(ERROR) << "cannot get children with " << full_path;
    return false;
  }

  for (int i = 0; i < childrens.size(); i++) {
    if (full_path == "/" || full_path[full_path.size() - 1] == '/') {
      List(full_path + childrens[i], results);
    } else {
      List(full_path + "/" + childrens[i], results);
    }
  }

  return true;
}

bool ZKManagerBase::Delete(const string& full_path,
                           bool recursive) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (!zh_) {
    return false;
  }

  if (recursive) {
    vector<pair<string, string> > results;
    if (!List(full_path, &results)) {
      LOG(ERROR) << "Cannot list " << full_path;
      return false;
    }

    for (int i = results.size() - 1; i >= 0; i--) {
      if (!Delete(results[i].first, false)) {
        LOG(ERROR) << "Cannot delete " << results[i].first;
        return false;
      }
    }
  } else {
    GetData(full_path);  // trick, set watch = true for full_path
    int rc = ZooDelete(zh_, full_path, -1);
    if (rc != ZOK) {
        LOG(ERROR) << "Cannot delete " << full_path
                   << " with " << zooRc2String(rc);
      return false;
    }
  }
  return true;
}


bool ZKManagerBase::SetData(const string& full_path, const string& value) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (!zh_) {
    return false;
  }
  int rc = ZooSet(zh_, full_path, value, -1);
  if (rc != ZOK) {
    LOG(INFO) << "set value failed:" << full_path << ", rc = " << rc;
    return false;
  }
  return true;
}

// Note: Always set watch = true for full_path
string ZKManagerBase::GetData(const string& full_path,
                              const shared_ptr<Closure>& closure/*NULL*/) {
  string data = "";
  GetData(full_path, &data, closure);
  return data;
}

bool ZKManagerBase::GetData(const string& full_path,
                            std::string* result,
                            const shared_ptr<Closure>& closure) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (closure.get()) {
    RegisterCallBack(full_path, closure);
  }
  if (!zh_) {
    return false;
  }
  int rc = ZOK;
  int retry = 5;
  while (retry-- > 0) {
    rc = ZooGet(zh_, full_path, true, result, NULL);  // watch always be true
    if (rc == ZOPERATIONTIMEOUT || rc == ZCONNECTIONLOSS) {
      LOG(WARNING) << "Get data from ZK path timeout or connection lost, "
                      "sleep and retry... : " << full_path;
      sleep(1);
      continue;
    }
    break;
  }
  if (rc != ZOK) {
    LOG(ERROR) << "Cannot get data from " << full_path << " rc = " << rc
               << " " << zooRc2String(rc);
    return false;
  } else {
    VLOG(3) << "Add watch for " << full_path;
  }
  VLOG(3) << "result:" << *result;
  return true;
}

// result indicates the path exists or not
// return value indicates the operation success or fail
bool ZKManagerBase::Exists(const string& full_path, bool* result) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (!zh_) {
    return false;
  }
  struct Stat stat;
  int retry = 5;
  int rc;
  while (retry-- > 0) {
    rc = ZooExists(zh_, full_path, true, &stat);
    // always set watch = true for full_path
    if (rc == ZOK) {
      *result = true;
      VLOG(3) << "add watch for " << full_path;
      return true;
    } else if (rc == ZNONODE) {
      *result = false;
      return true;
    } else if (rc == ZOPERATIONTIMEOUT || rc == ZCONNECTIONLOSS) {
      LOG(WARNING) << "Test ZK path exists timeout or connection lost, "
                      "sleep and retry..." << full_path;
      sleep(1);
      continue;
    } else {
      break;
    }
  }
  LOG(ERROR) << "Test ZK path exists failed:" << full_path
             << ", rc = " << rc;
  return false;
}

bool ZKManagerBase::Exists(const string& full_path) {
  bool result = false;
  Exists(full_path, &result);
  return result;
}

bool ZKManagerBase::CreatePath(const string& full_path,
                               const string& value,
                               bool auto_delete,
                               bool sequence,
                               bool reconnect,
                               std::string *real_path /*=null*/) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  int index = full_path.rfind("/");
  CHECK_GE(index, 0) << "path:" << full_path;
  string parent_path = full_path.substr(0, index);
  bool parent_exists;
  if (!Exists(parent_path, &parent_exists)) {
    LOG(ERROR) << "Cannot test if path exists:" << parent_path;
    return false;
  }
  if (!parent_exists) {
    // Create nodes from root
    int index = 0;
    while (true) {
      index++;
      index = full_path.find("/", index);
      if (index == string::npos) {
        break;
      }
      string temp = full_path.substr(0, index);
      VLOG(5) << "sub:" << temp;
      bool exists;
      if (!Exists(temp, &exists)) {
        LOG(ERROR) << "Cannot test if path exists:" << temp;
        return false;
      }
      if (!exists &&
          !CreateNodeWithRetry(temp, "", false, false, real_path)) {
        if (!Exists(temp)) {
          LOG(ERROR) << "cannot create " << temp;
          return false;
        }
      }
    }
  }
  if (!CreateNodeWithRetry(full_path, value,
                           auto_delete, sequence, real_path)) {
    LOG(ERROR) << "cannot create " << full_path;
    return false;
  }
  Exists(full_path);  // trick, set watch = true for full_path
  if (reconnect && auto_delete) {
    auto_recover_paths_.push_back(AutoRecoverPathInfo(full_path,
                                  value, auto_delete));
  }
  return true;
}

bool ZKManagerBase::CreatePath(const string& full_path,
                               const string& value,
                               bool auto_delete,
                               bool sequence) {
  return CreatePath(full_path, value, auto_delete, sequence, true);
}

bool ZKManagerBase::CreateSequencePath(const std::string& full_path,
                                       const std::string& value,
                                       bool auto_delete,
                                       std::string *real_path) {
  CHECK(real_path);
  return CreatePath(full_path, value, auto_delete, true, true, real_path);
}

bool ZKManagerBase::CreateNodeWithRetry(const string& full_path,
                                        const string& value,
                                        bool auto_delete,
                                        bool sequence,
                                        std::string *real_path) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (!zh_) {
    return false;
  }
  int retry = 5;
  while (retry-- > 0) {
    int rc = ZooCreate(zh_, full_path, value, auto_delete, sequence, real_path);
    if (rc == ZOPERATIONTIMEOUT || rc == ZCONNECTIONLOSS) {
      LOG(WARNING) << "create ZK node timeout or connection lost, "
          "sleep and retry...";
      sleep(1);
      continue;
    }
    if (rc == ZOK) {
      return true;
    }
    LOG(ERROR) << "cannot create " << full_path << ", rc = " << rc;
    break;
  }
  return false;
}

std::string ZKManagerBase::ACLVectorToString(const struct
                                             ACL_vector &acl_vector) {
  std::string s;
  for (int i = 0; i < acl_vector.count; ++i) {
    StringAppendF(&s, "[%d:%s:%s:",
                  i,
                  acl_vector.data[i].id.scheme,
                  acl_vector.data[i].id.id);

    if (acl_vector.data[i].perms & ZOO_PERM_READ) {
      StringAppendF(&s, "r");
    }
    if (acl_vector.data[i].perms & ZOO_PERM_WRITE) {
      StringAppendF(&s, "w");
    }
    if (acl_vector.data[i].perms & ZOO_PERM_CREATE) {
      StringAppendF(&s, "c");
    }
    if (acl_vector.data[i].perms & ZOO_PERM_DELETE) {
      StringAppendF(&s, "d");
    }
    if (acl_vector.data[i].perms & ZOO_PERM_ADMIN) {
      StringAppendF(&s, "a");
    }

    StringAppendF(&s, "] ");
  }
  return s;
}

bool ZKManagerBase::AddSelfAuth(const std::string &username,
                                const std::string &password) {
  CHECK(!username.empty());
  CHECK(!password.empty());
  if (!zh_) {
    return false;
  }
  std::string new_id = StringPrintf("%s:%s",
                                    username.c_str(),
                                    password.c_str());

  int rc = ZooAddAuth(zh_, digest_scheme, new_id);
  if (rc != ZOK) {
    LOG(ERROR) << "add auth fail : " << username << " " << password
               << " " << zooRc2String(rc);
  } else {
    VLOG(3) << "add auth success : " << username << " " << password;
  }
  return rc == ZOK;
}

bool ZKManagerBase::SetAcl(const std::string& full_path,
                           AclType acl_type) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (acl_type == ACL_TYPE_NONE) {
    return true;
  } else if (acl_type == ACL_TYPE_SET_OTHER_READONLY) {
    return SetAclMackOtherReadonly(full_path);
  } else {
    LOG(WARNING) << "SetAcl para wrong " << full_path << " " << acl_type;
    return false;
  }
}

bool ZKManagerBase::SetAclMackOtherReadonly(const std::string& full_path) {
  CHECK(!full_path.empty());
  if (digest_id_.empty()) {
    LOG(WARNING) << "SetAclMackOtherReadonly fail because digest_id_ is empty";
    return false;
  }
  /*
   * 1. get acl
   * 2. change id "world:anyone" to readonly
   * 3. add username:password to acl
   */
  struct Stat stat;
  struct ACL_vector old_acl;
  bool ret = GetAcl(full_path, &old_acl, &stat);
  if (!ret) {
    return ret;
  }
  bool has_default_acl = false;
  for (int i = 0; i < old_acl.count; ++i) {
    if (old_acl.data[i].id.scheme == default_scheme &&
        old_acl.data[i].id.id == default_id) {
      has_default_acl = true;
      old_acl.data[i].perms = ZOO_PERM_READ;
    }
  }
  struct ACL_vector new_acl;
  scoped_ptr<ACL> new_acl_data;

  new_acl.count = old_acl.count + (has_default_acl ? 1 : 2);
  new_acl_data.reset(new ACL[new_acl.count]);
  new_acl.data = new_acl_data.get();

  // set default user to readonly
  if (!has_default_acl) {
    int index = old_acl.count + 1;
    new_acl.data[index].perms = ZOO_PERM_READ;
    new_acl.data[index].id.scheme = const_cast<char*>(default_scheme.c_str());
    new_acl.data[index].id.id = const_cast<char*>(default_id.c_str());
  }

  // copy old user
  for (int i = 0; i < old_acl.count; ++i) {
    new_acl.data[i] = old_acl.data[i];
  }

  // set new user
  int index = old_acl.count;
  new_acl.data[index].perms = ZOO_PERM_ALL;
  new_acl.data[index].id.scheme = const_cast<char*>(digest_scheme.c_str());
  new_acl.data[index].id.id = const_cast<char*>(digest_id_.c_str());

  ret = SetAcl(full_path, new_acl, -1);

  deallocate_ACL_vector(&old_acl);

  return ret;
}

bool ZKManagerBase::SetAcl(const std::string& full_path,
                           const struct ACL_vector &acl_vector,
                           int version) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  if (!zh_) {
    return false;
  }
  int rc = ZooSetAcl(zh_, full_path, version, &acl_vector);
  if (rc != ZOK) {
    LOG(ERROR) << "set acl fail : " << full_path
               << " " << ACLVectorToString(acl_vector)
               << " " << zooRc2String(rc);
  } else {
    VLOG(3) << "set acl success : " << full_path
            << " " << ACLVectorToString(acl_vector);
  }
  return rc == ZOK;
}

bool ZKManagerBase::GetAcl(const std::string& full_path,
                           struct ACL_vector *acl_vector,
                           struct Stat *stat) {
  CHECK(!full_path.empty() && full_path[0] == '/') << full_path;
  CHECK(acl_vector);
  CHECK(stat);
  if (!zh_) {
    return false;
  }
  int rc = ZooGetAcl(zh_, full_path, acl_vector, stat);
  if (rc != ZOK) {
    LOG(ERROR) << "get acl fail : " << full_path
               << " " << zooRc2String(rc);
  } else {
    VLOG(3) << "get acl success : " << full_path
            << " " << ACLVectorToString(*acl_vector);
  }
  return rc == ZOK;
}

void ZKManagerBase::SHAString(const std::string &input,
                              std::string *output) {
  const int HASH_SIZE = 20;
  unsigned char hash[HASH_SIZE];
  scoped_ptr<unsigned char> d(new unsigned char[input.size()]);
  for (int i = 0; i < input.size(); ++i) {
    d.get()[i] = input[i];
  }

  SHA1(d.get(), input.size(), hash);

  std::string tmp;
  for (int i = 0; i < HASH_SIZE; ++i) {
    tmp.push_back(hash[i]);
  }
  Base64Encoder encoder;
  encoder.Encode(tmp, output);
}

void ZKManagerBase::GetDigestID(const std::string &username,
                                const std::string &password,
                                std::string *output) {
  std::string tmp_input = StringPrintf("%s:%s",
                                       username.c_str(),
                                       password.c_str());
  std::string tmp_output;
  SHAString(tmp_input, &tmp_output);
  *output = StringPrintf("%s:%s", username.c_str(),
                                  tmp_output.c_str());
}

}  // namespace util

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#ifndef MANAGER_BASE_H_
#define MANAGER_BASE_H_

#include <sys/ioctl.h>
#include <stdio.h>
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
#include "third_part/zookeeper/include/zookeeper/zookeeper.h"

struct _zhandle;
typedef struct _zhandle zhandle_t;
namespace util {

typedef void (*Watcher)(
      zhandle_t *zh,
      int type,
      int state,
      const char *path,
      void *watcherCtx);

enum AclType {
  ACL_TYPE_NONE = 0,
  ACL_TYPE_SET_OTHER_READONLY,
};

class ZKManagerBase {
 public:
  ZKManagerBase();
  virtual ~ZKManagerBase();

  void Init(const std::string& zk_server,
            const std::string& zk_userid,
            Watcher watcher,
            void* watcher_ctx);

  void GetCallbacks(const std::string& full_path,
                    std::vector<base::Closure*> *callbacks) const;

  void GetAllCallbacks(std::vector<base::Closure*> *callbacks) const;

  void ClearCallbacks();

  // Note: List will set watch = true for full_path and its children
  bool List(const std::string& full_path,
            std::vector<std::pair<std::string, std::string> >* results);

  // Note: Delete will set watch = true for full_path and
  //       its children when recursive = true.
  //       Delete will set watch = true only for full_path
  //       when recursive = false.
  bool Delete(const std::string& full_path,
              bool recursive);

  // Note: GetData will always set watch = true for full_path
  std::string GetData(const std::string& full_path,
                      const base::shared_ptr<base::Closure>& closure =
                      base::shared_ptr<base::Closure>(NULL));
  bool GetData(const std::string& full_path,
               std::string* result,
               const base::shared_ptr<base::Closure>& closure =
               base::shared_ptr<base::Closure>(NULL));

  bool SetData(const std::string& full_path, const std::string& value);

  // Note: Exists will always set watch = true for full_path
  bool Exists(const std::string& full_path, bool* result);
  bool Exists(const std::string& full_path);

  // Note: CreatePath will always set watch = true for full_path and its
  //       patent, grapdfather, ... /
  bool CreatePath(const std::string& full_path,
                  const std::string& value,
                  bool auto_delete = true,
                  bool sequence = true);
  bool CreateSequencePath(const std::string& full_path,
                          const std::string& value,
                          bool auto_delete,
                          std::string *real_path);

  void RegisterCallBack(const std::string& full_path,
                        const base::shared_ptr<base::Closure>& closure);

  bool GetChildren(const std::string& full_path,
                   std::vector<std::string>* children,
                   bool watch = true);

  bool NeedReconnect();

  bool Reconnect();

  bool RecoverAutoDeletedPathValue();

  void OnZookeeperClosed();

  bool SetAcl(const std::string& full_path,
              AclType acl_type);

  bool AddAutoRecoverPath(const std::string& full_path,
                          const std::string& value,
                          bool auto_delete);


protected:
  zhandle_t *zh_;

  virtual zhandle_t* ZooInit(const std::string &server,
                             watcher_fn fn,
                             void *context) = 0;

  virtual int ZooClose(zhandle_t *zh) {
    return ZOK;
  }

  virtual int ZooCreate(zhandle_t *zh,
                        const std::string &full_path,
                        const std::string &value,
                        bool auto_delete,
                        bool sequence,
                        std::string *real_path) = 0;

  virtual int ZooDelete(zhandle_t *zh,
                        const std::string &full_path,
                        int version) = 0;

  virtual int ZooExists(zhandle_t *zh,
                        const std::string &full_path,
                        bool watch,
                        struct Stat *stat) = 0;

  virtual int ZooGetChildren(zhandle_t *zh,
                             const std::string &full_path,
                             bool watch,
                             std::vector<std::string>* children) = 0;

  virtual int ZooSet(zhandle_t *zh,
                     const std::string &full_path,
                     const std::string &value,
                     int version) = 0;

  virtual int ZooGet(zhandle_t *zh,
                     const std::string &full_path,
                     bool watch,
                     std::string *value,
                     struct Stat *stat) = 0;

  virtual int ZooAddAuth(zhandle_t *zh,
                         const std::string &scheme,
                         const std::string &cert) = 0;

  virtual int ZooSetAcl(zhandle_t *zh,
                        const std::string &full_path,
                        int version,
                        const struct ACL_vector *acl) = 0;

  virtual int ZooGetAcl(zhandle_t *zh,
                        const std::string &full_path,
                        struct ACL_vector *acl,
                        struct Stat *stat) = 0;

private:
  bool CreateNodeWithRetry(const std::string& full_path,
                           const std::string& value,
                           bool auto_delete,
                           bool sequence,
                           std::string *real_path);

  bool CreatePath(const std::string& full_path,
                  const std::string& value,
                  bool auto_delete,
                  bool sequence,
                  bool reconnect,
                  std::string *real_path = NULL);

  void Init();

  bool AddSelfAuth(const std::string &username,
                   const std::string &password);

  bool SetAclMackOtherReadonly(const std::string& full_path);

  bool SetAcl(const std::string& full_path,
              const struct ACL_vector &acl_vector,
              int version);

  bool GetAcl(const std::string& full_path,
              struct ACL_vector *acl_vector,
              struct Stat *stat);

  std::string ACLVectorToString(const struct ACL_vector &acl_vector);

  void GetDigestID(const std::string &username,
                   const std::string &password,
                   std::string *output);

  void SHAString(const std::string &input,
                 std::string *output);

private:
  struct AutoRecoverPathInfo {
    std::string path;
    std::string value;
    bool is_auto_deleted;

    AutoRecoverPathInfo(std::string path,
                        std::string value,
                        bool is_auto_deleted) {
        this->path = path;
        this->value = value;
        this->is_auto_deleted = is_auto_deleted;
    }
  };

private:
  std::map<std::string, std::vector<base::shared_ptr<base::Closure> > >
      callbacks_;
  std::string zk_server_;
  Watcher watcher_;
  void* watcher_ctx_;

  std::vector<AutoRecoverPathInfo> auto_recover_paths_;
  std::vector<AutoRecoverPathInfo> unrecovered_paths_;

  static const std::string digest_scheme;
  static const std::string default_scheme;
  static const std::string default_id;
  std::string auth_username_;
  std::string auth_password_;
  std::string digest_id_;
};

}  // namespace util

#endif  // MANAGER_BASE_H_

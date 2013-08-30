// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#include "../public/production_safety_checker.h"

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string>
#include <utility>
#include <vector>

#include "base/public/flags.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "base/public/scoped_ptr.h"

DEFINE_bool(disable_safety_check_user, true,
            "true to disable safety check on user");

DEFINE_bool(disable_safety_check_ns_registering,
            true,
            "true to disable safety check on YRNS registering");

namespace util {

using std::string;

string GetNameWithUID(uid_t uid) {
  struct passwd pwd;
  struct passwd *result;
  scoped_array<char> buf;
  size_t bufsize;
  int s;

  bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1) {          /* Value was indeterminate */
    bufsize = 16384;            /* Should be more than enough */
  }

  buf.reset(new char[bufsize]);
  CHECK(buf.get());

  s = getpwuid_r(uid, &pwd, buf.get(), bufsize, &result);
  if (result == NULL) {
    if (s == 0) {
      LOG(ERROR) << "Not found name for " << uid;
    } else {
      PLOG(ERROR) << "getpwnam_r failed";
    }
    return "";
  }
  return result->pw_name;
}

string GetNameWithGID(gid_t gid) {
  struct group gr;
  struct group *result;
  scoped_ptr<char> buf;
  size_t bufsize;
  int s;

  bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
  if (bufsize == -1) {          /* Value was indeterminate */
    bufsize = 16384;            /* Should be more than enough */
  }

  buf.reset(static_cast<char*>(malloc(bufsize)));
  CHECK(buf.get());

  s = getgrgid_r(gid, &gr, buf.get(), bufsize, &result);
  if (result == NULL) {
    if (s == 0) {
      LOG(ERROR) << "Not found name for " << gid;
    } else {
      errno = s;
      PLOG(ERROR) << "getpwnam_r failed";
    }
    return "";
  }
  return result->gr_name;
}

string ProductionSafetyChecker::GetLoginName() {
  char login_name[128];
  getlogin_r(login_name, arraysize(login_name));
  string tmp(login_name);
  return tmp;
}

string ProductionSafetyChecker::GetUserName() {
  uid_t uid = getuid();
  return GetNameWithUID(uid);
}

string ProductionSafetyChecker::GetEffectiveUserName() {
  uid_t euid = geteuid();
  return GetNameWithUID(euid);
}

string ProductionSafetyChecker::GetGroupName() {
  gid_t gid = getgid();
  return GetNameWithGID(gid);
}

string ProductionSafetyChecker::GetEffectiveGroupName() {
  gid_t egid = getegid();
  return GetNameWithGID(egid);
}

bool ProductionSafetyChecker::CheckUser() {
  const char kExpectedUser[] = "service";
  const char kExpectedGroup[] = "admin";
  if (FLAGS_disable_safety_check_user) {
    return true;
  }

  string login_name = GetLoginName();

  string real_user = GetUserName();

  string effective_user = GetEffectiveUserName();

  string real_group = GetGroupName();

  string effective_group = GetEffectiveGroupName();

  string user_info;
  StringAppendF(&user_info, "current:\nlogin:%s\n", login_name.c_str());
  StringAppendF(
      &user_info,
      "user:%s,effective user:%s\n",
      real_user.c_str(),
      effective_user.c_str());
  StringAppendF(
      &user_info,
      "group:%s,effective group:%s\n",
      real_group.c_str(),
      effective_group.c_str());

  StringAppendF(
      &user_info,
      "expected:\nuser:%s,group:%s.",
      kExpectedUser,
      kExpectedGroup);

  if (real_user != kExpectedUser ||
      effective_user != kExpectedUser ||
      real_group != kExpectedGroup ||
      effective_group != kExpectedGroup) {
    if (IsProduction()) {
      LOG(ERROR) << "invalid user or group:" << user_info
        << "please run the service via sudo -u yrservice";
      return false;
    } else {
      LOG(WARNING) << "invalid user or group:" << user_info;
    }
    return true;
  }

  VLOG(1) << user_info;

  return true;
}

bool ProductionSafetyChecker::CheckRegisteredkNSPath(
    const string& full_path) {
  if (FLAGS_disable_safety_check_ns_registering) {
    return true;
  }
  const string testing_path = "/NS/testing";
  if (!IsProduction() && !StartsWithASCII(full_path, testing_path, false)) {
    LOG(ERROR) <<
      "Should not register non-testing NS path for non production "
      "environment, You could ether start your service in production "
      "env via yrservice.py, either change ns_datacenter flag to testing "
      "to run as non-production env";
    return false;
  }

  return true;
}
bool ProductionSafetyChecker::IsProduction() {
  const char kYRProductionMagicEnvName[] = "PRODUCTION";
  const char kYRProductionMagicEnvValue[] = "72e50dbd";
  char * value = getenv(kYRProductionMagicEnvName);
  return (value != NULL &&
      strncmp(
        value,
        kYRProductionMagicEnvValue,
        arraysize(kYRProductionMagicEnvValue)) == 0);
}
}  // namespace util

// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <syslog.h>
#include <utmp.h>
#include <sstream>
#include "base/basictypes.h"

#include "util/production/production_safety_checker.h"
using std::string;
using std::stringstream;

string getTtyFrom(const char* tty, size_t tty_len) {
  setutent();
  struct utmp* u;
  while (true) {
      u = getutent();
      if (!u) break;
      if (u->ut_type != USER_PROCESS) continue;
      size_t ut_line_len = strlen(u->ut_line);

      if (tty_len < ut_line_len) {
        continue;
      }
      if (tty_len == ut_line_len &&
          strncmp(u->ut_line, tty, tty_len) == 0) {
        return u->ut_host;
      }
  }
  return "";
}

extern "C" {
int execve(const char *filename, char *const argv[], char *const envp[]) {
  int (*func)(const char *, char *const[], char *const[]);
  if (!util::ProductionSafetyChecker::IsProduction()) {
    // safety margin
    char cwd[1024];
    stringstream output;

    // open  syslog
    openlog("yraudit", LOG_PID, LOG_AUTHPRIV);

    // get pwd
    char *rt = getcwd(cwd, sizeof(cwd));
    if (!rt) {
      cwd[0] = 0;
    }

    // get tty
    char tty[1024];
    string from;
    int rc = ttyname_r(0, tty, sizeof(tty));
    if (rc) {
      tty[0] = 0;
    } else {
      const char *prefix = "/dev/";
      size_t n_prefix = strlen(prefix);
      if (strncmp(tty, prefix, n_prefix) == 0) {
        size_t new_len = strlen(tty) - n_prefix;
        memmove(tty, tty + n_prefix, new_len);
        tty[new_len] = 0;
      }
      from = getTtyFrom(tty, strlen(tty));
    }


    string login_name = util::ProductionSafetyChecker::GetLoginName();
    string user_name = util::ProductionSafetyChecker::GetUserName();
    string effective_user_name =
      util::ProductionSafetyChecker::GetEffectiveUserName();
    string group_name = util::ProductionSafetyChecker::GetGroupName();
    string effective_group_name =
      util::ProductionSafetyChecker::GetEffectiveGroupName();
    output << "[";
    if (login_name != "") {
      output << login_name;
    } else {
      output << "(null)";
    }
    output << ",";
    if (user_name != login_name) {
      output << user_name;
    } else {
      output << "*";
    }
    output << ",";
    if (effective_user_name != user_name) {
      output << effective_user_name;
    } else {
      output << "*";
    }
    output << ":";
    if (group_name != user_name) {
      output << group_name;
    } else {
      output << "*";
    }

    output << ",";
    if (effective_group_name != group_name) {
      output << effective_group_name;
    } else {
      output << "*";
    }

    output << ":" <<  tty << "@" << from << ":" << cwd << "]";

    for (int i = 0; argv[i] != NULL; i++) {
      output << " " <<  argv[i];
    }

    syslog(LOG_INFO, "%s", output.str().c_str());
  }

  *reinterpret_cast<void **>(&func) = dlsym(RTLD_NEXT, "execve");

  return((*func)(filename, argv, envp));
}
}

// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#include "../public/release_path.h"
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "third_part/release/public/deploy.h"
#include "base/public/env.h"
#include "base/public/string_util.h"

DEFINE_string(release_data_prefix, "", "prefix of data in release path");

namespace release {
using std::string;

namespace {

bool initialized = false;
bool is_in_release_env = true;
const string default_release_root = "/var/service";  // NOLINT
string release_root = default_release_root;  // NOLINT
string instance = "default";  // NOLINT
string component_name = "";   // NOLINT
string binary_name = "";      // NOLINT

const string GetDevEnvRoot() {
  char buf[PATH_MAX];
  char *r = getcwd(buf, PATH_MAX);
  if (r != buf) {
    LOG(ERROR) << "getcwd failed, reason: " << errno;
    return "";
  }
  const char kMark[] = "/service";
  const char *p = buf;
  while (true) {
    p = strstr(p, kMark);
    if (!p) return ".";
    p += strlen(kMark);
    if (*p == '/')
      return std::string(buf, p - buf + 1);
    if (*p == '\0')
      return std::string(buf, p - buf) + "/";
  }
  return ".";
}

string GetExePath() {
  pid_t pid = getpid();
  string exe_link = StringPrintf("/proc/%ld/exe", static_cast<int64>(pid));
  char buf[PATH_MAX];
  int path_size = readlink(exe_link.c_str(), buf, PATH_MAX);
  return string(buf, path_size);
}

void Init() {
  if (!initialized) {
    string exe_path = GetExePath();
    int binary_begin = exe_path.rfind('/');
    binary_name = exe_path.substr(binary_begin + 1);
    int default_root_path_size = default_release_root.length();
    bool starts_with_default_root = false;
    if (StartsWithASCII(exe_path, default_release_root, true)) {
      starts_with_default_root = true;
      int instance_end = exe_path.find('/', default_root_path_size + 1);
      int component_end = exe_path.find('/', instance_end + 1);
      if (instance_end < 0 || component_end < 0) {
        is_in_release_env = false;
      } else {
        instance = exe_path.substr(default_root_path_size + 1,
                                   instance_end - default_root_path_size - 1);
        component_name = exe_path.substr(instance_end + 1,
                                   component_end - instance_end - 1);
      }
    }
    if (is_in_release_env) {
      #include "third_part/release/internal/deploy/root_deploy.conf"
      string release_path = deploy::GetReleasePath(binary_name.c_str());
      int exe_dir_end = exe_path.rfind('/');
      string exe_dir = exe_path.substr(0, exe_dir_end);
      if (!release_path.empty()) {
        component_name = release_path.substr(0, release_path.find('/'));
        if (EndsWith(exe_dir, release_path, true)) {
          int instance_end = exe_dir.length() - release_path.length();
          int instance_beg = exe_dir.rfind('/', instance_end - 1);
          if (instance_beg > 0) {
            release_root = exe_dir.substr(0, instance_beg);
            instance = exe_dir.substr(instance_beg,
                                      instance_end - instance_beg - 1);
          } else {
            is_in_release_env = false;
          }
        } else {
          is_in_release_env = false;
        }
      } else if (!starts_with_default_root) {
        is_in_release_env = false;
      }
    }
    initialized = true;
  }
}
}

const string GetReleaseRoot() {
  if (!base::HasCommandLineParsed()) {
    LOG(WARNING) << "Command line have not been parsed, the value of release"
                 << " paths may be incorrect.";
  }

  Init();

  if (!is_in_release_env)
    return GetDevEnvRoot();

  return release_root;
}

const string GetReleaseStaticPath() {
  Init();

  if (!is_in_release_env)
    return "./";

  return StringPrintf("%s/%s/static/",
                      GetReleaseRoot().c_str(),
                      instance.c_str());
}

const string GetReleaseDataPath(const string &path) {
  if (path.empty() || path[0] == '/')
    return path;

  if (!FLAGS_release_data_prefix.empty())
    return StringPrintf("%s/data/%s",
                        FLAGS_release_data_prefix.c_str(),
                        path.c_str());

  Init();

  if (!is_in_release_env)
    return GetReleaseRoot() + "/data/" + path;

  return StringPrintf("%s/%s/static/data/%s",
                      GetReleaseRoot().c_str(),
                      instance.c_str(),
                      path.c_str());
}

const string GetBinPath() {
  Init();
  string bin_path(".");
  if (is_in_release_env) {
    bin_path = StringPrintf("%s/%s/%s/bin", GetReleaseRoot().c_str(),
                            instance.c_str(), component_name.c_str());
  }
  return bin_path;
}

const string GetConfPath() {
  Init();
  string conf_path(".");
  if (is_in_release_env) {
    conf_path = StringPrintf("%s/%s/%s/config", GetReleaseRoot().c_str(),
                             instance.c_str(), component_name.c_str());
  }
  return conf_path;
}

const string GetDataPath() {
  Init();
  string data_path(".");
  if (is_in_release_env) {
    data_path = StringPrintf("%s/%s/%s/data", GetReleaseRoot().c_str(),
                             instance.c_str(), component_name.c_str());
  }
  return data_path;
}

const string GetTraceLogPath() {
  Init();
  string log_path(".");
  if (is_in_release_env) {
    log_path = StringPrintf("%s/%s/tracelogs/%s", GetReleaseRoot().c_str(),
                            instance.c_str(), component_name.c_str());
  }
  return log_path;
}

const string GetRunPath() {
  Init();
  string run_path(".");
  if (is_in_release_env) {
    run_path = StringPrintf("%s/%s/run", GetReleaseRoot().c_str(),
                            instance.c_str());
  }
  return run_path;
}

const string GetLogPath() {
  Init();
  string log_path(".");
  if (is_in_release_env) {
    log_path = StringPrintf("%s/logs/%s/%s", GetReleaseRoot().c_str(),
                            component_name.c_str(), instance.c_str());
  }
  return log_path;
}

const string GetFlagsFilePath() {
  Init();
  string flags_path(".");
  if (is_in_release_env) {
    flags_path = StringPrintf("%s/%s/%s/flags/%s.flag",
                              GetReleaseRoot().c_str(),
                              instance.c_str(),
                              component_name.c_str(),
                              binary_name.c_str());
  } else {
    flags_path = StringPrintf("%s/production/%s/flags/%s.flag",
                              GetReleaseRoot().c_str(),
                              component_name.c_str(),
                              binary_name.c_str());
  }
  return flags_path;
}

}  // namespace release

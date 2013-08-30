// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#ifndef UTIL_RELEASE_RELEASE_PATH_H_
#define UTIL_RELEASE_RELEASE_PATH_H_

#include <string>
#include "base/public/flags.h"

DECLARE_string(release_data_prefix);

namespace release {

// Get the release root dir
const std::string GetReleaseRoot();

const std::string GetReleaseStaticPath();

// Get the absolute data file path for the given file that denoted by a relative
// path from the yr/yrdata in the release folder structure.
const std::string GetReleaseDataPath(const std::string &path);

// Get the correct path for the executing binary in the released folders
const std::string GetBinPath();

// Get the correct path for config files of the executing binary in the released
// folders
const std::string GetConfPath();

// Get the correct path for data of the executing binary in the released folders
const std::string GetDataPath();

// Get the correct path for trace log of the executing binary in the released
// folders
const std::string GetTraceLogPath();

// Get the correct path for run information (such as pid file) in the released
// folders
const std::string GetRunPath();

// Get the correct path for log of the executing binary in the released folders
const std::string GetLogPath();

// Get the correct path for flags file of the executing binary in the released
// folders
const std::string GetFlagsFilePath();
}  // namespace release

#endif  // UTIL_RELEASE_RELEASE_PATH_H_

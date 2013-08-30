// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#ifndef UTIL_RELEASE_RELEASE_FLAG_H_
#define UTIL_RELEASE_RELEASE_FLAG_H_

#include <string>
#include "base/public/flags.h"

DECLARE_string(flag_dump_file);

namespace release {
void ParseFlags(int* argc, char*** argv);
}

#endif  // UTIL_RELEASE_RELEASE_FLAG_H_

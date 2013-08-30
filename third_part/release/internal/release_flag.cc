// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#include "../public/release_flag.h"

#include <ctype.h>
#include <string>

#include "base/public/env.h"
#include "file/public/file.h"
#include "../public/release_path.h"

DECLARE_string(flagfile);

namespace release {

void ParseFlags(int* argc, char*** argv) {
  base::ParseCommandLineNonHelpFlags(argc, argv, false);
  if (FLAGS_flagfile.empty()) {
    std::string flags_file_path = GetFlagsFilePath();
    if (file::File::Exists(flags_file_path)) {
      base::ReadFromFlagsFile(flags_file_path, NULL, false);
    }
    base::ReparseCommandLineNonHelpFlags();
  }
}

}  // namespace release

// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#ifndef UTIL_GLOBAL_INIT_GLOBAL_INIT_H_
#define UTIL_GLOBAL_INIT_GLOBAL_INIT_H_

#include "base/public/at_exit.h"
#include "base/public/basictypes.h"

namespace util {

class GlobalInit {
 public:
  GlobalInit(int *argc, char ***argv);

 private:
  base::AtExitManager at_exit_manager_;
  DISALLOW_COPY_AND_ASSIGN(GlobalInit);
};

}  // namespace util

#endif  // UTIL_GLOBAL_INIT_GLOBAL_INIT_H_

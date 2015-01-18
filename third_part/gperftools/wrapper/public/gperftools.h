// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-18 09:32:18
// File  : gperftools.h
// Brief :

#ifndef  __GPERFTOOLS_H_
#define  __GPERFTOOLS_H_

#include <string>
#include "third_part/gperftools/include/gperftools/profiler.h"

namespace profiler {
void Start(const std::string & file_name);
void Stop();
}  // namespace profiler

#endif  //__GPERFTOOLS_H_

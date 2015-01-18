// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-18 09:34:19
// File  : gperftools.cc
// Brief :

#include "third_part/gperftools/wrapper/public/gperftools.h"

namespace gperftools {
// add a CpuProfileWrapper class in gperftools, in order to
// init the CpuProfiler class.
static CpuProfilerWrapper cpu_profiler_wrapper;

void Start(const std::string & file_name) {
  ProfilerStart(file_name.c_str());
}

void Stop() {
  ProfilerStop();
}
}  // namespace gperftools

// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yinghuang@yunrang.com (Ying Huang)

#include "../../../../base/public/flags.h"
#include "../../../../base/public/logging.h"
#include "../include/gtest/gtest.h"

int main(int argc, char **argv) {
  LOG(INFO) << "Running adapted main() from benchmark_main.cc";

  base::ParseCommandLineFlags(&argc, &argv, true);
  testing::RUN_ALL_BENCHMARKS();

  return 0;
}

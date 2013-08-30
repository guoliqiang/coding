// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author: bingbai@yunrang.com (Bing Bai)
//
#include <stdio.h>
#include "third_part/host_util/public/host_util.h"
#include "third_part/global_init/public/global_init.h"


int main(int argc, char* argv[]) {
  util::GlobalInit global_init(&argc, &argv);

  fprintf(stdout, "%s\n", util::host_util::LocalHostName().c_str());
  return 0;
}

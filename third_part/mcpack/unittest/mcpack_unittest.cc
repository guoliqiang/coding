// Copyright 2011 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-05-29 05:49:54
// File  : mcpack_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "../public/mc_pack.h"

void Write(mc_pack_t * & pack) {
  pack = mc_pack_open_w(2, new char[1000], 1000, new char[1000], 1000);
  CHECK(MC_PACK_PTR_ERR(pack) == 0) << "open mc_pack error!";
  int t = mc_pack_put_str(pack, "name", "guoliqiang");
  CHECK(t == 0) << "errno:" << t;
}

void Read(mc_pack_t * & pack) {
  const char * str = mc_pack_get_str(pack, "name");
  CHECK(MC_PACK_PTR_ERR(str) >= 0);
  LOG(INFO) << str;
}

int main(int argc, char** argv) {
  mc_pack_t * pack = NULL;
  Write(pack);
  Read(pack);
  return 0;
}


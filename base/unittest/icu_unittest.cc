// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-28 09:54:52
// File  : icu_unittest.cc
// Brief :

#include "base/public/icu_utf.h"
#include "base/public/logging.h"

using std::string;

int main(int argc, char** argv) {
  std::string foo = "中华人民abc共和国";
  for (int i = 0; i < foo.size() - 1; i++ ) {
    uint16 * ptr = reinterpret_cast<uint16*>(&foo[i]);
    LOG(INFO) << std::hex << *ptr;
  }
  int start = 0;
  while (start < foo.size()) {
    int ret = -1;
    int pre_start = start;
    CBU8_NEXT(foo.c_str(), start, static_cast<int32>(foo.size()), ret);
    CHECK_GE(ret, 0) << "not utf8 encoding";
    string single_utf8(foo, pre_start, start - pre_start);
    LOG(INFO) << single_utf8 << " " << start - pre_start
              << " "<< std::hex << static_cast<uint16>(ret);
  }
  return 0;
}

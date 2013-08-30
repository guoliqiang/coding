// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-19 13:55:34
// File  : curl_wrapper_unittest.cc
// Brief :

#include "../public/curl_wrapper.h"
#include "base/public/logging.h"

using namespace util;

int main(int argc, char** argv) {
  std::string out;
  std::string error;
  CurlWrapper::GetRespondData("www.baidu.com", &out, &error);
  LOG(INFO) << out;
  return 0;
}

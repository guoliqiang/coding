// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-08 01:42:46
// File  : curl_unittest.h
// Brief :

#include "base/public/logging.h"
#include "../public/curl_wrapper.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

namespace curl {
TEST(CurlWrapper, Normal) {
  CurlWrapper curl_wrapper;
  std::string rs;
  CHECK(curl_wrapper.HttpGet("www.baidu.com", 10, &rs));
  LOG(INFO) << rs;
}
}  // namespace curl

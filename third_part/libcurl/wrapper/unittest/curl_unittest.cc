// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-08 01:42:46
// File  : curl_unittest.h
// Brief :

#include "base/public/logging.h"
#include "../public/curl_wrapper.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "file/public/file_binary.h"

namespace curl {
TEST(CurlWrapper, Img) {
  CurlWrapper curl_wrapper;
  std::string rs;
  CHECK(curl_wrapper.HttpGet("http://www.veerchina.com/images/front/v/pic2/ff/ec/2167426688.jpg", 10, &rs));  // NOLINT
  LOG(INFO) << rs.size();
  file::FileBinary::WriteStringToFile(rs, "down.jpg");
}

TEST(CurlWrapper, Text) {
  CurlWrapper curl_wrapper;
  std::string rs;
  CHECK(curl_wrapper.HttpGet("www.baidu.com", 10, &rs));
  LOG(INFO) << rs.size();
}

}  // namespace curl

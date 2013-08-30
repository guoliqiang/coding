// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include <string>

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "../public/url_denormalizer.h"

using namespace std;  // NOLINT
using namespace base;  // NOLINT

TEST(UrlDenormalizer, DenormalizeDetect) {
  util::url_parser::UrlDenormalizer denormalizer;

  string url = "http://www.xn--wnuw27a.xn--3jsx3po13a.com/adf?asd=123";
  string res;
  denormalizer.Denormalize(url, &res);
  LOG(INFO) << res;
  EXPECT_EQ("http://www.我爱.肯德基.com/adf?asd=123",
            res);

  url = "http://www.xn--3jsx3po13a.com/adf?asd=123";
  denormalizer.Denormalize(url, &res);
  LOG(INFO) << res;
  EXPECT_EQ("http://www.肯德基.com/adf?asd=123",
            res);

  url = "http://www.xn--love-jo5g990b8h0d.com/adf?asd=123";
  denormalizer.Denormalize(url, &res);
  LOG(INFO) << res;
  EXPECT_EQ("http://www.love肯德基.com/adf?asd=123",
            res);

  url = "http://xn--7rsr7cp60aunhx2m.xn--fiqs8s/bbs/forumdisplay.php";
  denormalizer.Denormalize(url, &res);
  LOG(INFO) << res;
  EXPECT_EQ("http://留学海外网.中国/bbs/forumdisplay.php",
            res);

  url = "http://www.baike.baidu.com/adf?asd=123";
  denormalizer.Denormalize(url, &res);
  LOG(INFO) << res;
  EXPECT_EQ("http://www.baike.baidu.com/adf?asd=123",
            res);
}

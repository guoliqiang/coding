// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#include <string>
#include <map>
#include "base/public/logging.h"
#include "../public/http_client.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

using std::string;

TEST(HttpClient, ParseHttpHeader) {
  int32 response_code = 0;
  map<string, string> headers;
  string html =
      "http/1.1 200OK\r\n"
      "test: test\r\n"
      "content-encoding: gzip\r\n";
  EXPECT_EQ(true, util::http_client::ParseHttpHeader(
                      html, &response_code, &headers));
  EXPECT_EQ(200, response_code);
  EXPECT_EQ(2, headers.size());
  EXPECT_EQ(headers.end(), headers.find("content-encoding1"));
  EXPECT_EQ("gzip", headers["content-encoding"]);
  EXPECT_EQ("test", headers["test"]);
}

TEST(HttpClient, ParseHttpHeaderMulti) {
  int32 response_code = 0;
  map<string, string> headers;
  string html =
      "http/1.1 200OK\r\n"
      "test: test\r\n"
      "Test: test1\r\n";
  EXPECT_EQ(true, util::http_client::ParseHttpHeader(
                      html, &response_code, &headers));
  EXPECT_EQ(200, response_code);
  EXPECT_EQ(1, headers.size());
  EXPECT_EQ("test\ntest1", headers["test"]);
}

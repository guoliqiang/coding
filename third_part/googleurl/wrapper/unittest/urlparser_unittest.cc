// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "../public/url.h"

DECLARE_string(domain_suffix_data);
namespace util {
namespace url {

TEST(UrlParser_Unittest, Standard) {
  UrlParser url("http://user:pass@map.google.com:99/foo;bar?q=a#ref");
  EXPECT_TRUE(url.IsValid());
  EXPECT_TRUE(url.SchemeIs("http"));
  EXPECT_FALSE(url.SchemeIsFile());

  // This is the narrow version of the URL, which should match the wide input.
  EXPECT_EQ("http://user:pass@map.google.com:99/foo;bar?q=a#ref", url.Spec());

  EXPECT_EQ("http", url.Scheme());
  EXPECT_EQ("user", url.Username());
  EXPECT_EQ("pass", url.Password());
  EXPECT_EQ("map.google.com", url.Host());
  EXPECT_EQ("google.com", url.Domain());
  EXPECT_EQ("99", url.Port());
  int port = 0;
  EXPECT_EQ(true, url.IntPort(&port));
  EXPECT_EQ(99, port);
  EXPECT_EQ("/foo;bar", url.Path());
  EXPECT_EQ("q=a", url.Query());
  EXPECT_EQ("ref", url.Ref());
}

TEST(UrlParser_Unittest, Empty) {
  UrlParser url;
  EXPECT_FALSE(url.IsValid());
  EXPECT_EQ("", url.Spec());

  EXPECT_EQ("", url.Scheme());
  EXPECT_EQ("", url.Username());
  EXPECT_EQ("", url.Password());
  EXPECT_EQ("", url.Host());
  EXPECT_EQ("", url.Port());
  int port = 0;
  EXPECT_EQ(false, url.IntPort(&port));
  EXPECT_EQ("", url.Path());
  EXPECT_EQ("", url.Query());
  EXPECT_EQ("", url.Ref());
}

TEST(UrlParser_Unittest, Invalid) {
  UrlParser url("http:google.com:foo");
  EXPECT_FALSE(url.IsValid());
  EXPECT_EQ("http://google.com:foo/", url.PossiblyInvalidSpec());

  EXPECT_EQ("http", url.Scheme());
  EXPECT_EQ("", url.Username());
  EXPECT_EQ("", url.Password());
  EXPECT_EQ("google.com", url.Host());
  EXPECT_EQ("foo", url.Port());
  int port = 0;
  EXPECT_EQ(false, url.IntPort(&port));
  EXPECT_EQ("/", url.Path());
  EXPECT_EQ("", url.Query());
  EXPECT_EQ("", url.Ref());
}

TEST(UrlParser_Unittest, PathForRequest) {
  struct TestCase {
    const char* input;
    const char* expected;
  } cases[] = {
    {"http://www.google.com", "/"},
    {"http://www.google.com/", "/"},
    {"http://www.google.com/foo/bar.html?baz=22", "/foo/bar.html?baz=22"},
    {"http://www.google.com/foo/bar.html#ref", "/foo/bar.html"},
    {"http://www.google.com/foo/bar.html?query#ref", "/foo/bar.html?query"},
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    UrlParser url(cases[i].input);
    std::string path_request = url.PathForRequest();
    EXPECT_EQ(cases[i].expected, path_request);
  }
}

TEST(UrlParser_Unittest, GetOrigin) {
  struct TestCase {
    const char* input;
    const char* expected;
  } cases[] = {
    {"http://www.google.com", "http://www.google.com/"},
    {"javascript:window.alert(\"hello,world\");", ""},
    {"http://user:pass@www.google.com:21/bl#b", "http://www.google.com:21/"},
    {"http://user@www.google.com", "http://www.google.com/"},
    {"http://:pass@www.google.com", "http://www.google.com/"},
    {"http://:@www.google.com", "http://www.google.com/"},
  };
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    UrlParser url(cases[i].input);
    string origin = url.GetOrigin();
    EXPECT_EQ(cases[i].expected, origin);
  }
}

TEST(UrlParser_Unittest, IsStandard) {
  UrlParser a("http:foo/bar");
  EXPECT_TRUE(a.IsStandard());

  UrlParser b("foo:bar/baz");
  EXPECT_FALSE(b.IsStandard());

  UrlParser c("foo://bar/baz");
  EXPECT_FALSE(c.IsStandard());
}
}  // url
}  // namespace util

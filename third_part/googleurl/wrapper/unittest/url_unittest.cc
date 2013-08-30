// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/logging.h"
#include "third_part/googleurl/public/gurl.h"
#include "../public/url.h"

namespace util {
namespace url {

TEST(UrlParser_Unittest, IsHostPage) {
  EXPECT_EQ(false, IsHostPage("http://199.2.2.3/123"));
  EXPECT_EQ(false, IsHostPage("https://199.2.2.33/a?b=2"));
  EXPECT_EQ(true, IsHostPage("http://sina.123/"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/index.html"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/index.htm"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/index.php"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/index.jsp"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/default.asp"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/index.shtml"));
  EXPECT_EQ(true, IsHostPage("http://sports.sina.com.cn/default.aspx"));
  EXPECT_EQ(false, IsHostPage("http://sports.sina.com.cn/ik.aspx"));
  EXPECT_EQ(true, IsHostPage("http://fiwm.e6q5f.aviocun.trailalfa.com/"));
}

TEST(UrlParser_Unittest, IsHostPageEx) {
  EXPECT_EQ(false, IsHostPageEx("http://199.2.2.3/123"));
  EXPECT_EQ(false, IsHostPageEx("https://199.2.2.33/a?b=2"));
  EXPECT_EQ(true, IsHostPageEx("http://sina.123/"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/index.html"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/index.htm"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/index.php"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/index.jsp"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/default.asp"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/index.shtml"));
  EXPECT_EQ(true, IsHostPageEx("http://sports.sina.com.cn/default.aspx"));
  EXPECT_EQ(false, IsHostPageEx("http://sports.sina.com.cn/ik.aspx"));
  EXPECT_EQ(true, IsHostPageEx("http://fiwm.e6q5f.aviocun.trailalfa.com/"));
}

TEST(UrlParser_Unittest, IpHost) {
  EXPECT_EQ(true, HostIsIPAddress("http://199.2.2.3/1313j"));
  EXPECT_EQ(true, HostIsIPAddress("https://199.2.2.33/123"));
  EXPECT_EQ(false, HostIsIPAddress("http://sina.123"));

  string path;
  GetPath("http://www.baidu.com/?a=12", &path);
  LOG(INFO) << "Path " << path;

  string host;
  GetHost("http://www.baidu.com/", &host);
  LOG(INFO) << "Host " << host;
}

TEST(UrlParser_Unittest, Component) {
  std::string url = "http://user:pass@google.com:99/foo;bar?q=a#ref";

  std::string scheme;
  GetScheme(url, &scheme);
  EXPECT_EQ("http", scheme);

  std::string username;
  GetUserName(url, &username);
  EXPECT_EQ("user", username);

  std::string password;
  GetPassword(url, &password);
  EXPECT_EQ("pass", password);

  std::string host;
  GetHost(url, &host);
  EXPECT_EQ("google.com", host);

  std::string port;
  GetPort(url, &port);
  EXPECT_EQ("99", port);

  std::string path;
  GetPath(url, &path);
  EXPECT_EQ("/foo;bar", path);

  int path_level;
  bool is_dir;
  path_level = GetDirLevel(url, &is_dir);
  EXPECT_EQ(1, path_level);
  EXPECT_EQ(true, is_dir);

  std::string query;
  GetQuery(url, &query);
  EXPECT_EQ("q=a", query);

  std::string ref;
  GetRef(url, &ref);
  EXPECT_EQ("ref", ref);

  url = "http://google.com/a/b.shtml?a=123&b=123";
  path_level = GetDirLevel(url, &is_dir);
  EXPECT_EQ(1, path_level);
  EXPECT_EQ(false, is_dir);
}

TEST(UrlParser_Unittest, EscapeEncodeTest) {
  std::string url = "http://www.made-in-china.com/manufacturers-search/china-products/huntb3nolimit/Pendant_Lamp-9.html";  // NOLINT
  GURL gurl(url);
  std::string encoded_str;
  std::string decoded_str;

  EncodeURL(url, &encoded_str);
  EXPECT_EQ(gurl.spec(), encoded_str);

  DecodeURL(encoded_str, &decoded_str);
  EXPECT_EQ(url, decoded_str);

  std::string domain_str;
  GetDomain(url, &domain_str);
  EXPECT_EQ("made-in-china.com", domain_str);

  domain_str.clear();
  GetDomain("http://50.6.244.164/tags.php?/%B3%C9%C8%CBav%BF%EC%B2%A5/=",
            &domain_str);
  EXPECT_EQ("50.6.244.164", domain_str);
}


TEST(UrlParser_Unittest, Empty) {
  std::string url = "";

  std::string scheme;
  GetScheme(url, &scheme);
  EXPECT_EQ("", scheme);

  std::string username;
  GetUserName(url, &username);
  EXPECT_EQ("", username);

  std::string password;
  GetPassword(url, &password);
  EXPECT_EQ("", password);

  std::string host;
  GetHost(url, &host);
  EXPECT_EQ("", host);

  std::string port;
  GetPort(url, &port);
  EXPECT_EQ("", port);

  std::string path;
  GetPath(url, &path);
  EXPECT_EQ("", path);

  int path_level;
  bool is_dir;
  path_level = GetDirLevel(url, &is_dir);
  EXPECT_EQ(-1, path_level);

  std::string query;
  GetQuery(url, &query);
  EXPECT_EQ("", query);

  std::string ref;
  GetRef(url, &ref);
  EXPECT_EQ("", ref);
}

TEST(UrlParser_Unittest, Invalid) {
  std::string url = "http:google.com:foo";

  std::string scheme;
  GetScheme(url, &scheme);
  EXPECT_EQ("http", scheme);

  std::string username;
  GetUserName(url, &username);
  EXPECT_EQ("", username);

  std::string password;
  GetPassword(url, &password);
  EXPECT_EQ("", password);

  std::string host;
  GetHost(url, &host);
  EXPECT_EQ("google.com", host);

  std::string port;
  GetPort(url, &port);
  EXPECT_EQ("foo", port);

  std::string path;
  GetPath(url, &path);
  EXPECT_EQ("/", path);

  std::string query;
  GetQuery(url, &query);
  EXPECT_EQ("", query);

  std::string ref;
  GetRef(url, &ref);
  EXPECT_EQ("", ref);
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

  std::string path_request;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    path_request = "";
    GetPathForRequest(std::string(cases[i].input), &path_request);
    EXPECT_EQ(cases[i].expected, path_request);
  }
}

TEST(UrlParser_Unittest, Resolve) {
  struct TestCase {
    const char* base;
    const char* relative;
    const char* expected;
  } cases[] = {
    {"http://www.google.com", "/", "http://www.google.com/"},
    {"http://www.google.com/", "/", "http://www.google.com/"},

    {"http://www.google.com/foo/bar.html?baz=22",
     "../zoo.html?a=XX",
     "http://www.google.com/zoo.html?a=XX"},

    {"http://www.google.com/foo/bar.html#ref",
     "http://www.yunrang.com/",
     "http://www.yunrang.com/"},

    {"http://www.google.com:8080/foo/a/bar.html#ref",
     "../../zoo.html#ref",
     "http://www.google.com:8080/zoo.html#ref"},

    {"http://www.google.com:80/foo/a/bar.html#ref",
     "../../zoo.html",
     "http://www.google.com/zoo.html"},

    {"http://www.google.com/foo/bar.html?query#ref",
     "zoo.html",
     "http://www.google.com/foo/zoo.html"},
  };

  std::string result;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    result.clear();
    UrlParser parser(cases[i].base);
    parser.Resolve(cases[i].relative, &result);
    EXPECT_EQ(cases[i].expected, result);
  }
}

TEST(UrlParser_Unittest, GetOrigin) {
  struct TestCase {
    const char* input;
    const char* expected;
  } cases[] = {
    {"http://www.google.com", "http://www.google.com/"},
    {"javascript:window.alert(\"hello,world\");", ""},
    {"http://user:pass@www.google.com:21/blah#baz",
      "http://www.google.com:21/"},
    {"http://user@www.google.com", "http://www.google.com/"},
    {"http://:pass@www.google.com", "http://www.google.com/"},
    {"http://:@www.google.com", "http://www.google.com/"},
  };
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    std::string url(cases[i].input);
    std::string origin;
    GetOrigin(url, &origin);
    EXPECT_EQ(cases[i].expected, origin);
  }
}

TEST(UrlParser_Unittest, NormalizeURLForCrawler) {
  struct TestCase {
    const char* url;
    const char* expected;
  } cases[] = {
    {"HTTP://www.google.com", "http://www.google.com/"},
    {"HTTPS://www.google.com", "https://www.google.com/"},
    {"FTP://www.google.com", ""},
    {"http://www.Google.com/", "http://www.google.com/"},

    {"http://www.google.com/foo/bar.html#ref",
     "http://www.google.com/foo/bar.html"},

    {"http://www.google.com/foo/////bar.html",
     "http://www.google.com/foo/bar.html"},
    {"http://www.google.com/////foo/bar.html",
     "http://www.google.com/foo/bar.html"},
    {"http://www.google.com/////foo/bar////",
     "http://www.google.com/foo/bar/"},
    {"http://www.google.com/foo/bar.html?test=123&test=456#test",
     "http://www.google.com/foo/bar.html?test=123&test=456"},

    {"http://www.google.com/foo/教育新闻",
     "http://www.google.com/foo/%E6%95%99%E8%82%B2%E6%96%B0%E9%97%BB"},
    {"http://smqcw@126.com",
     ""},
    {"http://mailto:smqcw@126.com",
     ""},
    {"http://smqcw@126.com",
     ""},

    {"javascript:window.alert(\"hello,world\");", ""},
    {"http://www.google.com/foo/bar.html?test=123&amp;test=456&ampaaa#test",
     "http://www.google.com/foo/bar.html?test=123&test=456&ampaaa"},
    {"http://www.google.com/bar.html?test=123&amp;amp;amp;test=456&ampaaa#test",
     "http://www.google.com/bar.html?test=123&test=456&ampaaa"},
    {"http://www.google.com/foo/bar.html?test=123&amp;amp;amp;test=456&",
     "http://www.google.com/foo/bar.html?test=123&test=456"},
    {"http://www.google.com/foo/bar.html?test=456&&&",
     "http://www.google.com/foo/bar.html?test=456"},
    {"http://www.google.com/foo/bar.html?test=456&&?&",
     "http://www.google.com/foo/bar.html?test=456"},
    {"http://www.google.com/foo/#!test=456&&?&",
     "http://www.google.com/foo/#!test=456"},
    {"http://vi0.ku6img.com/data1/p4//ku6video/2012/1/4/0/"
     "1328813909110_13868691_13868691/5.jpg",
     "http://vi0.ku6img.com/data1/p4/ku6video/2012/1/4/0/"
     "1328813909110_13868691_13868691/5.jpg"},
    {"http://torrentscan.com/hu/#!/search/VA Remixland",
     "http://torrentscan.com/hu/#!/search/VA%20Remixland"},
    {"http://torrentscan.com/hu/#!/search/测试",
     "http://torrentscan.com/hu/#!/search/%E6%B5%8B%E8%AF%95"},
    {"http://torrentscan.com/hu/search/测试",
     "http://torrentscan.com/hu/search/%E6%B5%8B%E8%AF%95"},
    {"http://torrentscan.com/hu/search?",
     "http://torrentscan.com/hu/search"},
    {" http://torrentscan.com/hu/search ",
     "http://torrentscan.com/hu/search"},
  };

  std::string result;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    result.clear();
    NormalizeURLForCrawler(cases[i].url, &result);
    EXPECT_EQ(cases[i].expected, result);
  }
}


TEST(UrlParser_Unittest, Totally) {
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int line_no = 0;
  fp = fopen("../testdata/stdout", "r");
  if (fp == NULL) {
    ASSERT_TRUE(false);
  }

  std::string url;
  std::string scheme;
  std::string username;
  std::string password;
  std::string host;
  std::string port;
  std::string path;
  std::string query;
  std::string ref;
  GURL* gurl = NULL;
  while ((read = getline(&line, &len, fp)) != -1) {
    line_no++;
//    LOG(INFO)<< "parsed url:"<< line;

    // set data
    url = line;
    gurl = new GURL(url);

    GetScheme(url, &scheme);
    EXPECT_EQ(gurl->scheme(), scheme);

    GetUserName(url, &username);
    EXPECT_EQ(gurl->username(), username);

    GetPassword(url, &password);
    EXPECT_EQ(gurl->password(), password);

    GetHost(url, &host);
    EXPECT_EQ(gurl->host(), host);

    GetPort(url, &port);
    EXPECT_EQ(gurl->port(), port);

    GetPath(url, &path);
    EXPECT_EQ(gurl->path(), path);

    GetQuery(url, &query);
    EXPECT_EQ(gurl->query(), query);

    GetRef(url, &ref);
    EXPECT_EQ(gurl->ref(), ref);

    delete gurl;
    gurl = NULL;
  }

  if (line) {
    free(line);
  }

  LOG(INFO) << "Amount of urls be checked:" << line_no;
}

TEST(UrlParser_Unittest, NormalizeSiteHomepage) {
  std::string result;
  std::string site = "http://www.qq.com/index.html";
  EXPECT_TRUE(util::url::NormalizeSiteHomepage(site, &result));
  EXPECT_EQ(result, "http://www.qq.com/");

  site = "http://tech.qq.com/web/index.htm";
  EXPECT_FALSE(util::url::NormalizeSiteHomepage(site, &result));
  site = "http://tech.qq.com/a/20110317/000051.htm";
  EXPECT_FALSE(util::url::NormalizeSiteHomepage(site, &result));

  site = "http://www.qidian.com/Default.aspx";
  EXPECT_TRUE(util::url::NormalizeSiteHomepage(site, &result));
  EXPECT_EQ("http://www.qidian.com/", result);
}

TEST(UrlParser_Unittest, GetDomains) {
  std::vector<std::string> domains;

  EXPECT_EQ(false, GetDomains("http://www.abc.common", &domains));

  EXPECT_EQ(true, GetDomains("http://www.google.com.cn", &domains));
  EXPECT_EQ(3, domains.size());
  EXPECT_EQ("google.com.cn", domains[0]);
  EXPECT_EQ("google.com", domains[1]);
  EXPECT_EQ("google", domains[2]);

  EXPECT_EQ(true, GetDomains("www.google.cn", &domains));

  EXPECT_EQ(true, GetDomains("http://www.google.cn", &domains));
  EXPECT_EQ(2, domains.size());
  EXPECT_EQ("google.cn", domains[0]);
  EXPECT_EQ("google", domains[1]);

  EXPECT_EQ(true, GetDomains("http://www.google.com", &domains));
  EXPECT_EQ(2, domains.size());
  EXPECT_EQ("google.com", domains[0]);
  EXPECT_EQ("google", domains[1]);
}

TEST(UrlParser_Unittest, GetDirWithLevel) {
  string result;

  GetDirWithLevel("http://www.foo.com/xxx", &result, 1);
  EXPECT_EQ("www.foo.com/", result);

  GetDirWithLevel("http://www.foo.com/xxx/xxx", &result, 1);
  EXPECT_EQ("www.foo.com/xxx/", result);

  GetDirWithLevel("http://www.foo.com/xxx/", &result, 1);
  EXPECT_EQ("www.foo.com/xxx/", result);

  GetDirWithLevel("http://www.foo.com/xxx/", &result, 2);
  EXPECT_EQ("www.foo.com/xxx/", result);

  GetDirWithLevel("http://www.foo.com/xxx/xxx/xxx", &result, 1);
  EXPECT_EQ("www.foo.com/xxx/", result);

  GetDirWithLevel("http://www.foo.com/xxx/xxx/xxx", &result, 2);
  EXPECT_EQ("www.foo.com/xxx/xxx/", result);

  GetDirWithLevel("http://www.foo.com/xxx/xxx/xxx", &result, 10);
  EXPECT_EQ("www.foo.com/xxx/xxx/", result);

  GetDirWithLevel("http://www.foo.com/xxx?xxx/xxx", &result, 1);
  EXPECT_EQ("www.foo.com/", result);

  GetDirWithLevel("http://www.foo.com/", &result, 10);
  EXPECT_EQ("www.foo.com/", result);

  GetDirWithLevel("http://www.foo.com//", &result, 10);
  EXPECT_EQ("www.foo.com//", result);
}

TEST(UrlParser_Unittest, GetSingleDirWithLevel) {
  string result;

  GetSingleDirWithLevel("http://www.foo.com/xxx", &result, 1);
  EXPECT_EQ("xxx", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/yyy", &result, 1);
  EXPECT_EQ("xxx", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/yyy", &result, 2);
  EXPECT_EQ("yyy", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/", &result, 1);
  EXPECT_EQ("xxx", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/", &result, 2);
  EXPECT_EQ("", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/yyy/zzz", &result, 1);
  EXPECT_EQ("xxx", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/yyy/zzz", &result, 2);
  EXPECT_EQ("yyy", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx/yyy/zzz", &result, 10);
  EXPECT_EQ("", result);

  GetSingleDirWithLevel("http://www.foo.com/xxx?yyy/zzz", &result, 1);
  EXPECT_EQ("xxx", result);

  GetSingleDirWithLevel("http://www.foo.com/", &result, 10);
  EXPECT_EQ("", result);

  GetSingleDirWithLevel("http://www.foo.com//", &result, 10);
  EXPECT_EQ("", result);
}
}  // url
}  // namespace util

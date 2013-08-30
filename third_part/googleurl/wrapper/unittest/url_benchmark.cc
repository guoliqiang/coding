// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "../public/url.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/logging.h"
#include "third_part/googleurl/public/gurl.h"

using testing::NumCPUs;

namespace util {
namespace url {
namespace benchmark{

void UrlParser() {
  for (int i = 0; i< 100; ++i) {
    std::string host;
    GURL url("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff");
  }
}

void GetInfoFromParser(int iteration) {
    for (int i = 0;i < iteration;++i) UrlParser();
}

// -------------------
void Scheme() {
  for (int i = 0; i< 100; ++i) {
    std::string scheme;
    util::url::GetScheme("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &scheme);
  }
}


void GetScheme(int iteration) {
    for (int i = 0;i < iteration;++i) Scheme();
}

// -------------------
void UserName() {
  for (int i = 0; i< 100; ++i) {
    std::string username;
    util::url::GetUserName("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &username);
  }
}


void GetUserName(int iteration) {
    for (int i = 0;i < iteration;++i) UserName();
}

// -------------------
void Password() {
  for (int i = 0; i< 100; ++i) {
    std::string password;
    util::url::GetPassword("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &password);
  }
}


void GetPassword(int iteration) {
    for (int i = 0;i < iteration;++i) Password();
}

// -------------------
void Path() {
  for (int i = 0; i< 100; ++i) {
    std::string path;
    util::url::GetPath("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &path);
  }
}


void GetPath(int iteration) {
    for (int i = 0;i < iteration;++i) Path();
}

// -------------------
void Port() {
  for (int i = 0; i< 100; ++i) {
    std::string port;
    util::url::GetPort("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &port);
  }
}


void GetPort(int iteration) {
    for (int i = 0;i < iteration;++i) Port();
}

// -------------------
void Query() {
  for (int i = 0; i< 100; ++i) {
    std::string query;
    util::url::GetQuery("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &query);
  }
}


void GetQuery(int iteration) {
    for (int i = 0;i < iteration;++i) Query();
}

// -------------------
void Ref() {
  for (int i = 0; i< 100; ++i) {
    std::string ref;
    util::url::GetRef("http://user:password@www.google.com.hk:8080/search?q=abc 云壤#refff", &ref);
  }
}


void GetRef(int iteration) {
    for (int i = 0;i < iteration;++i) Ref();
}

// -------------------
void Host() {
  for (int i = 0; i< 100; ++i) {
    std::string host;
    util::url::GetHost("http://www.google.com.hk:8080/search?q=abc 云壤#refff", &host);
  }
}

void GetHost(int iteration) {
    for (int i = 0;i < iteration;++i) Host();
}

BENCHMARK(GetInfoFromParser)->ThreadRange(1, NumCPUs());
BENCHMARK(GetScheme)->ThreadRange(1, NumCPUs());
BENCHMARK(GetUserName)->ThreadRange(1, NumCPUs());
BENCHMARK(GetPassword)->ThreadRange(1, NumCPUs());
BENCHMARK(GetPort)->ThreadRange(1, NumCPUs());
BENCHMARK(GetQuery)->ThreadRange(1, NumCPUs());
BENCHMARK(GetRef)->ThreadRange(1, NumCPUs());
BENCHMARK(GetPath)->ThreadRange(1, NumCPUs());
BENCHMARK(GetHost)->ThreadRange(1, NumCPUs());

}  // namespace benchmark
}  // namespace url
}  // namespace util 


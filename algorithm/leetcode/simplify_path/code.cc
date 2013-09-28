// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 23:51:55
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 23/23 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 52 milli secs
 * Progress: 243/243 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool Next(std::string & path, std::string * rs) {
  rs->clear();
  if (path.empty()) return false;
  int i = 0;
  while (path[i] != '/' && i < path.size()) {
    i++;
  }
  if (i == path.size()) {
    *rs = path;
    path.clear();
  } else {
    *rs = path.substr(0, i);
    while (i < path.size() && path[i] == '/') i++;
    path = (i == path.size()) ? "" : path.substr(i);
  }
  return true;
}

std::string SimplyPath(std::string path) {
  int b = 0;
  while (b < path.size() && (path[b] == ' ' || path[b] == '/')) b++;

  int e = path.size() - 1;
  while (e >= 0 && (path[e] == ' ')) e--;
  if (e <= b) return "/";
  path = path.substr(b, e - b + 1);

  std::stack<std::string> s;
  std::string foo = "";
  while (Next(path, &foo)) {
    if (foo == ".") continue;
    if (foo == "..") {
      if (!s.empty()) s.pop();
    } else {
      s.push(foo);
    }
  }
  std::string rs = "";
  while (!s.empty()) {
    std::string t = std::string("/") + s.top();
    rs = t + rs;
    s.pop();
  }
  if (rs.empty()) rs = "/";
  return rs;
}

}  //  namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string path = "/a/./b/../../c/";
  LOG(INFO) << SimplyPath(path);
  
  path = "/home/";
  LOG(INFO) << SimplyPath(path);
  
  path = "/home//foo/";
  LOG(INFO) << SimplyPath(path);
  
  path = "/../";
  LOG(INFO) << SimplyPath(path);
  
  path = "/";
  LOG(INFO) << SimplyPath(path);
  return 0;
}

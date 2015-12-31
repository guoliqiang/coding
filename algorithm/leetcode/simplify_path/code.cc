// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 23:51:55
// File  : code.cc
// Brief :

/*
Given an absolute path for a file (Unix-style), simplify it.

For example,
path = "/home/", => "/home"
path = "/a/./b/../../c/", => "/c"
click to show corner cases.

Corner Cases:
Did you consider the case where path = "/../"?
In this case, you should return "/".
Another corner case is the path might contain multiple 
*/

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


namespace third {

std::string SimplifyPath(std::string path) {
  // split 的写法，非常清晰
  std::vector<std::string> parts;
  std::string cur;
  for (int i = 0; i < path.size(); i++) {
    if (path[i] == '/') {
      if (cur.size()) parts.push_back(cur);
      cur.clear();
    } else {
      cur.push_back(path[i]);
    }
  }
  if (cur.size()) parts.push_back(cur);

  std::stack<std::string> stack;
  for (int i = 0; i < parts.size(); i++) {
    if (parts[i] == ".") continue;
    if (parts[i] == "..") {
      if (stack.size()) stack.pop();
    } else {
      stack.push(parts[i]);
    }
  }

  std::string ans;
  while (stack.size()) {
    ans = "/" + stack.top() + ans;
    stack.pop();
  }
  return ans.size() ? ans : "/";
}
}  // namespace third

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

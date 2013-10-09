// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 15:23:15
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int Distance(const std::string & v1, const std::string & v2) {
  int rs = 0;
  for (int i = 0; i < v1.size(); i++) {
    if (v1[i] != v2[i]) rs++;
  }
  return rs;
}

bool Find(std::string & v, std::string & final, std::vector<std::string> & list,
          std::vector<bool> & seen, std::vector<std::string> & path) {
  path.push_back(v);
  if (v == final) return true;
  for (int i = 0; i < list.size(); i++) {
    if (!seen[i] && Distance(v, list[i]) == 1) {
      // LOG(INFO) << list[i];
      seen[i] = true;
      if (Find(list[i], final, list, seen, path)) return true;
      seen[i] = false;
    }
  }
  path.pop_back();
  return false;
}

std::vector<std::string> Find(std::string & from, std::string & to, std::vector<std::string> & list) {
  std::vector<std::string> path;
  std::vector<bool> seen(list.size(), false);
  Find(from, to, list, seen, path);
  return path;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> v;
  v.push_back("LIME");
  v.push_back("LIMP");
  v.push_back("LAMP");
  v.push_back("LIKE");
  v.push_back("AAAA");
  v.push_back("BBBB");
  std::string from = "DAMP";
  std::string to = "LIKE";
  std::vector<std::string> rs = Find(from, to, v);
  LOG(INFO) << JoinVector(rs);
  return 0;
}

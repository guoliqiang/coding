// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 16:06:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::map<char, char> dict;

void Build() {
  dict['0'] = '0';
  dict['1'] = '1';
  dict['6'] = '9';
  dict['8'] = '8';
  dict['9'] = '6';
}

void Get(std::string & path, int size, int idx,
         std::vector<std::string> & ans) {
  if (idx == size) {
    ans.push_back(path);
    return;
  }
  for (std::map<char, char>::iterator i = dict.begin(); i != dict.end(); i++) {
    if (i->first == '0' && path.size() == 0) continue;
    path.push_back(i->first);
    Get(path, size, idx + 1, ans);
    path.resize(path.size() - 1);
  }
}

std::string GetOther(const std::string & str) {
  std::string ans;
  for (int i = str.size() - 1; i >= 0; i--) {
    ans.push_back(dict[str[i]]);
  }
  return ans;
}

std::vector<std::string> StrobogrammaticPrint(int n) {
  Build();
  std::vector<std::string> tmp;
  std::vector<std::string> ans;
  std::string path;
  Get(path, n / 2, 0, tmp);
  for (int i = 0; i < tmp.size(); i++) {
    std::string other = GetOther(tmp[i]);
    if (n % 2) {
      for (std::map<char, char>::iterator j = dict.begin(); j != dict.end(); j++) {
        if (j->first != j->second) continue;
        ans.push_back(tmp[i] + std::string(1, j->first) + other);
      }
    } else {
      ans.push_back(tmp[i] + other);
    }
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

namespace NB {
std::vector<std::string> Get(int n, int m) {
  std::vector<std::string> ans;
  if (n == 0) {
    ans.push_back("");
    return ans;
  }
  if (n == 1) {
    ans.push_back("0");
    ans.push_back("1");
    ans.push_back("8");
    return ans;
  }

  std::vector<std::string> tmp = Get(n - 2, m);
  for (int i = 0; i < tmp.size(); i++) {
    ans.push_back(std::string("1") + tmp[i] + "1");
    ans.push_back(std::string("6") + tmp[i] + "9");
    ans.push_back(std::string("9") + tmp[i] + "6");
    ans.push_back(std::string("8") + tmp[i] + "8");
    if (n != m) {
      ans.push_back("0" + tmp[i] + "0");
    }
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}

std::vector<std::string> StrobogrammaticPrint(int n) {
  return Get(n, n);
}
}  // namespace NB

int main(int argc, char** argv) {
  LOG(INFO) << JoinVector(StrobogrammaticPrint(1));
  LOG(INFO) << JoinVector(NB::StrobogrammaticPrint(1));
  LOG(INFO) << JoinVector(StrobogrammaticPrint(3));
  LOG(INFO) << JoinVector(NB::StrobogrammaticPrint(3));
  LOG(INFO) << JoinVector(StrobogrammaticPrint(2));
  LOG(INFO) << JoinVector(NB::StrobogrammaticPrint(2));
  LOG(INFO) << JoinVector(StrobogrammaticPrint(4));
  LOG(INFO) << JoinVector(NB::StrobogrammaticPrint(4));
  return 0;
}

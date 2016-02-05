// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-20 22:08:14
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::string Encode(const std::string & str) {
  std::string ans;
  int idx = 0;
  while (idx < str.size()) {
    int i = idx;
    while (i < str.size() && str[i] == str[idx]) i++;
    ans.push_back('0' + (i - idx));
    ans.push_back('x');
    ans.push_back(str[idx]);
    idx = i;
  }

  return ans;
}

std::string Decode(const std::string & str) {
  std::string ans;
  int idx = 0;
  while (idx < str.size()) {
    int size = 0;
    while (str[idx] != 'x') {
      size = size * 10 + (str[idx] - '0');
      idx++;
    }
    ans.append(size, str[idx + 1]);
    idx += 2;
  }
  return ans;
}

}  // namespace algorithm

namespace NB {

std::string IntToString(int v) {
  char buff[100] = { 0 };
  sprintf(buff, "%d", v);
  return std::string(buff);
}

std::string Encode(const std::string & str) {
  std::string ans;
  std::string pre;
  int idx = 0;
  while (idx < str.size()) {
    int i = idx;
    while (i < str.size() && str[i] == str[idx]) i++;

    if (i == idx + 1) {
      pre.append(1, str[idx]);
    } else {
      if (pre.size()) {
        ans.append(1, 'x');
        ans += IntToString(pre.size());
        ans.append(1, 'x');
        ans += pre;
        pre.clear();
      }
      ans += IntToString(i - idx);
      ans.append(1, 'x');
      ans.append(1, str[idx]);
    }
    idx = i;
  }

  if (pre.size()) {
    ans.append(1, 'x');
    ans += IntToString(pre.size());
    ans.append(1, 'x');
    ans += pre;
    pre.clear();
  }

  return ans;
}

std::string Decode(const std::string & str) {
  std::string ans;
  int idx = 0;
  while (idx < str.size()) {
    int size = 0;
    if (str[idx] == 'x') {
      idx++;
      while (str[idx] != 'x') {
        size = size * 10 + (str[idx] - '0');
        idx++;
      }
      idx++;
      ans += str.substr(idx, size);
      idx += size;
    } else {
      while (str[idx] != 'x') {
        size = size * 10 + (str[idx] - '0');
        idx++;
      }
      ans.append(size, str[idx + 1]);
      idx += 2;
    }
  }
  return ans;
}

}  // namespace NB


using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << NB::Encode("a") << " " << "a";
  LOG(INFO) << Decode(Encode("a")) << " " << "a";
  LOG(INFO) << NB::Encode("abc") << " abc";
  LOG(INFO) << NB::Encode("abcxaxx") << " abcxaxx";
  LOG(INFO) << NB::Decode(NB::Encode("abcxaxx")) << " abcxaxx";
  LOG(INFO) << NB::Decode(NB::Encode("xabcxaxxabcx")) << " xabcxaxxabcx";
  return 0;
  LOG(INFO) << Decode(Encode("abc")) << " abc";
  LOG(INFO) << Encode("abb") << " abb";
  LOG(INFO) << Decode(Encode("abb")) << " abb";
  LOG(INFO) << Encode("aabbc") << " aabbc";
  LOG(INFO) << Decode(Encode("aabbc")) << " aabbc";
  LOG(INFO) << Encode("ebbb") << " ebbb";
  LOG(INFO) << Decode(Encode("ebbb")) << " ebbb";
  return 0;
}

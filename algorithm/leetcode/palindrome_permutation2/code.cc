// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-10 12:02:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Get(std::vector<char> & ch, std::vector<int> & cnt,
         std::string & path, int size, std::vector<std::string> & ans) {
  if (path.size() == size) {
    ans.push_back(path);
    return;
  }
  for (int i = 0; i < ch.size(); i++) {
    if (cnt[i] > 0) {
      cnt[i]--;
      path.push_back(ch[i]);
      Get(ch, cnt, path, size, ans);
      cnt[i]++;
      path.resize(path.size() - 1);
    }
  }
}

std::vector<std::string> GeneratePalindromes(std::string s) {
  std::map<char, int> t;
  std::vector<std::string> ans;
  for (int i = 0; i < s.size(); i++) {
    if (!t.count(s[i])) t[s[i]] = 0;
    t[s[i]]++;
  }
  char mid = '\0';
  std::vector<char> ch;
  std::vector<int> cnt;
  int size = 0;
  for (std::map<char, int>::iterator i = t.begin(); i != t.end(); i++) {
    ch.push_back(i->first);
    if (i->second % 2) {
      if (mid == '\0') mid = i->first;
      else return ans;
    }
    cnt.push_back(i->second / 2);
    size += cnt.back();
  }
  std::string path;
  Get(ch, cnt, path, size, ans);
  for (int i = 0; i < ans.size(); i++) {
    std::string cur = ans[i];
    if (mid != '\0') ans[i].append(1, mid);
    std::reverse(cur.begin(), cur.end());
    ans[i].append(cur);
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string s = "abb";
  LOG(INFO) << JoinVector(GeneratePalindromes(s));
  s = "aabb";
  LOG(INFO) << JoinVector(GeneratePalindromes(s));
  s = "abc";
  LOG(INFO) << JoinVector(GeneratePalindromes(s));
  return 0;
}

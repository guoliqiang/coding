// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-10 12:02:03
// File  : code.cc
// Brief :

/*
Given a string s, return all the palindromic permutations (without duplicates) of it. Return an empty list if no palindromic permutation could be form.

For example:

Given s = "aabb", return ["abba", "baab"].

Given s = "abc", return [].

Hint:

If a palindromic permutation exists, we just need to generate the first half of the string.
To generate all distinct permutations of a (half of) string, use a similar approach from: Permutations II or Next Permutation.
*/

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

namespace twice {
void DFS(std::vector<char> & v, std::vector<int> & c, std::string path,
         int size, std::vector<std::string> & ans, char mid) {
  if (path.size() == size) {
    ans.push_back(path);
    return;
  }
  if (path.size() == size - 1) {
    path.insert(path.begin() + path.size() / 2, mid);
    ans.push_back(path);
    return;
  }

  for (int i = 0; i < v.size(); i++) {
    if (c[i] >= 2) {
      c[i] -= 2;
      DFS(v, c, std::string(1, v[i]) + path + std::string(1, v[i]), size, ans, mid);
      c[i] += 2;
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
    cnt.push_back(i->second);
    size += cnt.back();
  }
  std::string path;
  DFS(ch, cnt, path, size, ans, mid);
  return ans;
}
}  // namspace twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::string s = "abb";
  LOG(INFO) << JoinVector(twice::GeneratePalindromes(s));
  s = "aabb";
  LOG(INFO) << JoinVector(twice::GeneratePalindromes(s));
  s = "abccba";
  LOG(INFO) << JoinVector(twice::GeneratePalindromes(s));
  return 0;
}

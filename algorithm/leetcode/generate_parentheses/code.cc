// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 13:21:11
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 3/3 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 28 milli secs
 * Progress: 8/8 test cases passed.
 * */

#include <vector>
#include <set>
#include "base/public/logging.h"


namespace algorithm {

void Generate(int n, std::vector<std::set<std::string> > * rs) {
  (*rs)[1].insert("()");
  for (int i = 2; i <= n; i++) {
    for (int j = 1; j < i; j++) {
      for (std::set<std::string>::iterator it1 = (*rs)[j].begin();
           it1 != (*rs)[j].end(); it1++) {
        for (std::set<std::string>::iterator it2 = (*rs)[i - j].begin();
             it2 != (*rs)[i - j].end(); it2++) {
          (*rs)[i].insert(*it1 + *it2);
          (*rs)[i].insert(*it2 + *it1);
          if (j == 1) (*rs)[i].insert("(" + *it2 + ")");
        }
      }
    }
  }
}

std::vector<std::string> GenerateParenthses(int n) {
  std::vector<std::set<std::string> > dp;
  for (int i = 0; i <= n; i++) {
    dp.push_back(std::set<std::string>());
  }
  Generate(n, &dp);
  std::vector<std::string> rs;
  for (std::set<std::string>::iterator i = dp[n].begin();
       i != dp[n].end(); i++) {
    rs.push_back(*i);
  }
  return rs;
}
}  // namespace algorithm

namespace official {

// 左括号可以随便加，右括号只有在当前串中右括号的个数小于左括号时
// 才可以加，注意path变量不是引用。
/*
 * Run Status: Accepted!
 * Program Runtime: 0 milli secs
 * Progress: 3/3 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 8/8 test cases passed.
 *
 * */

void Generate(int n, int l, int r,
              std::string path, std::vector<std::string> * rs) {
  if (l == n) {
    rs->push_back(path.append(n - r, ')'));
  } else {
    Generate(n, l + 1, r, path + "(", rs);
    if (r < l) Generate(n, l, r + 1, path + ")", rs);
  }
}

std::vector<std::string> GenerateParenthses(int n) {
  int l = 0;
  int r = 0;
  std::string path = "";
  std::vector<std::string> rs;
  Generate(n, l, r, path, &rs);
  return rs;
}

}  // namespace official

using namespace algorithm;

int main(int argc, char** argv) {

  std::vector<std::string> rs;
  rs = GenerateParenthses(3);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }

  rs = GenerateParenthses(4);
  LOG(INFO) << rs.size();
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }
  rs = GenerateParenthses(6);
  LOG(INFO) << rs.size();
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }
  rs = official::GenerateParenthses(6);
  LOG(INFO) << rs.size();
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }
  return 0;
}

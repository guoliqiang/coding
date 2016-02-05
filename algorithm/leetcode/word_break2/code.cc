// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 16:33:48
// File  : code.cc
// Brief :

/*
Given a string s and a dictionary of words dict, add spaces in s to construct a sentence where each word is a valid dictionary word.

Return all such possible sentences.

For example, given
s = "catsanddog",
dict = ["cat", "cats", "and", "sand", "dog"].

A solution is ["cats and dog", "cat sand dog"].
*/

/*
 *
 * 24 / 24 test cases passed.
 * Status: Accepted
 * Runtime: 28 ms
 * Submitted: 0 minutes ago
 *
 * */

#include "base/public/common_head.h"
#include <unordered_set>

namespace algorithm {

void DP(const std::string & s, std::unordered_set<std::string> & dict,
        std::vector<bool> & dp, int len) {
  for (int i = 0; i < s.size(); i++) {
    for (int j = i; j >= std::max(0, i - len - 1); j--) {
      const std::string & t = s.substr(j, i - j + 1);
      if (j - 1 >= 0) dp[i] = dict.count(t) ? dp[j - 1] : false;
      else dp[i] = dict.count(t) ? true : false;
      if (dp[i]) break;
    }
  }
}

void Trace(const std::string & s, std::vector<bool> & dp,
           int k, int len,
           std::vector<std::string> & path,
           std::vector<std::string> & rs,
           std::unordered_set<std::string> & dict) {
  if (k == -1) {
    std::string t = "";
    for (int i = path.size() - 1; i >= 0; i--) {
      if (!t.empty()) t.append(" ");
      t.append(path[i]);
    }
    rs.push_back(t);
  } else {
    if (dp[k] == false) return;
    for (int i = k; i >= std::max(0, k - len + 1); i--) {
      std::string t = s.substr(i, k - i + 1);
      if (dict.count(t) && ((i != 0 && dp[i - 1]) || i == 0)) {
        path.push_back(t);
        Trace(s, dp, i - 1, len, path, rs, dict);
        path.pop_back();
      }
    }
  }
}

std::vector<std::string> WordBreak(std::string & s, std::unordered_set<std::string> & dict) {
  int len = 0;
  for (std::unordered_set<std::string>::iterator i = dict.begin(); i != dict.end();
       i++) {
    len = std::max(len, (int)(i->size()));
  }
  std::vector<bool> dp(s.size(), false);
  DP(s, dict, dp, len);
  std::vector<std::string> rs;
  std::vector<std::string> path;
  Trace(s, dp, s.size() - 1, len, path, rs, dict);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

// 会超内存
namespace DP {
vector<string> wordBreak(string s, unordered_set<string>& wordDict) {
        int len = 0;
        for (unordered_set<string>::iterator i = wordDict.begin(); i != wordDict.end(); i++) {
            len = std::max(len, (int)(i->size()));
        }
        int n = s.size();
        if (n == 0) return std::vector<std::string>();
        std::vector<std::vector<std::string> > dp(n, std::vector<std::string>());
        
        for (int i = 0; i < n; i++) {
            for (int j = std::max(0, i - len + 1); j <= i; j++) {
                std::string cur = s.substr(j, i - j + 1);
                
                if (wordDict.count(cur)) {
                    if (j == 0) dp[i].push_back(cur);
                    else {
                        for (int k = 0; k < dp[j - 1].size(); k++) {
                            std::string t = dp[j - 1][k] + " " + cur;
                            dp[i].push_back(t);
                        }
                    }
                }
            }
        }
        return dp[n - 1];
    }
}  // namespace DP

int main(int argc, char** argv) {
  std::string s = "catsanddog";
  std::unordered_set<std::string> dict;
  dict.insert("cat");
  dict.insert("cats");
  dict.insert("and");
  dict.insert("sand");
  dict.insert("dog");
  std::vector<std::string> rs = WordBreak(s, dict);
  LOG(INFO) << JoinVector(rs);
  return 0;
}

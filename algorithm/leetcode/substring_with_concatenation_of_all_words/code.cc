// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-23 17:09:29
// File  : code.cc
// Brief :
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include "base/public/logging.h"
#include "base/public/string_util.h"

/*
 * time limited
 * 每个 i 都需要简历i-1的map，时间消耗大
 *
 * */

namespace algorithm {
void DP(std::string & str, std::map<std::string, int> & l, std::vector<int> &dp) {
  int size = str.size();
  int w_size = l.begin()->first.size();
  for (int i = 0; i < w_size - 1; i++) dp[i] = 0;
  if (l.count(str.substr(0, w_size))) dp[w_size - 1] = 1;

  for (int i = w_size; i < size; i++) {
    std::string tmp = str.substr(i - w_size + 1, w_size);
    // LOG(INFO) << "tmp:" << tmp;
    if (!l.count(tmp)) {
      dp[i] = 0;
    } else {
      if (dp[i - w_size] != 0) {
        dp[i] = 1;
        int num = dp[i - w_size];
        std::map<std::string, int> foo;
        foo[tmp] = 1;
        while (num) {
          // LOG(INFO) << "tmp:" << tmp << " " << (i - w_size) << " "
          //           << dp[i - w_size] - num + 1 << " num:" << num;
          // LOG(INFO) << "tmp:" << tmp << " "
          //           << (i - w_size) - (dp[i - w_size] - num + 1) * w_size
          //           << " " << w_size;
          std::string one = str.substr((i - w_size) - (dp[i - w_size] - num + 1) * w_size + 1, w_size);
          // LOG(INFO) << "one:" << one;
          if (foo.count(one) && foo[one] == l[one]) {
            // LOG(INFO) << "find one:" << one << " break "
            //           << foo.size() << " " << l.size()
            //           << foo[one] << " " << l[one];
            break;
          } else {
            dp[i]++;
            num--;
            if (foo.count(one)) foo[one]++;
            else foo[one] = 1;
          }
        }
        // LOG(INFO) << "dp[" << i << "]:" << dp[i];
      } else {
        dp[i] = 1;
      }
    }
  }
}

std::vector<int> FindSubString(std::string & str, std::vector<std::string> & v) {
  std::map<std::string, int> map_v;
  std::vector<int> rs;
  if (v.size() == 0 || str.size() == 0) return rs;
  for (int i = 0; i < v.size(); i++) {
    if (map_v.count(v[i])) map_v[v[i]]++;
    else map_v[v[i]] = 1;
  }
  std::vector<int> dp(str.size(), 0);
  DP(str, map_v, dp);
  for (int i = 0; i < dp.size(); i++) {
    if (dp[i] == v.size()) rs.push_back(i - v.size() * v.front().size() + 1);
  }
  return rs;
}

}  // namespace algorithm

namespace other {
/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 18/18 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 1888 milli secs
 * Progress: 160/160 test cases passed.
 *
 * */

std::vector<int> FindSubString(std::string &str, std::vector<std::string> & v) {
  std::map<std::string, int> lexical;
  std::map<std::string, int> foo;
  std::vector<int> rs;
  for (int i = 0; i < v.size(); i++) {
    if (lexical.count(v[i])) lexical[v[i]] ++;
    else lexical[v[i]] = 1;
  }
  int w_size = v[0].size();
  int size = v.size();
  if (size * w_size > str.size()) return rs;
  for (int i = 0; i <= str.size() - w_size * size; i++) {
    foo.clear();
    int j = 0;
    for (j = 0; j < size; j++) {
      std::string word = str.substr(i + j * w_size, w_size);
      if (!lexical.count(word)) break;
      else {
        if (foo.count(word)) foo[word]++;
        else foo[word] = 1;
        if (foo[word] > lexical[word]) break;
      }
    }
    if (j == size) rs.push_back(i);
  }
  return rs;
}
}  // namespace other

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 18/18 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 196 milli secs
 * Progress: 160/160 test cases passed.
 * */

namespace best {
std::vector<int> FindSubString(std::string & str, std::vector<std::string> & v) {
  std::map<std::string, int> str_count;
  std::map<std::string, int> foo;
  int sum_len = 0;
  std::vector<int> rs;
  if (str.size() == 0 || v.size() == 0) return rs;

  for (int i = 0; i < v.size(); i++) {
    if (str_count.count(v[i])) str_count[v[i]]++;
    else str_count[v[i]] = 1;
    sum_len += v[i].size();
  }

  int w_size = v[0].size();
  // for v element size is fixed
  for (int i = 0; i < w_size; i++) {
    int begin = i;
    int end = i;
    int count = 0;
    for (int k = 0; k < v.size(); k++) foo[v[k]] = 0;
    
    while (end < str.size()) {
      std::string word = str.substr(end, w_size);
      // LOG(INFO) << "word:" << word << " end:" << end;
      if (!foo.count(word)) {
        end += w_size;
        continue;
      }
      if (foo[word] < str_count[word]) count++;
      foo[word]++;
      if (count == v.size()) {
        while (begin < end) {
          std::string w = str.substr(begin, w_size);
          if (!foo.count(w)) {
            begin += w_size;
            continue;
          }
          if (foo[w] > str_count[w]) {
            foo[w]--;
            begin += w_size;
          } else {
            break;
          }
        }
        // LOG(INFO) << begin  << " " << end;
        if (end - begin + w_size == sum_len) rs.push_back(begin);
      }
      end += w_size;
    }
  }
  return rs;
}
}  // namespace best

namespace twice {
using namespace std;
class Solution {
public:
    vector<int> findSubstring(string S, vector<string> &L) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        std::vector<int> rs;
        std::map<std::string, int> tmp;
        std::map<std::string, int> dict;
        for (int i = 0; i < L.size(); i++) {
            if (dict.count(L[i]))dict[L[i]]++;
            else dict[L[i]] = 1;
        }
        
        int m = L.size() > 0 ? L[0].size(): 0;
        if (S.size() < m) return rs;
        for (int k = 0; k < m; k++) {
            int b = k;
            int e = k;
            for (int i = 0; i < L.size(); i++) {
              tmp[L[i]] = 0;
            }
            int cnt = 0;
            while (e <= S.size() - m) {
              std::string t = S.substr(e, m);
              if (tmp.count(t)) {
                  if (tmp[t] < dict[t]) cnt++;
                  tmp[t] += 1;
              };
              if (cnt == L.size()) {
                  while(true) {
                      std::string sub = S.substr(b, m);
                      if (!tmp.count(sub)) b += m;
                      else if (tmp[sub] > dict[sub]) {
                          b += m;
                          tmp[sub]--;
                      } else break;
                  }
                  if (e - b + m== L.size() * m) rs.push_back(b);
              }
              e += m;
            }
        }
        return rs;
    }
};
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::string foo = "barfoothefoobarman";
  std::vector<std::string> v;
  v.push_back("foo");
  v.push_back("bar");
  std::vector<int> rs = FindSubString(foo, v);
  LOG(INFO) << JoinVector(rs);
  
  rs = other::FindSubString(foo, v);
  LOG(INFO) << JoinVector(rs);
  rs = best::FindSubString(foo, v);
  LOG(INFO) << JoinVector(rs);
  return 0;
}

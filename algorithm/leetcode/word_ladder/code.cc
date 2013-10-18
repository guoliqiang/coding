// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-01 14:01:59
// File  : code.cc
// Brief :

#include <unordered_set>
#include "base/public/common_head.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 1540 milli secs
 * Progress: 37/37 test cases passed.
 *
 * BFS 和 dijastra都会T
 *
 * 反而直接查能过Large Test
 * */

namespace NB {

using namespace std;

int LadderLength(string start, string end, unordered_set<string> &dict) {
  int ret = 0;
  if (start == end) return ret;
  unordered_set<string> visited;
  queue<string> que;
  int lev1 = 1;
  int lev2 = 0;
  
  que.push(start);
  visited.insert(start);

  while (!que.empty()) {
    string s = que.front();
    que.pop();
    lev1--;

    for (int i = 0; i < s.length(); i++) {
      for (int j = 0; j < 26; ++j) {
        string t = s;
        t[i] = 'a' + j;
        // 因为每条边的权值相当于1,所以可以直接返回
        // 否则需要比较大小
        if (t == end) return ret + 2;
        if (dict.find(t) != dict.end() &&
            visited.find(t) == visited.end()) {
           que.push(t);
           visited.insert(t);
           lev2++;
         }
       }
     }
     if (lev1 == 0) {
        ret++;
        lev1 = lev2;
        lev2 = 0;
     }
   }
   return 0;
}

}  // namespace NB

namespace algorithm {
class BitSet {
 public:
  BitSet(int len) : len_(len) {
    ptr_ = new char[len_ / 8 + 1];
    memset(ptr_, 0, sizeof(char) * (len_ / 8 + 1));
  }

  bool IsSet(int i) {
    char * p = ptr_ + (i / 8);
    char foo = (1 << (i % 8));
    return (foo & (*p));
  }

  void Set(int i) {
    char * p = ptr_ + (i / 8);
    char foo = (1 << (i % 8));
    (*p) |= foo;
  }

  void Clear(int i) {
    char * p = ptr_ + (i / 8);
    char foo = ~(1 << (i % 8));
    (*p) &= foo;
  }

  ~BitSet() {
    delete [] ptr_;
  }

 private:
  int len_;
  char * ptr_;
 
 private:
  BitSet(const BitSet &) {}
  void operator = (const BitSet &) {}
};


bool Ok(std::string & s1, std::string & s2) {
  int flag = false;
  for (int i = 0; i < s1.size(); i++) {
    if (s1[i] != s2[i] && flag) return false;
    if (s1[i] != s2[i] && !flag) flag = true;
  }
  return flag;
}

int Dijastra(std::vector<std::string> & v, BitSet & bs) {
  std::vector<int> d(v.size(), -1);
  
  int min = 0;
  d[0] = 0;
  std::set<int> used;

   while (used.size() < v.size() && min != -1) {
     for (int i = 0; i < v.size(); i ++) {
       if (bs.IsSet(min * v.size() + i) && (d[i] == -1 || d[i] > d[min] + 1)) {
         d[i] = d[min] + 1;
       }
     }
     if (d[v.size() - 1] != -1) break;
     // LOG(INFO) << "d:" << JoinVector(d);
     used.insert(min);
     min = -1;
     for (int i = 1; i < d.size(); i++) {
       if (used.count(i)) continue;
       if (min == -1 && d[i] != -1) min = i;
       else if (d[i] < d[min] && d[i] != -1) {
         min = i;
       }
     }
  }
  return d[v.size() - 1] == -1 ? 0 : d[v.size() - 1] + 1;
}

// 也会T

int Dijastra(std::vector<std::string> & v) {
  BitSet bs(v.size() * v.size());
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      if (Ok(v[i], v[j])) {
        bs.Set(i * v.size() + j);
        bs.Set(j * v.size() + i);
      }
    }
  }
  return Dijastra(v, bs);
}

int DP(std::vector<std::string> & v) {
  std::vector<int> dp(v.size(), -1);
  std::vector<std::vector<int> > foo(2, std::vector<int>());
  BitSet bs(v.size() * v.size());
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      if (Ok(v[i], v[j])) {
        bs.Set(i * v.size() + j);
        bs.Set(j * v.size() + i);
      }
    }
  }
  bool flag = false;
  dp[0] = 0;
  foo[flag].push_back(0);
  
  while (dp[v.size() - 1] == -1) {
    foo[!flag].clear();
    bool find = false;
    for (int i = 0; i < foo[flag].size(); i++) {
      for (int j = 0; j < v.size(); j++) {
        if (bs.IsSet(foo[flag][i]* v.size() + j) &&
            (dp[j] == -1 || dp[j] > dp[foo[flag][i]] + 1)) {
          dp[j] = dp[foo[flag][i]] + 1;
          foo[!flag].push_back(j);
          find = true;
          LOG(INFO) << "dp[" << v[j] << "]:" << dp[j];
        }
      }
    }
    flag = !flag;
    if (!find) break;
  }
  return dp[v.size() - 1] == -1 ? 0 : dp[v.size() - 1] + 1;
}

int LadderLength(std::string & start, std::string & end, std::unordered_set<std::string> & dict) {
  std::vector<std::string> tdict;
  for (std::unordered_set<std::string>::iterator i = dict.begin(); i != dict.end();
       i++) {
    if (*i != start && *i != end) tdict.push_back(*i);
  }
  tdict.insert(tdict.begin(), start);
  tdict.insert(tdict.end(), end);
  return Dijastra(tdict);
  // return DP(tdict);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string start = "hit";
  std::string end = "tmm";
  std::unordered_set<std::string> dict;
  dict.insert("hot");
  dict.insert("dot");
  dict.insert("dog");
  dict.insert("lot");
  dict.insert("log");
  LOG(INFO) <<  LadderLength(start, end, dict);
  return 0;
}

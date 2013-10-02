// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 00:20:23
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

/*
 * DP 会 T原因是计算了太多最后不用的中间结果
 * 比如 klaaaaabs
 * klaaaaa的中间结果是用不到的
 *  
 *
 * 此题目应该用一维DP而不是二维DP
 * */

struct Node {
  void Push(const Node & p, const std::string & s) {
    for (int i = 0; i < p.v.size(); i++) {
      std::vector<std::string> foo = p.v[i];
      foo.push_back(s);
      v.push_back(foo);
    }
  }
  void Push(const std::string & s) {
    std::vector<std::string> foo;
    foo.push_back(s);
    v.push_back(foo);
  }
  std::vector<std::vector<std::string> > v;
};

bool Palindrome(const std::string & s, int i, std::vector<std::vector<bool> > & palin) {
  if (s.size() <= 1) return true;
  int b = 0;
  int e = s.size() - 1;

  if (s.size() == 2) {
    if (s[e] == s[b]) palin[i][i + e] = true;
    else palin[i][i + e] = false;
    return palin[i][i + e];
  }

  if (s[b] == s[e] && palin[i + 1][i + e - 1]) {
    palin[i][i + e] = true;
    return true;
  } else {
    palin[i][i + e] = false;
    return false;
  }

  while (b <= e) {
    if (s[b] != s[e]) return false;
    else {
      b++;
      e--;
    }
  }
  return true;
}

bool Palindrome(const std::string & s) {
  if (s.size() <= 1) return true;
  int b = 0;
  int e = s.size() - 1;

  while (b <= e) {
    if (s[b] != s[e]) return false;
    else {
      b++;
      e--;
    }
  }
  return true;
}


void DP(std::string & str, std::vector<std::vector<Node> > & dp) {
  int n = str.size();
  std::vector<std::vector<bool> > palin(n, std::vector<bool>(n, false));
  for (int i = 0; i < n; i++) {
    dp[i][i].Push(str.substr(i, 1));
    palin[i][i] = true;
  }
  
  for (int k = 1; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      for (int j = i + k; j >= i; j--) {
        std::string t = str.substr(j, i + k - j + 1);
        if(Palindrome(t, j, palin)) {
          if (j == i) dp[i][j + k].Push(t);
          else dp[i][i + k].Push(dp[i][j - 1], t);
        }
      }  // for
    }  // for
  }  // for
}

std::vector<std::vector<std::string> > Partition(std::string & str) {
  int n = str.size();
  std::vector<std::vector<Node> > dp(n, std::vector<Node>(n, Node()));
  DP(str, dp);
  return dp[0][n - 1].v;

}

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 184 milli secs
 * Progress: 20/20 test cases passed.
 *
 * trick:http://blog.sina.com.cn/s/blog_b9285de20101iwqt.html
 * */

void DP(std::string & str, std::vector<Node> & dp) {
  dp[0].Push(str.substr(0, 1));
  std::vector<std::vector<bool> > tdp(str.size(), std::vector<bool>(str.size(), false));
  for (int i = 1; i < str.size(); i++) {
    for (int j = i; j >= 0; j--) {
      std::string t = str.substr(j, i - j + 1);
      // if (Palindrome(t)) {
      // 加速搜索回文串，这个trick可以处理T的问题,但使用二维DP即使加速后
      // 依旧会T
      if (t.size() == 1 ||
          (t.size() == 2 && t[0] == t[1]) ||
          (t[0] == t[t.size() - 1] && tdp[j + 1][i - 1])) {
        tdp[j][i] = true;
        if (j == 0) dp[i].Push(t);
        else {
          dp[i].Push(dp[j - 1], t);
        }
      }
    }
  }
}

std::vector<std::vector<std::string> > Partition2(std::string & str) {
  int n = str.size();
  std::vector<Node> dp(n, Node());
  DP(str, dp);
  return dp[n - 1].v;
}



/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 108 milli secs
 * Progress: 20/20 test cases passed.
 * */

void DFS(const std::string & str, std::vector<std::string> & path,
         std::vector<std::vector<std::string> > & rs) {
  if (str.size() == 0) {
    rs.push_back(path);
  } else {
    for (int i = 0; i < str.size(); i++) {
      std::string t= str.substr(0, i + 1);
      if (Palindrome(t)) {
        path.push_back(t);
        DFS(str.substr(i + 1), path, rs);
        path.pop_back();
      }
    }
  }
}

std::vector<std::vector<std::string> > DFS(const std::string & str) {
  std::vector<std::vector<std::string> > rs;
  std::vector<std::string> path;
  DFS(str, path, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "abbab";
  std::vector<std::vector<std::string> > rs = Partition(str);
  LOG(INFO) << JoinMatrix(&rs);
  rs = DFS(str);
  LOG(INFO) << JoinMatrix(&rs);
  rs = Partition2(str);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}

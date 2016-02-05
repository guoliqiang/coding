// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 23:46:35
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<int> DiffWaysToCompute(const std::string & input) {
  std::vector<int> data;
  std::vector<char> op;
  int cur = 0;
  for (int i = 0; i < input.size(); i++) {
    if (isdigit(input[i])) {
      cur = cur * 10 + input[i] - '0';
    } else {
      data.push_back(cur);
      cur = 0;
      op.push_back(input[i]);
    }
  }
  data.push_back(cur);
  int n = data.size();

  std::vector<std::vector<std::vector<int> > > dp(n,
      std::vector<std::vector<int> >(n, std::vector<int>()));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j + i < n; j++) {
      if (i == 0) dp[j][j + i].push_back(data[j]);
      else {
        for (int k = j; k < j + i; k++) {
          std::vector<int> & a = dp[j][k];
          std::vector<int> & b = dp[k + 1][j + i];
          for (int m = 0; m < a.size(); m++) {
            for (int n = 0; n < b.size(); n++) {
              if (op[k] == '+') dp[j][j + i].push_back(a[m] + b[n]);
              if (op[k] == '-') dp[j][j + i].push_back(a[m] - b[n]);
              if (op[k] == '*') dp[j][j + i].push_back(a[m] * b[n]);
            }
          }
        }
      }
    }
  }
  return dp[0][n - 1];
}
}  // namespace algorithm

namespace recursive {

std::vector<int> Get(std::vector<int> & num, std::vector<char> & op, int b, int e) {
  if (b == e) return std::vector<int>(1, num[b]);
  else {
    std::vector<int> ans;
    for (int i = b; i < e; i++) {
      std::vector<int> l = Get(num, op, b, i);
      std::vector<int> r = Get(num, op, i + 1, e);
      for (int k1 = 0; k1 < l.size(); k1++) {
        for (int k2 = 0; k2 < r.size(); k2++) {
          if (op[i] == '+') ans.push_back(l[k1] + r[k2]);
          else if (op[i] == '-') ans.push_back(l[k1] - r[k2]);
          else ans.push_back(l[k1] * r[k2]);
        }
      }
    }
    return ans;
  }
}

int StringToInt(const std::string & str) {
  return atoi(str.c_str());
}

std::vector<int> DiffWaysToCompute(std::string input) {
  int n = input.size();
  if (n == 0) return std::vector<int>();

  std::vector<int> num;
  std::vector<char> op;
  std::string cur;
  for (int i = 0; i < n; i++) {
    if (input[i] == ' ') {
      if (cur.size()) num.push_back(StringToInt(cur));
      cur.clear();
    } else if (input[i] == '+' || input[i] == '-' || input[i] == '*') {
      if (cur.size()) num.push_back(StringToInt(cur));
      cur.clear();
      op.push_back(input[i]);
    } else {
      cur.push_back(input[i]);
    }
  }
  if (cur.size()) num.push_back(StringToInt(cur));
  if (num.size() == 0) return std::vector<int>();
  if (op.size() == 0) return std::vector<int>(1, num[0]);

  return Get(num, op, 0, num.size() - 1);
}
}  // namespace recursive

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << JoinVector(recursive::DiffWaysToCompute("2*3-4*5"));
  return 0;
}

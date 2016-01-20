// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 14:03:36
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// 也可以用str(a)+str(b)与str(b)+str(a)比较
bool Cmp(int ia, int ib) {
  int a = ia;
  int b = ib;
  std::vector<int> vec_a;
  std::vector<int> vec_b;
  do {
    vec_a.push_back(a % 10);
    a /= 10;
  } while(a != 0);

  do {
    vec_b.push_back(b % 10);
    b /= 10;
  } while(b != 0);

  long long int tmp_a = ia;
  for (int i = vec_b.size() - 1; i >= 0; i--) {
    tmp_a = tmp_a * 10 + vec_b[i];
  }
  long long int tmp_b = ib;
  for (int i = vec_a.size() - 1; i >= 0; i--) {
    tmp_b = tmp_b * 10 + vec_a[i];
  }
  return tmp_a > tmp_b;
}

std::string ToString(int n) {
  std::string ans;
  do {
    ans.insert(ans.begin(), '0' + n % 10);
    n /= 10;
  } while(n > 0);
  return ans;
}

std::string LargestNumber(std::vector<int> nums) {
  std::sort(nums.begin(), nums.end(), Cmp);
  if (nums.size() == 0 || nums[0] == 0) return "0";
  std::string ans;
  for (int  i = 0; i < nums.size(); i++) {
    ans.append(ToString(nums[i]));
  }
  return ans;
}
}  // namespace algorithm

namespace twice {
using namespace std;

std::string IntToString(int v) {
  char buff[100] = {0};
  sprintf(buff, "%d", v);
  return std::string(buff);
}

bool Cmp(int a, int b) {
  std::string s1 = IntToString(a) + IntToString(b);
  std::string s2 = IntToString(b) + IntToString(a);
  return s1 > s2;
}

class Solution {
  public:
    string largestNumber(vector<int>& nums) {
      std::sort(nums.begin(), nums.end(), Cmp);
      std::string ans;
      for (int i = 0; i < nums.size(); i++) {
        ans += IntToString(nums[i]);
      }
      int idx = 0;
      while (idx < ans.size() && ans[idx] == '0') idx++;
      if (idx == ans.size()) return "0";
      else return ans.substr(idx);
    }
};
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(3);
  vec.push_back(30);
  vec.push_back(34);
  vec.push_back(9);
  vec.push_back(5);
  LOG(INFO) << LargestNumber(vec);
  return 0;
}

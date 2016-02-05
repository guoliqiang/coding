// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 07:50:53
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::string FractionToDecimal(long long int num, long long int den) {
  std::string prefix = "";
  std::string suffix = "";
  bool negative = false;
  if (num * den < 0) negative = true;
  num = fabs(num);
  den = fabs(den);

  long long int cur = num / den;
  int tmp = cur;
  LOG(INFO) << cur << " " << tmp;
  num %= den;
  do {
    prefix.insert(prefix.begin(), '0' + (cur % 10));
    cur /= 10;
  } while (cur > 0);

  std::map<long long int, int> v_idx;
  int cnt = 0;
  while (num > 0) {
    if (v_idx.count(num)) {
      suffix.insert(suffix.begin() + v_idx[num], '(');
      suffix.insert(suffix.end(), ')');
      break;
    }
    v_idx[num] = cnt++;

    num *= 10;
    cur = num / den;
    if (num >= den) num %= den;
    suffix.insert(suffix.end(), '0' + cur);
  }
  std::string rs = suffix.size() > 0 ? prefix + "." + suffix : prefix;
  return negative ? "-" + rs : rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace clear {

std::string IntToString(long long int v) {
    char buff[100] = { 0 };
    sprintf(buff, "%lld", v);
    return std::string(buff);
}

std::string FractionToDecimal(int numerator, int denominator) {
  long long int d1 = numerator;
  long long int d2 = denominator;
  bool minus = d1 * d2 < 0 ? true : false;
  d1 = fabs(d1);
  d2 = fabs(d2);

  std::string ans = IntToString(d1 / d2);
  if (minus) ans = "-" + ans;

  long long int left = d1 % d2;
  if (left == 0) return ans;

  std::map<int, int> idx;
  std::string tail;
  while (left > 0) {
      left *= 10;
      if (idx.count(left)) {
          tail.insert(tail.begin() + idx[left], '(');
          tail.push_back(')');
          return ans + "." + tail;
      }
      tail.push_back('0' + left / d2);
      idx[left] = tail.size() - 1;
      left %= d2;
  }
  return ans + "." + tail;
}
}  // namespace clear

int main(int argc, char** argv) {
  LOG(INFO) << FractionToDecimal(-2147483648, 1);
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 12:43:21
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 24/24 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

std::string Space(int n) {
  return std::string(n, ' ');
}

void FullJustify(std::vector<std::string> & v, int b, int l,
                 std::vector<std::string> *rs) {
  if (b >= v.size()) return;
  int e = b - 1;
  int sum = 0;
  // Note this loop is difficult
  // while (A && B);
  // 这样的循环 如果接下来要用到跳出的状态，
  // 跳出时应 ！A 或 !B  最好不要出现 ！A && ！B
  while (sum < l) {
    e++;
    if (e == v.size()) break;
    if (sum == 0) sum += v[e].size();
    else sum += (v[e].size() + 1);
  }
  // LOG(INFO) << "b:" << b << " e: " << e;
  
  if (e == v.size() || sum == l) {  // sum <= l
    std::string tmp = "";
    for (int i = b; i <= std::min(e, (int)v.size() - 1); i++) {
      if (i == b) tmp += v[i];
      else tmp += (Space(1) + v[i]);
    }
    if (sum < l) tmp += Space(l - sum);
    rs->push_back(tmp);
    if (sum == l) FullJustify(v, e + 1, l, rs);
  } else {  // sum > l
    sum -= (v[e].size() + 1);
    e--;
    // note e may equal with b
    int k = (e == b) ? 0 : (l - sum) / (e - b);
    int left = (e == b) ? (l - sum) : (l - sum) % (e - b);
    // LOG(INFO) << "k:" << k << " left:" << left;
    std::string tmp = "";
    for (int i = b; i<= e; i++) {
      if (i == b) tmp += v[i];
      else {
        if (left) {
          tmp += Space(1);
          left--;
        }
        tmp += (Space(k + 1) + v[i]);
      }
    }
    if (left) tmp += Space(left);
    rs->push_back(tmp);
    FullJustify(v, e + 1, l, rs);
  }
}

std::vector<std::string> FullJustify(std::vector<std::string> & words, int l) {
  if (l == 0) return words;
  std::vector<std::string> rs;
  FullJustify(words, 0, l, &rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> words;
  words.push_back("This");
  words.push_back("is");
  words.push_back("an");
  words.push_back("example");
  words.push_back("of");
  words.push_back("text");
  words.push_back("justification");
  int l = 16;
  std::vector<std::string> rs = FullJustify(words, l);

  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }
  return 0;
}

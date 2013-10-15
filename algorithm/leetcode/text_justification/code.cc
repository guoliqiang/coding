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

namespace twice {
using namespace std;
    vector<string> FullJustify(vector<string> &words, int L) {
        if (L == 0) return words;
        std::vector<std::string> rs;
        int i = 0;
        while (i < words.size()) {
            int len = 0;
            int b = i;
            while (i < words.size() && len + words[i].size() + (b == i ? 0 : 1) <= L) {
                len += words[i].size() + (b == i ? 0 : 1);
                i++;
            }
            
            int space = 0;
            int left = 0;
            if (i == words.size()) {
                space = 0;
                left = L - len;
            } else {
                space = (i - b == 1) ? 0 : (L - len) / (i - b - 1);
                left = (i - b == 1) ? L - len : (L - len) % (i - b - 1);
            }
            std::string foo = "";
            for (int j = b; j < i; j ++) {
                if (j == b) {
                    foo.append(words[j]);
                } else {
                    if (left && i != words.size()) {  // bug fixed
                        foo.append(" ");
                        left--;
                    }
                    for (int k = 0; k < space; k++) foo.append(" ");
                    foo.append(" ");
                    foo.append(words[j]);
                }
            }
            if (i == words.size() || i  == b + 1) {
                for (int k = 0; k < left; k++) foo.append(" ");
            };
            rs.push_back(foo);
        }
        return rs;
    }
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<std::string> words;
  words.push_back("What");
  words.push_back("must");
  words.push_back("be");
  words.push_back("shall");
  words.push_back("be.");
  int l = 12;
  std::vector<std::string> rs = twice::FullJustify(words, l);

  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }
  return 0;
}

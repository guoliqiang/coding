// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-03-10 02:18:54
// File  : code.cc
// Brief :

/*

22 / 22 test cases passed.
Status: Accepted
Runtime: 44 ms
Submitted: 0 minutes ago

*/

#include "base/public/common_ojhead.h"

namespace algorithm {

std::string Reverse(const std::string & str) {
  std::string rs = "";
  int b = 0;
  while (b < str.size()) {
    while (str[b] == ' ' || str[b] == '\t') b++;
    if (b == str.size()) break;
    int e = b;
    while (str[e] != ' ' && str[e] != '\t' && e < str.size()) e++;
    if (rs.size() != 0) rs.insert(rs.begin(), 1, ' ');
    rs.insert(rs.begin(), &(str[b]), &(str[e]));
    b = e;
  }
  return rs;
}

}  // namespace algorithm

namespace c_clear {
void Swap(std::string & s, int b, int e) {
    while (b < e) {
        std::swap(s[b++], s[e--]);
    }
}

void ReverseWords(std::string &s) {
    int n = s.size();
    if (n == 0) return;
    Swap(s, 0 , n - 1);

    int b = -1;
    int e = 0;
    while (e < n) {
        if (s[e] == ' ') {
            if (b != -1) {
                Swap(s, b, e - 1);
                b = -1;
            }
        } else {
            if (b == -1) b = e;
        }
        e++;
    }
    if (b != -1) Swap(s, b, n - 1);

    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (s[i] == ' ') {
            if (idx != -1 && s[idx] != ' ') s[++idx] = ' ';
        } else {
            s[++idx] = s[i];
        }
    }
    if (idx == -1) s.clear();
    if (s[idx] == ' ') s.resize(idx);
    else s.resize(idx + 1);
}
}  // namespace c_clear

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::string str = "   the    sky is blue  ";
  LOG(INFO) << str;
  str = Reverse(str);
  LOG(INFO) << str;
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-01 04:06:42
// File  : code.cc
// Brief :

/*
 * Karp-Rabin字符串匹配
 * hash(w[0..m-1]) = (w[0] * 2^(m-1) + ... + w[m-1] * 2^0) % q;
 * hash(w[j+1..j+m]) =   rehash(y[j], y[j+m], hash(w[j..j+m-1]); 
 * rehash(a, b, h) = ((h - a * 2^(m-1) ) * 2 + b) % q;
 * 可以用q = 2^32简化%运算
 *
 * http://blog.chinaunix.net/uid-23629988-id-2983531.html
 * http://igm.univ-mlv.fr/~lecroq/string/node5.html
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

#define REHASH(a, b, h) (((h) - a * d) << 1) + (b)

int KR(const char * pattern, const char * str) {
  int m = strlen(pattern);
  int n = strlen(str);
  /*
   * 不能使用 d << (m - 1)
   * 因为m - 1 > 31时，编译器回用 (m - 1) % 32进行移位操作
   * http://blog.csdn.net/VIPVIPVIP/article/details/6263690
   * */
  int d = 1;
  for (int i = 1; i < m; i++) d <<= 1;
  
  int hp = 0;
  int hs = 0;
  for (int i = 0; i < m; i++) {
    hp = (hp << 1) + pattern[i];
    hs = (hs << 1) + str[i];
  }
  for (int i = 0; i <= n - m; i++) {
    if (hp == hs && memcmp(pattern, str + i, m) == 0) return i;
    hs = REHASH(str[i], str[i + m], hs);
  }
  return -1;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string pattern;
  std::string str;
  pattern = "mmmmmlllllllllllkkkkkkkkkkkkkkllllllllllllppppppppp";
  str = "kskslpppopopmmmmmlllllllllllkkkkkkkkkkkkkkllllllllllllpppppppppllksksksk";
  LOG(INFO) << KR(pattern.c_str(), str.c_str());
  return 0;
}

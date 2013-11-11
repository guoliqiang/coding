// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 15:27:02
// File  : code.cc
// Brief :

/*
 *  Given a (decimal – e.g. 3.72) number that is passed in as a string,
 *  print the binary representation. 
 *  If the number can not be represented accurately in binary, print “ERROR”
 * */

#include "base/public/common_head.h"

namespace algorithm {

std::string Binary(const std::string & str, int size) {
  int p1  = 0;
  double p2 = std::atof(str.c_str());
  std::string rs = "";
  if (str.find('.') != std::string::npos) {
    p1 = std::atoi(str.substr(0, str.find('.')).c_str());
    p2 -= p1;
  } else {
    p1 = p2;
    p2 = 0;
  }
  while (p1) {
    if (p1 % 2) rs = "1" + rs;
    else rs = "0" + rs;
    p1 >>= 1;
  }
  if (rs.size() > size) return "ERROR";
  if (p2) rs += ".";
  while (p2 != 0) {
    // LOG(INFO) << p2;
    p2 *= 2;
    if (p2 >= 1) rs += "1";
    else rs += "0";
    p2 = p2 >= 1 ? p2 - 1 : p2;
    if (rs.size() > size) return "ERROR";
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Binary("8.4", 100); 
  // 由于计算精度原因会产生错误，比如这个case,这个case应该输出ERROR
  // TODO(guoliqiang)
  // 解决方法是小数部分用字符串表示，直接在字符串上进行运算
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 13:01:40
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

std::string num[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", 
                     "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen",
                     "Seventeen", "Eighteen", "Ninteen"};
std::string decade[] = {"", "", "Twenty", "Thirty", "Fourty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninty"};


std::string Read(int n) {
  std::string rs = "";
  int n1 = n % 1000;
  int n2 = n / 1000;
  if (n1) {
    int t = n1 % 100;
    if (t < sizeof(num) / sizeof(std::string))  rs = num[t];
    else rs = decade[t / 10] + " " + num[t % 10];
    t = n1 / 100;
    if (t) rs = num[t] + " Hundred and " + rs;
  }
  if (n2) {
    rs = "Thousand, " + rs;
    int t = n2 % 100;
    if (t < sizeof(num) / sizeof(std::string))  rs = num[t] + " " + rs;
    else rs = decade[t / 10] + " " + num[t % 10] + " " + rs;
    t = n2 / 100;
    if (t) rs = num[t] + " Hundred and " + rs;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Read(90999);
  LOG(INFO) << Read(82);
  LOG(INFO) << Read(12345);
  LOG(INFO) << Read(21345);
  return 0;
}

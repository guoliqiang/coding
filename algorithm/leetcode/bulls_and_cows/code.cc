// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 18:55:25
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string ToString(int v) {
  char buff[100];
  sprintf(buff, "%d", v);
  return std::string(buff);
}

std::string GetHint(const std::string & secret, const std::string & guess) {
  int data[10] = { 0 };
  for (int i = 0; i < secret.size(); i++) {
    data[secret[i] - '0']++;
  }
  int a = 0;
  int b = 0;
  for (int i = 0; i < secret.size(); i++) {
    if (secret[i] == guess[i]) {
      a++;
      data[secret[i] - '0']--;
    }
  }

  for (int i = 0; i < secret.size(); i++) {
    if (secret[i] != guess[i] && data[guess[i] - '0'] > 0) {
      b++;
      data[guess[i] - '0']--;
    }
  }

  return ToString(a) + "A" + ToString(b) + "B";
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << GetHint("1807", "7810");
  LOG(INFO) << GetHint("1123", "0111");
  return 0;
}

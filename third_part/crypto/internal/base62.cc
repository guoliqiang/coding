// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Jian Xu)

#include <string>
#include <cmath>

#include "../public/base62.h"

namespace crypto {

static int kBase = 62;
static std::string kAlphabet = "0123456789abcdefghijklmnopqrstuvwxyz"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool Base62Encode(int64 input, std::string* output) {
  if (NULL == output)
    return false;
  output->assign(Base62Encode(input));
  return (!output->empty());
}

std::string Base62Encode(int64 input) {
  if (input == 0)
    return "0";
  std::string str;
  while (input != 0) {
    int rem = input % kBase;
    input = input / kBase;
    str = kAlphabet[rem] + str;
  }
  return str;
}

bool Base62Decode(const std::string& input, int64 *output) {
  if (input.empty())
    return false;
  *output = Base62Decode(input);
  return (*output != -1);
}

int64 Base62Decode(const std::string& input) {
  int64 num = 0;
  for (int i = 0; i < input.size(); ++i) {
    int power = input.size() - (i + 1);
    size_t index = kAlphabet.find(input[i]);
    if (index == std::string::npos)
      return -1;
    num += index * pow(kBase, power);
  }
  return num;
}

}  // namespace crypto

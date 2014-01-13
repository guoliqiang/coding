// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-11 22:16:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Process(std::string & text, std::string & command, std::string & result) {
  std::list<char> list;
  for (int i = 0; i < text.size(); i++) {
    list.push_back(text[i]);
  }
  std::list<char>::iterator i = list.begin();
  
  int j = 0;
  while (j < command.size()) {
    if (command[j] == '+') i++;
    else if (command[j] == '-') i--;
    else if (command[j] == 'd') i = list.erase(i);
    else if (command[j] == 'a') {
      list.insert(i, command[j + 1]);
      j++;
    }
    j++;
  }
  i = list.begin();
  while (i != list.end()) {
    result.push_back(*i);
    i++;
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string text = "guoliqiang";
  std::string command = "++ajddaj";
  std::string rs = "";
  Process(text, command, rs);
  LOG(INFO) << rs;
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 23:21:07
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 17/17 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 204 milli secs
 * Progress: 100/100 test cases passed.
 *
 * */

#include <vector>
#include <map>
#include <algorithm>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {
std::vector<std::string> Anagrams(std::vector<std::string> & v) {
  std::map<std::string, int> foo;
  std::vector<std::string> rs;
  for (int i = 0; i< v.size(); i++) {
    std::string bar = v[i];
    std::sort(bar.begin(), bar.end());
    if (foo.count(bar)) {
      if (foo[bar] >= 0) {
        rs.push_back(v[foo[bar]]);
        foo[bar] = -1;
      }
      rs.push_back(v[i]);
    } else {
      foo.insert(std::make_pair(bar, i));
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> foo;
  foo.push_back("and");
  foo.push_back("adn");
  foo.push_back("eat");
  foo.push_back("tea");
  foo.push_back("m");
  foo.push_back("");
  foo.push_back("");
  LOG(INFO) << JoinVector(Anagrams(foo));
  return 0;
}

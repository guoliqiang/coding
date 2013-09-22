// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 10:18:50
// File  : code.cc
// Brief :
#include <algorithm>
#include <set>
#include <vector>
#include "base/public/logging.h"
#include "base/public/string_util.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 15/15 test cases passed.
 * 
 * Run Status: Accepted!
 * Program Runtime: 264 milli secs
 * Progress: 282/282 test cases passed.
 *
 * */

namespace algorithm {

void FourSum(std::vector<int> & sum, int target, std::set<std::vector<int> > * rs) {
  rs->clear();
  std::vector<int> quadruplet;
  std::sort(sum.begin(), sum.end());
  for (int i = 0; i < sum.size(); i++) {
    for (int j = i + 1; j < sum.size(); j++) {
      int k = j + 1;
      int l = sum.size() - 1;
      while (k < l) {
        int qsum = sum[i] + sum[j] + sum[k] + sum[l];
        if (qsum < target) k++;
        else if (qsum > target) l--;
        else {
          quadruplet.clear();
          quadruplet.push_back(sum[i]);
          quadruplet.push_back(sum[j]);
          quadruplet.push_back(sum[k]);
          quadruplet.push_back(sum[l]);
          rs->insert(quadruplet);
          k++;
        }
      }
    }
  }
}

std::vector<std::vector<int> > FourSum(std::vector<int> & sum, int target) {
  std::set<std::vector<int> > foo;
  std::vector<std::vector<int> > rs;
  FourSum(sum, target, &foo);
  for (std::set<std::vector<int> >::iterator i = foo.begin(); i != foo.end(); i++) {
    rs.push_back(*i);
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> input;
  int target = 0;
  input.push_back(1);
  input.push_back(0);
  input.push_back(-1);
  input.push_back(0);
  input.push_back(-2);
  input.push_back(2);
  std::vector<std::vector<int> > rs = FourSum(input, target);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << JoinVector(rs[i]);
  }
  return 0;
}

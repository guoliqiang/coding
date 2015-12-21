// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 14:14:56
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::vector<int> FindOrder(int num, std::vector<std::pair<int, int> >& pre) {
  std::vector<int> queue(num, 0);
  std::vector<int> count(num, 0);

  for (int i = 0; i < pre.size(); i++) {
    count[pre[i].first]++;
  }
  int tail = 0;
  for (int i = 0; i < num; i++) {
    if (count[i] == 0) queue[tail++] = i;
  }
  int index = 0;
  while (index < tail) {
    for (int i = 0; i < pre.size(); i++) {
      if (pre[i].second == queue[index]) {
        count[pre[i].first]--;
        if (count[pre[i].first] == 0) {
          queue[tail++] = pre[i].first;
        }
      }
    }
    index++;
  }
  if (tail != num) queue.clear();
  return queue;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(1, 0));
  LOG(INFO) << JoinVector(FindOrder(2, vec));
  return 0;
}

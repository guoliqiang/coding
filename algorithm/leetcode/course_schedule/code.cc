// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 14:14:56
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

bool CanFinish(int num, const std::vector<std::pair<int, int> >& pre) {
  int * count = new int[num];
  int * queue = new int[num];
  memset(count, 0, sizeof(int) * num);
  memset(queue, 0, sizeof(int) * num);

  for (int i = 0; i < pre.size(); i++) {
    count[pre[i].second]++;
  }
  int tail = 0;
  for (int i = 0; i < num; i++) {
    if (count[i] == 0) queue[tail++] = i;
  }
  int index = 0;
  while (index < tail) {
    for (int i = 0; i < pre.size(); i++) {
      if (pre[i].first == queue[index]) {
        count[pre[i].second]--;
        if (count[pre[i].second] == 0) {
          queue[tail++] = pre[i].second;
        }
      }
    }
    index++;
  }
  delete [] count;
  delete [] queue;
  return tail == num;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(0, 1));
  // vec.push_back(std::make_pair(1, 0));
  LOG(INFO) << CanFinish(2, vec);
  return 0;
}

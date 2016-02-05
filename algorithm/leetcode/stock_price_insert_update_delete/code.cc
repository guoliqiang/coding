// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-21 16:43:59
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class Stock {
 public:
  bool Add(int idx, int val) {
    if (data.count(idx)) return false;
    data[idx] = val;
    min.push(std::make_pair(val, idx));
    max.push(std::make_pair(val, idx));
    return true;
  }

  bool Update(int idx, int val) {
    if (!data.count(idx)) return false;
    if (!del.count(idx)) del[idx] = std::map<int, int>();

    int pre = data[idx];
    if (!del[idx].count(pre)) del[idx][pre] = 0;
    del[idx][pre]++;

    data[idx] = val;
    min.push(std::make_pair(val, idx));
    max.push(std::make_pair(val, idx));
    return true;
  }

  bool Delete(int idx) {
    if (!data.count(idx)) return false;

    if (!del.count(idx)) del[idx] = std::map<int, int>();

    int pre = data[idx];
    if (!del[idx].count(pre)) del[idx][pre] = 0;
    del[idx][pre]++;
    data.erase(idx);
    return true;
  }

  int Max() {
    while (true) {
      std::pair<int, int> cur = max.top();
      if (del.count(cur.second) && del[cur.second].count(cur.first) && del[cur.second][cur.first] > 0) {
        del[cur.second][cur.first]--;
        max.pop();
      } else break;
    }
    return max.top().first;
  }

  int Min() {
    while (true) {
      std::pair<int, int> cur = min.top();
      if (del.count(cur.second) && del[cur.second].count(cur.first) && del[cur.second][cur.first] > 0) {
        del[cur.second][cur.first]--;
        min.pop();
      } else break;
    }
    return min.top().first;
  }

 private:
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >,
                      std::less<std::pair<int, int> > > max;
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >,
                      std::greater<std::pair<int, int> > > min;
  std::map<int, std::map<int, int> > del;
  std::map<int, int> data;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Stock foo;
  foo.Add(0, 3);
  foo.Add(1, 2);
  foo.Add(2, 4);
  LOG(INFO) << foo.Max() << " " << foo.Min();
  foo.Delete(0);
  LOG(INFO) << foo.Max() << " " << foo.Min();
  foo.Delete(1);
  LOG(INFO) << foo.Max() << " " << foo.Min();
  return 0;
}

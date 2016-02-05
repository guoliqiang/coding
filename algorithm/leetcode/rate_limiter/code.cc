// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-03 14:24:26
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class RateLimiter {
 private:
  RateLimiter(int range, int limit_num) : range(range), limit_num(limit_num),
      last_check(time(NULL)), available(limit_num) {}

  bool Request() {
    int cur = time(NULL);
    available += (cur - last_check) * (double(limit_num) / range);
    last_check = cur;
    if (available > limit_num) available = limit_num;

    if (available >= 1) {
      available -= 1;
      return true;
    } else {
      return false;
    }
  }

 public:
  int range;
  int limit_num;
  int last_check;
  double available;
};

// 妙极的精度，只能保证任何以整数妙开头的range内不会出现多余limit_num个
class RateLimiter2 {
 private:
  RateLimiter2(int range, int limit_num) : range(range), limit_num(limit_num) {}

  bool Request(int v) {
    int cur = time(NULL);
    int left = cur - range;
    while (queue.size() && queue.front().first <= left) {
      sum -= queue.front().second;
      queue.pop();
    }
    if (sum + v > limit_num) return false;
    else {
      sum += v;
      queue.push(std::make_pair(cur, v));
    }
  }

 public:
  int range;
  int limit_num;
  int sum;
  std::queue<std::pair<int, int> > queue;
};

int dir[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void DFS(int x, int y, std::vector<std::pair<int, int> > & path, std::set<std::pair<int, int> > & visited) {
  path.push_back(std::make_pair(x, y));
  visited.insert(std::make_pair(x, y));
  if (x == 9 && y == 9) {
    LOG(INFO) << JoinVectorP(path);
  } else {
    for (int i = 0; i < 4; i++) {
      int nx = x + dir[i][0];
      int ny = y + dir[i][1];
      if (ny < 0 || ny >= 10 || nx < 0 || nx >= 10) continue;
      if (visited.count(std::make_pair(nx, ny))) continue;
      DFS(nx, ny, path, visited);
    }
  }
  visited.erase(std::make_pair(x, y));
}

void DFS() {
  std::vector<std::pair<int, int> > path;
  std::set<std::pair<int, int> > visited;
  DFS(0, 0, path, visited);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  DFS();
  return 0;
}

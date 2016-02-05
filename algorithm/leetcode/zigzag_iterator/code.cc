// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 21:24:50
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class ZigzagIterator {
 public:
  ZigzagIterator(std::vector<int>& v1, std::vector<int>& v2) {
    vec1 = v1;
    vec2 = v2;
    idx1 = 0;
    idx2 = 0;
    idx = 0;
  }

  int next() {
    if (idx1 == vec1.size()) {
      return vec2[idx2++];
    } else if (idx2 == vec2.size()) {
      return vec1[idx1++];
    } else {
      int ans = 0;
      if (idx == 0) ans = vec1[idx1++];
      else ans= vec2[idx2++];
      idx = (idx + 1) % 2;
      return ans;
    }
  }

  bool hasNext() {
    return idx1 != vec1.size() || idx2 != vec2.size();
  }
  int idx1;
  int idx2;
  int idx;
  std::vector<int> vec1;
  std::vector<int> vec2;
};
}  // namespace algorithm

using namespace algorithm;

namespace NB {
class ZigzagIterator {
 public:
  ZigzagIterator(std::vector<int>& v1, std::vector<int>& v2) {
    if (v1.size()) queue.push(std::make_pair(v1.begin(), v1.end()));
    if (v2.size()) queue.push(std::make_pair(v2.begin(), v2.end()));
  }

  int next() {
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> cur = queue.front();
    queue.pop();
    int ans = *(cur.first);
    cur.first++;
    if (cur.first != cur.second) queue.push(cur);
    return ans;
  }

  bool hasNext() {
    return queue.size() > 0;
  }
  std::queue<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> > queue;
};

}  // namespace NB

namespace NB2 {
class ZigzagIterator {
 public:
  ZigzagIterator(std::vector<int>& v1, std::vector<int>& v2) {
    queue.push(std::make_pair(v1.begin(), v1.end()));
    queue.push(std::make_pair(v2.begin(), v2.end()));
  }

  int next() {
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> cur = queue.front();
    queue.pop();
    int ans = *(cur.first);
    cur.first++;
    queue.push(cur);
    return ans;
  }

  bool hasNext() {
    int size = queue.size();
    for (int i = 0; i < size; i++) {
      std::pair<std::vector<int>::iterator, std::vector<int>::iterator> cur = queue.front();
      if (cur.first == cur.second) {
        queue.pop();
        queue.push(cur);
      } else return true;
    }
    return false;
  }
  std::queue<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> > queue;
};

}  // namespace NB


int main(int argc, char** argv) {
  std::set<std::pair<int, int> > set;
  std::vector<std::pair<int, int> > vec;
  std::pair<int, int> s = std::make_pair(0, 1);
  do {
    
  } while (true)
  return 0;
  std::vector<int> vec1;
  std::vector<int> vec2;
  for (int i = 0; i < 10; i++) vec1.push_back(i);
  for (int i = 10; i < 18; i++) vec2.push_back(i);
  NB2::ZigzagIterator foo(vec1, vec2);
  while (foo.hasNext()) {
    LOG(INFO) << foo.next();
  }
  return 0;
}

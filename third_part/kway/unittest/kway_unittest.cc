// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-19 05:05:15
// File  : kway_unittest.cc
// Brief :

#include <vector>
#include "third_part/kway/public/k_merge_tree.h"
#include "third_part/kway/public/fast_k_merge_tree.h"
#include "base/public/common_head.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/logging.h"
#include "base/public/random.h"

template <typename Container>
class Cmp {
 public:
  typedef typename Container::iterator Iterator;
  bool operator()(
      const std::pair<Iterator, Iterator> & x,
      const std::pair<Iterator, Iterator> & y) const {
     return *(x.first) > *(y.first);
  }
};

void Fake(int m, int n, std::vector<std::vector<int> > * rs) {
  for (int i = 0; i < m; i++) {
    std::vector<int> tmp;
    for (int j = 0; j < n; j++) {
      tmp.push_back(base::Random());
    }
    std::sort(tmp.begin(), tmp.end());
    rs->push_back(tmp);
  }
}

std::vector<std::vector<int> > data;

TEST(Kway, Random) {
  Fake(2, 5, &data);
}

TEST(Kway, PriorityQueue) {
  typedef std::vector<int> Vec;
  std::priority_queue<std::pair<Vec::iterator, Vec::iterator>,
      std::vector<std::pair<Vec::iterator, Vec::iterator> >,
      Cmp<std::vector<int> > > queue;
  for (int i = 0; i < data.size(); i++) {
    queue.push(std::make_pair(data[i].begin(), data[i].end()));
  }
  while (!queue.empty()) {
    std::pair<Vec::iterator, Vec::iterator> tmp = queue.top();
    LOG(INFO) << *tmp.first;
    queue.pop();
    tmp.first++;
    if (tmp.first != tmp.second) queue.push(tmp);
  }
}

class Cmp2 {
 public:
  bool operator() (const int & x, const int & y) {
    return x < y;
  }
};

TEST(Kway, KmergeTree) {
  kway::KmergeTree<int, std::vector<int>::iterator, Cmp2> merge(data.size());
  for (int i = 0; i < data.size(); i++) {
    merge.add(data[i].begin(), data[i].end());
  }

  merge.execute();
  while (true) {
    vector<int>::iterator tmp;
    if (!merge.get_top(tmp)) break;
    LOG(INFO) << *tmp;
    merge.next();
  }
}

TEST(Kway, FastKmergeTree) {
  kway::FastKmergeTree<int, std::vector<int>::iterator, Cmp2> merge;
  for (int i = 0; i < data.size(); i++) {
    merge.Add(data[i].begin(), data[i].end());
  }
  merge.Build();
  while (true) {
    vector<int>::iterator tmp;
    if (!merge.Top(&tmp)) break;
    LOG(INFO) << *tmp;
    merge.Next();
  }
}


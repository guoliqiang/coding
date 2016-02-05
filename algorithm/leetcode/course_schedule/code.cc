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

namespace twice {
bool CanFinish(int numCourses, std::vector<std::pair<int, int> >& prerequisites) {
  int n = numCourses;
  std::map<int, std::set<int> > graph;
  std::vector<int> degree(n, 0);
  for (int i = 0; i < prerequisites.size(); i++) {
    graph[prerequisites[i].first].insert(prerequisites[i].second);
  }

  for (std::map<int, std::set<int> >::iterator i = graph.begin(); i != graph.end(); i++) {
    for (std::set<int>::iterator j = i->second.begin(); j != i->second.end(); j++) {
      degree[*j]++;
    }
  }
  std::queue<int> queue;
  for (int i = 0; i < n; i++) {
    if (degree[i] == 0) queue.push(i);
  }
  int cnt = 0;
  while (queue.size()) {
    int cur = queue.front();
    cnt++;
    queue.pop();
    std::set<int> & neighbors = graph[cur];
    for (std::set<int>::iterator i = neighbors.begin(); i != neighbors.end(); i++) {
      degree[*i]--;
      if (degree[*i] == 0) queue.push(*i);
    }
  }
  return cnt == n;
}
}  // namespace twice

namespace other {
// DFS 找环
// https://leetcode.com/discuss/34791/bfs-topological-sort-and-dfs-finding-cycle-by-c
}  // namespace other

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(0, 1));
  // vec.push_back(std::make_pair(1, 0));
  LOG(INFO) << CanFinish(2, vec);
  return 0;
}

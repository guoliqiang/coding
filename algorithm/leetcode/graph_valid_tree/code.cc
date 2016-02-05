// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 10:37:02
// File  : code.cc
// Brief :

// https://leetcode.com/discuss/82861/share-my-line-dfs-line-bfs-and-clean-union-find-java-solutions

#include "base/public/common_ojhead.h"

namespace algorithm {
int Find(std::vector<int> & vec, int k) {
  while (vec[k] != k) k = vec[k];
  return k;
}

void Union(std::vector<int> & vec, int a, int b) {
  vec[a] = b;
}

bool ValidTree(int n, std::vector<std::pair<int, int> > & pair) {
  std::vector<int> vec(n, 0);
  for (int i = 0; i < n; i++) vec[i] = i;
  int cnt = n;

  for (int i = 0; i < pair.size(); i++) {
    int x = Find(vec, pair[i].first);
    int y = Find(vec, pair[i].second);
    if (x == y) return false;
    Union(vec, x, y);
    cnt--;
  }
  return cnt == 1;
}
}  // namespace algorithm

namespace DFS1 {
bool DFS(std::vector<bool> & visited, int start, std::vector<std::pair<int, int> > & pair) {
  if (visited[start]) return false;
  for (int i = 0; i < pair.size(); i++) {
    if (pair[i].first == start) {
      if (!DFS(visited, pair[i].second, pair)) return false;
    }
  }
  return true;
}
bool ValidTree(int n, std::vector<std::pair<int, int> > & pair) {
  std::vector<bool> visited(n, false);
  if (!DFS(visited, 0, pair)) return false;
  for (int i = 0; i < n; i++) {
    if (!visited[i]) return false;
  }
  return true;
}
}  // namespace DFS1

namespace DFS2 {
bool DFS(std::vector<bool> & visited, int start, int parent, std::map<int, std::set<int> > & graph) {
  if (visited[start]) return false;
  if (graph.count(start)) {
    std::set<int> & t = graph[start];
    for (std::set<int>::iterator i = t.begin(); i != t.end(); i++) {
      if (*i == parent) continue;
      if (!DFS(visited, *i, start, graph)) return false;
    }
  }
  return true;
}

bool ValidTree(int n, std::vector<std::pair<int, int> > & pair) {
  std::vector<bool> visited(n, false);
  std::map<int, std::set<int> > graph;
  for (int i = 0; i < pair.size(); i++) {
    graph[pair[i].first].insert(pair[i].second);
    graph[pair[i].second].insert(pair[i].first);
  }
  if (!DFS(visited, 0, -1, graph)) return false;
  for (int i = 0; i < n; i++) {
    if (!visited[i]) return false;
  }
  return true;
}
}

namespace BFS {
bool ValidTree(int n, std::vector<std::pair<int, int> > & pair) {
  std::map<int, std::set<int> > graph;
  for (int i = 0; i < pair.size(); i++) {
    graph[pair[i].first].insert(pair[i].second);
    graph[pair[i].second].insert(pair[i].first);
  }
  std::vector<int> visited(n, 0);
  std::queue<int> queue;
  queue.push(0);
  visited[0] = 1;
  while (queue.size()) {
    int cur = queue.front();
    queue.pop();
    std::set<int> & t = graph[cur];
    for (std::set<int>::iterator i = t.begin(); i != t.end(); i++) {
      if (visited[*i] == 1) return false;
      if (visited[*i] == 0) {
        queue.push(*i);
        visited[*i] = 1;
      }
    }
    visited[cur] = 2;  // 关键是这行
  }
  for (int i = 0; i < n; i++) {
    if (visited[i] == 0) return false;
  }
  return true;
}
}  // namespace BFS

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(0, 2));
  vec.push_back(std::make_pair(0, 3));
  vec.push_back(std::make_pair(1, 4));
  LOG(INFO) << ValidTree(5, vec);
  vec.clear();
  vec.push_back(std::make_pair(0, 1));
  vec.push_back(std::make_pair(1, 2));
  vec.push_back(std::make_pair(2, 3));
  vec.push_back(std::make_pair(1, 3));
  vec.push_back(std::make_pair(1, 4));
  LOG(INFO) << ValidTree(5, vec);
  return 0;
}

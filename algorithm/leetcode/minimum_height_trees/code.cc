// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-24 21:58:50
// File  : code.cc
// Brief :

// DFS 可以直接求出depth和pre数组
#include "base/public/common_ojhead.h"

namespace algorithm {

std::map<int, std::vector<int> > matrix;

void DFS(int v, int father, int d,
         std::vector<int> & pre, std::vector<int> & depth) {
  pre[v] = father;
  depth[v] = d;
  bool flag = false;
  for (int i = 0; i < matrix[v].size(); i++) {
    if (matrix[v][i] != father) {
      flag = true;
      DFS(matrix[v][i], v, d + 1, pre, depth);
    }
  }
}

std::vector<int> FindMinHeightTrees(int n,
    std::vector<std::pair<int, int> > & edges) {
  matrix.clear();
  std::vector<int> ans;
  if (n == 0) return ans;

  std::vector<std::pair<int, int> > vec;
  for (int i = 0; i < edges.size(); i++) {
    if (!matrix.count(edges[i].first)) {
      matrix[edges[i].first] = std::vector<int>();
    }
    if (!matrix.count(edges[i].second)) {
      matrix[edges[i].second] = std::vector<int>();
    }
    matrix[edges[i].first].push_back(edges[i].second);
    matrix[edges[i].second].push_back(edges[i].first);
  }
  std::vector<int> pre(n, 0);
  std::vector<int> depth(n, 0);
  DFS(0, -1, 0, pre, depth);

  int max = 0;
  for (int i = 1; i < n; i++) {
    if (depth[i] > depth[max]) max = i;
  }

  pre.clear();
  pre.assign(n, 0);
  depth.clear();
  depth.assign(n, 0);

  DFS(max, -1, 0, pre, depth);
  max = 0;
  for (int i = 1; i < n; i++) {
    if (depth[i] > depth[max]) max = i;
  }
  std::vector<int> path;
  while (max != -1) {
    path.push_back(max);
    max = pre[max];
  }
  ans.push_back(path[path.size() / 2]);
  if (path.size() % 2 == 0) {
    ans.push_back(path[path.size() / 2 - 1]);
  }
  return ans;
}

}  // namespace algorithm

namespace NB {
// delete layer by layer
struct Node {
  int v;
  std::set<int> neighbors;
  bool IsLeaf() { return neighbors.size() == 1; }
};

std::vector<int> FindMinHeightTrees(int n,
    std::vector<std::pair<int, int> > & edges) {
  std::vector<Node> graph(n, Node());
  for (int i = 0; i < edges.size(); i++) {
    graph[edges[i].first].neighbors.insert(edges[i].second);
    graph[edges[i].second].neighbors.insert(edges[i].first);
  }

  std::queue<int> queue;
  for (int i = 0; i < n; i++) {
    if (graph[i].IsLeaf()) queue.push(i);
  }
  if (queue.size() == 0) return std::vector<int>(1, 0);

  std::vector<int> ans;
  while (queue.size()) {
    ans.clear();
    int size = queue.size();
    for (int i = 0; i < size; i++) {
      int cur = queue.front();
      queue.pop();
      ans.push_back(cur);
      for (std::set<int>::iterator j = graph[cur].neighbors.begin();
           j != graph[cur].neighbors.end(); j++) {
        graph[*j].neighbors.erase(cur);
        if (graph[*j].IsLeaf()) queue.push(*j);
      }
    }
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  vec.push_back(std::make_pair(1, 0));
  vec.push_back(std::make_pair(1, 2));
  vec.push_back(std::make_pair(1, 3));
  LOG(INFO) << JoinVector(FindMinHeightTrees(4, vec));
  LOG(INFO) << JoinVector(NB::FindMinHeightTrees(4, vec));
  vec.clear();
  vec.push_back(std::make_pair(0, 3));
  vec.push_back(std::make_pair(1, 3));
  vec.push_back(std::make_pair(2, 3));
  vec.push_back(std::make_pair(4, 3));
  vec.push_back(std::make_pair(5, 4));
  LOG(INFO) << JoinVector(FindMinHeightTrees(6, vec));
  LOG(INFO) << JoinVector(NB::FindMinHeightTrees(6, vec));
  return 0;
}


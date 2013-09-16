// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 11:03:39
// File  : max_matching.h
// Brief :
// 1.Hungarian Algorithm
//   http://www.nocow.cn/index.php/%E5%8C%88%E7%89%99%E5%88%A9%E7%AE%97%E6%B3%95
//   O(V*E)
//   边最多
//   最大匹配边的个数 <==> 二分图下的最小覆盖（选出一个覆盖所有边的最小点集合）点的个数
//   proof:http://www.matrix67.com/blog/archives/116
//   找出最小覆盖点的算法
//
//   http://162.105.203.28/course/ada09/students/00748244-YT-HungaryAlgorithm.pdf

// poj1274 poj2159 poj

// 也可以用来求解最小匹配问题（转换成最大匹配问题）
// 扩展 带权重的最大匹配问题

#ifndef  __MAX_MATCHING_H_
#define  __MAX_MATCHING_H_

#include <vector>
#include <algorithm>
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "../base/graph.h"

namespace algorithm {

// has in max_matching
bool InMatching(std::vector<std::vector<int> > & max_matching, int n) {
  for (int i = 0; i < max_matching.size(); i++) {
    if (max_matching[n][i]) return true;
  }
  return false;
}

bool DFS(std::vector<std::vector<int> > & matrix,
         std::vector<std::vector<int> > & max_matching,
         int v, int n, std::vector<int> * path) {
  if (n > matrix.size()) return false;
  if (n == 1 && InMatching(max_matching, v)) return false;
  if (std::find(path->begin(), path->end(), v) != path->end()) return false;

  if (n % 2) {  // find not in max_matching
    for (int i = 0; i < matrix.size(); i++) {
      if (max_matching[v][i] == 0 && matrix[v][i] == 1) {
        path->push_back(v);
        if (!InMatching(max_matching, i)) {
          path->push_back(i);
          return true;
        } else {
          if (DFS(matrix, max_matching, i, n + 1, path)) return true;
          else path->pop_back();
        }
      }
    }
    return false;
  } else {  // find in max_matching
    for (int i = 0; i < matrix.size(); i++) {
      if (max_matching[v][i] == 1 && matrix[v][i] == 1) {
        path->push_back(v);
        if (DFS(matrix, max_matching, i, n + 1, path)) return true;
        else path->pop_back();
      }
    }
    return false;
  }
}

//
// 可以证明，如果一个未批配点在某次增广过程中没有匹配上，在其它
// 增广路径上也不可能会匹配上
//
// return how many matched nodes of left

int Hungarian(std::vector<std::vector<int> > & matrix,
               std::vector<std::vector<int> > * max_matching,
               std::vector<int> & left) {
  // can prof only need to consider each node sequencely.
  int match_count = 0;
  for (int i = 0; i < left.size(); i++) {
    std::vector<int> path;
    if (DFS(matrix, *max_matching, left[i], 1, &path)) {
      match_count++;
      CHECK(path.size() % 2 == 0) << path.size();
      // LOG(INFO) << JoinVector(path);
      for (int i = 0; i < path.size() - 1; i++) {
        int foo1 = path[i];
        int foo2 = path[i + 1];
        if (i % 2 == 0) {
          CHECK((*max_matching)[foo1][foo2] == 0);
          CHECK((*max_matching)[foo2][foo1] == 0);
          SetMatrix(*max_matching, foo1, foo2, 1);
        } else {
          CHECK((*max_matching)[foo1][foo2] == 1);
          CHECK((*max_matching)[foo2][foo1] == 1);
          SetMatrix(*max_matching, foo1, foo2, 0);
        }
      }  // for
    }  // if
  }  // for
  return match_count;
}

bool Search(std::vector<std::vector<int> > & matrix,
            std::vector<int> & used,
            std::vector<int> & link,
            int n) {
  for (int i = 0; i < matrix.size(); i++) {
    if (!used[i] && matrix[n][i]) {
      used[i] = 1;
      if (link[i] == -1 ||
          Search(matrix, used, link, link[i])) {
        link[i] = n;
        // modify link one time <==> remove the edge in the mathced result
        //                           and add a new edge
        return true;
      }
    }
  }
  return false;
}

// more simple implementation
// link recored the edge from right to left
int Hungarian(std::vector<std::vector<int> > & matrix,
              std::vector<int> & left,
              std::vector<int> * link) {
  int match_count = 0;
  link->clear();
  link->assign(matrix.size(), -1);
  std::vector<int> used;
  for (int i = 0; i < left.size(); i++) {
    // i must not in the mathced results (cam be proof)
    used.clear();
    used.assign(matrix.size(), 0);
    if (Search(matrix, used, *link, left[i])) match_count++;
  }
  return match_count;
}

// TODO (guoliqiang) 
// 从左边开始找出所有未成功找的增广路径，标记这些路径中的点
// set = 左边标记过的点  +  右边没有标记过的点
void FindMinCoverSet() {}

}  // namespace algorithm

#endif  //  __MAX_MATCHING_H_

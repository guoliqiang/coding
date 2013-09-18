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
//   proof:
//   http://www.matrix67.com/blog/archives/116
//   找出最小覆盖点的算法
//
//   http://162.105.203.28/course/ada09/students/00748244-YT-HungaryAlgorithm.pdf
// 匹配：
// 给定一个图G=(V, E)，一个匹配是一个边的子集合M∝E，
// 且满足对所有的顶点v ∈V，M中至多有一条边与v关联。
// 
// 最大二分匹配：(从顶点角度定义)
// 即在二分图上寻找一个匹配，使其所覆盖的顶点最多。
//
// 另一种定义：(从边的角度定义)
// 给定一个二分图G，M为G边集的一个子集，
// 如果M满足当中的任意两条边都不依附于同一个顶点，则称M是一个匹配。
// 极大匹配(Maximal Matching)是指在当前已完成的匹配下,
// 无法再通过增加未完成匹配的边的方式来增加匹配的边数。
// 最大匹配(maximum matching)是所有极大匹配当中边数最大的一个匹配。
// 选择这样的边数最大的子集称为图的最大匹配问题。

// poj1274 poj2159 poj


// 带权二分图
// 在带权二分图的所有匹配中，使匹配边权
// 值之和最大的匹配。

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


// 二分图的最大权匹配
// KM算法是通过给每个顶点一个标号（叫做顶标）来把求最大权匹配的问题转化为求完备匹配的问题的。
// 设顶点Xi的顶标为A[i]，顶点Yi的顶标为B[i]，顶点Xi与Yj之间的边权为w[i,j]。
// 在算法执行过程中的任一时刻，对于任一条边(i,j)，A[i]+B[j]>=w[i,j]始终成立。
//
// 首先保证左右两边顶点个数一直，不一致增加顶点。
// KM算法中其实左右每个定点间都有边（之前没有的是存在权重为0的边）
// 关键是这边边能不能用要看 A[i] + B[j] 是否　等于　matrix[i][j]
//
// KM算法的正确性基于以下定理：
// 若由二分图中所有满足A[i]+B[j]=w[i,j]的边(i,j)构成的子图（称做相等子图）有完备匹配，
// 那么这个完备匹配就是二分图的最大权匹配。
// proof:
// 在这个完备匹配中其边的　权重和　＝　sum(A[i]) + sum(B[j])
// 其它的完备匹配中，因为　有　ｗ[i][j] <= A[i] + B[j] 所以其权重和一定小于上面的值.
//
//
// 初始时
// x0 
// x1   y1
// x2   y2
// [x0 y1] = 2 [x1 y2] = 3 [x2 y2] = 3
//
// 由于左右点的个数不一致，右端增加B0点，如下：
// x0   y0
// x1   y1
// x2   y2
// [x0 y1] = 2 [x1 y2] = 3 [x2 y2] = 3
// 
// 初始化：
// A = {2, 3, 3}  B = {0, 0, 0}
// 
// 首先寻找x0的增广路径，成功。此时 slack为{2, MAX, MAX}  link[1] = 0 表示x0与y1链接
//
// 寻找x1的增广路径，成功。此时 slack = {3, 3, MAX}  link[2] = 1
//
// 寻找x2的增广路径，失败。slack = { 3 3 MAX}
// 修改 A = { 2 0 0} B= {0, 0, 3} stack = {0 0 MAX}
//
// 再次寻找增广路径，成功：x2 -> y2 -> x1 ->y0
// 
//
// O(n^3)
namespace algorithm {

bool KMSearch(std::vector<std::vector<int> > & matrix,
              std::vector<int> & usedx,
              std::vector<int> & usedy,
              std::vector<int> & link,
              std::vector<int> & A,
              std::vector<int> & B,
              std::vector<int> & slack,
              int n) {
  if (usedx[n] == 1) {
    // LOG(INFO) << "usedx[" << n << "] == 1"; 
    return false;
  }
  usedx[n] = 1;
  // LOG(INFO) << "usedx[" << n << "]" << " set be 1";
  for (int i = 0; i < matrix.size(); i++) {
    // LOG(INFO) << " usedy[" << i << "]:" << usedy[i];
    if (usedy[i]) continue;
    // LOG(INFO) << "A[" << n << "]:" << A[n] << " B[" << i << "]" << B[i]
    //           << " matrix[" << n << "][" << i << "]:" << matrix[n][i];
    if (A[n] + B[i] - matrix[n][i] == 0) {
      usedy[i] = 1;
      // LOG(INFO) << "usedy[" << i << "]" << " set be 1";
      if (link[i] == -1 ||
          KMSearch(matrix, usedx, usedy, link, A, B, slack, link[i])) {
        link[i] = n;
        // LOG(INFO) << "link[" << i << "]:" << link[i];
        return true;
      }
    }  else {
      // LOG(INFO) << "A[" << n << "]:" << A[n] << " B[" << i << "]:"
      //           << B[i] << " matrix[" << n << "][" << i << "]:"
      //           << matrix[n][i] << " "<< "slack[" << i << "]:" << slack[i]
      //           << " A[n] + B[i] - matrix[n][i]:" << A[n] + B[i] - matrix[n][i];
      if (slack[i] > A[n] + B[i] - matrix[n][i]) {
        slack[i] = A[n] + B[i] - matrix[n][i];
      }
    }
  }
  return false;
}

void KM(std::vector<std::vector<int> > & matrix,  // n * n  row = A col = B
        std::vector<int> * link) {
  link->clear();
  link->assign(matrix.size(), -1);
  std::vector<int> slack(matrix.size(), 0);
  // 边A[x] ~ B[i] 中 A[n] + B[i] - matrix[n][i] 的最小值，此时A[n] + B[i] - matrix[n][i] ！=0
  // 也就是 A[x] ~ B[i] 没在相等子图中，注意 A[x]处于交错树中 B[i]未处于交错树中
  // stack[i] = min {A[x] + B[i] - matrix[x][i] | A[x]处于交错树中, B[i]不处于交错树中}
  std::vector<int> usedx(matrix.size(), 0);
  std::vector<int> usedy(matrix.size(), 0);
  std::vector<int> A(matrix.size(), 0);  // left
  std::vector<int> B(matrix.size(), 0);  // right
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix.size(); j++) {
      if(matrix[i][j] > A[i]) A[i] = matrix[i][j];
    }
  }
  for (int i = 0; i < matrix.size(); i++) {
  // 从每个x出发寻找增广路,为了找到完备匹配，
  // 对于每一个x中的顶点，找到其增广路就跳出，找不到的话
  // 就需要修改顶标值直至找到一条增广路径为止
    slack.clear();  // Note: slack 的含义仅限于本次的交错树中
    slack.assign(matrix.size(), 0x7fffffff);
    while (true) {
      // LOG(INFO) << "sack:" << JoinVector(slack);
      usedy.clear();
      usedy.assign(matrix.size(), 0);
      usedx.clear();
      usedx.assign(matrix.size(), 0);
      if (KMSearch(matrix, usedx, usedy, *link, A, B, slack, i)) {
        // LOG(INFO) << "usedx:" << JoinVector(usedx);
        // LOG(INFO) << "usedy:" << JoinVector(usedy);
        // LOG(INFO) << "slack:" << JoinVector(slack);
        break;
      }
      int d = 0x7fffffff;
      for (int j = 0; j < slack.size(); j++) {
        if (usedy[j] == 0 && slack[j] < d) d = slack[j];
      }
      CHECK(d != 0x7fffffff);
      for (int j = 0; j < A.size(); j++) {
        if (usedx[j]) {
          A[j] -= d;
          // LOG(INFO) << "A[" << j << "]" << A[j] << " d:" << d;
        } else {
          // LOG(INFO) << "A[" << j << "]" << A[j] << " d:" << d;
        }
      }
      for (int j = 0; j < B.size(); j++) {
        if (usedy[j]) {
          // 这里的slack[j]可能等于0x7fffffff
          B[j] += d;
          // LOG(INFO) << "B[" << j << "]" << B[j] << " d:" << d;
        } else {
          CHECK(slack[j] != 0x7fffffff);
          slack[j] -= d;  // Note
        }
      }
      // LOG(INFO) << "A:" << JoinVector(A);
      // LOG(INFO) << "B:" << JoinVector(B);
      // LOG(INFO) << "slack:" << JoinVector(slack);
    }
  }
}

}  // namespace algorithm

#endif  //  __MAX_MATCHING_H_

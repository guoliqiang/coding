// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-16 20:46:59
// File  : code.cc
// Brief :
// 最小路径覆盖
// (路径可能不只一条)
// 定义：在一个有向图中，找出最少的路径，使得这些路径，经过每一个点，且每一个点只与一条路径相关联，
// 由上面得出：
// 1.一个单独的点是一个路径
//
// 2：如果有路径a，b，c。。。。f，g。a为起点，g为终点。那么a到g的点不在与其他点之间存在有向边。
// 最小路径覆盖=点数---最大匹配数
//
// 证明：
// 1  如果匹配数为0，那么图中没有边，需要n条路径
// 2  如果a，b之间连一条边，那么匹配数增1，需要的路径数会减少一，因为a，b之间只需要一条，那么就证明了

// 下面是黑书的原文，关于最小路径覆盖的证明
//  最小路径覆盖：用尽量少的不想交的简单路径覆盖有向无环图G的所有顶点。
//
//  我们给这个图建立一个二分图模型，把所有顶点拆为两个：X结点i和Y结点i‘，如果图G中存在有向边i-->j，则在二分图中引入边i-->j'
//  ，设二分图的最大匹配为m，则结果就是n-m。这个结果不难理解，因为匹配和路径覆盖是一一对应的。路径覆盖中的每条简单路径除
//  了最后一个结点外都有唯一的后继和它对应（即匹配点）因此匹配边数就是非路径结尾的点数。因为匹配数达到最大时，非路径结尾的
//  结点数达到最大，故路径结尾结点数目最少，即路径数最少。

//  最小路径覆盖=|P|－最大匹配数

// 路径覆盖：就是在图中找一些路经，使之覆盖了图中的所有顶点，且任何一个 顶点有且只有一条路径与之关联。 
// 最小路径覆盖：就是找出最少的路径条数，使之成为Ｐ的一个路径覆盖．路径 覆盖与二分图匹配的关系：最小路径覆盖＝｜Ｐ｜－最大匹配数； 
// 其中最大匹配数的求法是把Ｐ中的每个顶点pi分成两个顶点pi'与pi''，如果 在p中存在一条pi到pj的边，那么在二分图Ｐ＇
// 中就有一条连接pi'与pj''的 有向边（求二分图匹配时必须是单向边）；这里pi' 就是p中pi的出边，pj'' 就是p中pj 的一条入边； 
// 有向图: 最小路径覆盖＝｜Ｐ｜－最大匹配数; 
// 无向图: 最小路径覆盖＝｜Ｐ｜－最大匹配数/2; 

// 最小点集覆盖 
// 一个二分图中的最大匹配数等于这个图中的最小点覆盖数。
// 见最大二分匹配部分

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
bool matrix[MAX][MAX] = {{0}};
int visited[MAX] = {0};
int linkr[MAX] = {0};
int linkl[MAX] = {0};

bool Search(int n) {
  for (int i = 0; i < N; i++) {
    if (matrix[n][i] && visited[i] == 0) {
      if (linkr[i] == -1 || Search(linkr[i])) {
        visited[i] = 1;
        linkr[i] = n;
        return true;
      }
    }
  }
  return false;
}

void Read(std::vector<std::pair<int, int> > & vec) {
  N = vec.size();
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < vec.size(); i++) {
    matrix[vec[i].first][vec[i].second] = true;
  }
}

int MaxMatch(bool directed = true) {
  int rs = 0;
  for (int i = 0; i < N; i++) {
    memset(visited, 0, sizeof(visited));
    if (Search(i)) rs++;
  }
  return directed ? N - rs : N - rs / 2;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

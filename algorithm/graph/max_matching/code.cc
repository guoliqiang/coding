// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-14 20:32:54
// File  : code.cc
// Brief :

/*
普通图的最小点／边覆盖可以转换成二分图最大匹配问题
详细见　mini_edge_point_cover
*/

#include "base/public/common_head.h"

namespace algorithm {

static const int MAX = 100;    // 左右中最大的点个数
bool matrix[MAX][MAX];  // 伪邻接矩阵，和通常的邻接矩阵不同matrix[i][i] 是有意义的：表示左边ith与右边ith有没有关系
bool visited[MAX] = {false};
int link_r[MAX] = {-1};  // out : right ->left
int link_l[MAX] = {-1};  // out : left -> right

bool Search(int n) {
  for (int i = 0; i < MAX; i++) {
    if (matrix[n][i] > 0 && !visited[i]) {
      visited[i] = true;
      if (link_r[i] == - 1 || Search(link_r[i])) {
        link_r[i] = n;
        link_l[n] = i;
        return true;
      }
    }
  }
  return false;
}

int MaxMatch() {
  int rs = 0;
  for (int i = 0; i < MAX; i++) {
    memset(visited, 0, sizeof(visited));  // 当前轮不能找已经找过的点
    // if (link_l[i] != -1) continue;　　这个条件是不用判断的
    if (Search(i)) rs++;  // 不用担心visited[i]没有赋值为1,因为此时还没有任何一个处于右侧的点链接到i
  }
  return rs;
}

}  // namespace algorithm


int main(int argc, char** argv) {
  return 0;
}

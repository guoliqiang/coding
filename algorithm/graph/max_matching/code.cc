// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-14 20:32:54
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

static const int MAX = 100;    // 左右中最大的点个数
bool matrix[MAX][MAX];  // 邻接矩阵
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
    memset(visited, 0, sizeof(visited));
    if (Search(i)) rs++;
  }
  return rs;
}

}  // namespace algorithm


int main(int argc, char** argv) {
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-13 19:23:20
// File  : code.cc
// Brief :
// 树边， 向前边 ， 向后边， 横叉边
// http://www.dlifep.com/?p=825

#include "base/public/common_head.h"

namespace algorithm {

void DFSTag(std::vector<std::vector<int> > & matrix, int cur, std::vector<int> pre, std::vector<int> post, int & cnt) {
  pre[cur] = cnt++;
  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[cur][i] == 0) continue;
    if (pre[i] == 0) {
      LOG(INFO) << "Tree Edge :" << cur << "~" << i;
      DFSTag(matrix, i, pre, post, cnt);
    } else {
      if (post[i] == 0) {
        LOG(INFO) << "Back Edge:" << cur << "~" << i;
      } else {
        if (pre[i] > pre[cur])
          LOG(INFO) << "Forward edge" << cur << "~" << i;
        else {
          LOG(INFO) << "Corss Edge" << cur << "~" << i;
        }
      }
    }
  }
  post[cur] = cnt++;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

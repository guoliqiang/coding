// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 20:06:04
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::vector<std::vector<int> > Multiply(std::vector<std::vector<int> >& A,
                                        std::vector<std::vector<int> >& B) {
  int m1 = A.size();
  int n1 = A[0].size();
  int n2 = B[0].size();

  std::vector<std::vector<int> > ans(m1, std::vector<int>(n2, 0));
  for (int i = 0; i < m1; i++) {
    for (int k = 0; k < n1; k++) {
      if (A[i][k] != 0) {
        for (int j = 0; j < n2; j++) {
          ans[i][j] += A[i][k] * B[k][j];
        }
      }
    }
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > A(2, std::vector<int>(3, 0));
  std::vector<std::vector<int> > B(3, std::vector<int>(3, 0));
  A[0][0] = 1;
  A[1][0] = -1;
  A[1][2] = 3;

  B[0][0] = 7;
  B[0][1] = 1;
  B[2][2] = 1;
  std::vector<std::vector<int> > ans = Multiply(A, B);
  LOG(INFO) << JoinMatrix(&ans);
  return 0;
}

// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 11:17:32
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int ComputeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
  int total = (C - A) * (D - B) + (G - E) * (H - F);

  if (B >= H || D <= F || C <= E || A >= G) {
    return total;
  } else {
    std::vector<int> x;
    std::vector<int> y;
    x.push_back(A);
    x.push_back(C);
    x.push_back(E);
    x.push_back(G);

    y.push_back(B);
    y.push_back(D);
    y.push_back(F);
    y.push_back(H);

    std::sort(x.begin(), x.end());
    std::sort(y.begin(), y.end());
    return total - (x[2] - x[1]) * (y[2] - y[1]);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

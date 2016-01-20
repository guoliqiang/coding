// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 14:09:14
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// next only called after hasNext() == true;
class Vector2D {
 public:
  Vector2D(std::vector<std::vector<int> >& vec2d) {
    data = vec2d;
    i = 0;
    j = 0;
  }

  int next() {
    return data[i][j++];
  }

  bool hasNext() {
    while (i < data.size() && j == data[i].size()) {
      i++;
      j = 0;
    }
    return i < data.size();
  }
  int i;
  int j;
  std::vector<std::vector<int> > data;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  std::vector<int> cur;
  cur.push_back(1);
  matrix.push_back(cur);
  cur.clear();
  cur.push_back(2);
  cur.push_back(3);
  matrix.push_back(cur);
  cur.clear();
  matrix.push_back(cur);
  matrix.push_back(cur);
  Vector2D foo(matrix);
  while (foo.hasNext()) {
    LOG(INFO) << foo.next();
  }
  return 0;
}

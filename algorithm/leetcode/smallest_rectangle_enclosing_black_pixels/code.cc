// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 10:23:09
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int SearchUpLeft(std::vector<int> & row, int x) {
  int b = 0;
  int e = x;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == b && row[mid] > 0) return mid;
    else if (row[mid] > 0 && row[mid - 1] == 0) return mid;
    else if (row[mid] > 0) e = mid - 1;
    else b = mid + 1;
  }
  return 0;
}

int SearchDownRight(std::vector<int> & row, int x) {
  int b = x;
  int e = row.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == e && row[mid] > 0) return mid;
    else if (row[mid] > 0 && row[mid + 1] == 0) return mid;
    else if (row[mid] > 0) b = mid + 1;
    else e = mid - 1;
  }
  return 0;
}

int MinArea(std::vector<std::vector<int> > & image, int x, int y) {
  int m = image.size();
  if (m == 0) return 0;

  int n = image[0].size();
  std::vector<int> row(m, 0);
  std::vector<int> col(n, 0);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      row[i] += image[i][j];
      col[j] += image[i][j];
    }
  }
  int up = SearchUpLeft(row, x);
  int left = SearchUpLeft(col, y);
  int down = SearchDownRight(row, x);
  int right = SearchDownRight(col, y);

  return (right - left + 1) * (down - up + 1);
}

}  // namespace algorithm

namespace NB {
int bsearch(const std::vector<int> & vec, int begin, int end) {
  int b = begin;
  int e = end;
  bool flag = vec[begin] > 0;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] > 0) {
      if (flag) {
        if (mid == e || vec[mid + 1] == 0) return mid;
        else b = mid + 1;
      } else {
        if (mid == b || vec[mid - 1] == 0) return mid;
        else e = mid - 1;
      }
    } else {
      if (flag) e = mid - 1;
      else b = mid + 1;
    }
  }
  return 0;
}

int MinArea(std::vector<std::vector<int> > & image, int x, int y) {
  int m = image.size();
  if (m == 0) return 0;

  int n = image[0].size();
  std::vector<int> row(m, 0);
  std::vector<int> col(n, 0);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      row[i] += image[i][j];
      col[j] += image[i][j];
    }
  }
  int up = bsearch(row, 0, x);
  int down = bsearch(row, x, m - 1);
  int left = bsearch(col, 0, y);
  int right = bsearch(col, y, n - 1);

  return (right - left + 1) * (down - up + 1);
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > vec(3, std::vector<int>(4, 0));
  vec[0][2] = 1;
  vec[1][1] = 1;
  vec[1][2] = 1;
  vec[2][1] = 1;
  LOG(INFO) << MinArea(vec, 0, 2);
  LOG(INFO) << NB::MinArea(vec, 0, 2);
  return 0;
}

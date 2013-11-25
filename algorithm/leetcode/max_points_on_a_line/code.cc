// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-25 20:32:40
// File  : code.cc
// Brief :

/*
 * 27 / 27 test cases passed.
 * Status: Accepted
 * Runtime: 40 ms
 * Submitted: 0 minutes ago
 * 数据貌似有点弱阿
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
struct Point {
  int x;
  int y;
  Point() : x(0), y(0){}
  Point(int a, int b): x(a), y(b) {}
};

class Cmp {
 public:
  bool operator() (const Point & p1, const Point & p2) const {
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
  }
};

bool Check(Point & p1, Point & p2, Point & p3) {
  int t = (p3.x - p2.x) * (p2.y - p1.y) - (p3.y - p2.y) * (p2.x - p1.x);
  return t == 0;
}

std::vector<std::pair<Point, int> > Unique(std::vector<Point> & points) {
  std::map<Point, int, Cmp> foo;
  for (int i = 0; i < points.size(); i++) {
    if (!foo.count(points[i])) foo[points[i]] = 1;
    else foo[points[i]]++;
  }
  std::vector<std::pair<Point, int> > rs;
  for (std::map<Point, int, Cmp>::iterator i = foo.begin(); i != foo.end(); i++) {
    rs.push_back(std::make_pair(i->first, i->second));
  }
  return rs;
}

int MaxPoints(std::vector<Point> & points) {
  std::vector<std::pair<Point, int> > data = Unique(points);
  if (data.size() <= 2) {
    int count = 0;
    for (int i = 0; i < data.size(); i++) count += data[i].second;
    return count;
  }
  
  int max = 0;
  int n = data.size();
  for (int i = 0; i < n - max; i++) {
    std::vector<int> used(n, 0);
    for (int j = i + 1; j < n; j++) {
      if (used[j] == 1) continue;
      int t = data[i].second + data[j].second;
      used[j] = 1;
      for (int k = j + 1; k < n; k++) {
        if (Check(data[i].first, data[j].first, data[k].first)) {
          t += data[k].second;
          used[k] = 1;
        }
      }
      max = std::max(t, max);
    }
  }
  return max;
}


}  // namespace algorithm

using namespace algorithm;

/*
 * (0,9),(138,429),(115,359),(115,359),(-30,-102),(230,709),(-150,-686),(-135,-613),(-60,-248),(-161,-481),(207,639),(23,79),(-230,-691),(-115,-341),(92,289),(60,336),(-105,-467),(135,701),(-90,-394),(-184,-551),(150,774)
 * */

int main(int argc, char** argv) {
  std::vector<Point> data;
  data.push_back(Point(0, 9));
  data.push_back(Point(138, 429));
  data.push_back(Point(115, 359));
  data.push_back(Point(115, 359));
  data.push_back(Point(-30, -102));
  data.push_back(Point(230, 709));
  data.push_back(Point(-150, -686));
  data.push_back(Point(-135, -613));
  data.push_back(Point(-60, -248));
  data.push_back(Point(-161, -481));
  data.push_back(Point(207, 639));
  data.push_back(Point(23, 79));
  data.push_back(Point(-230, -691));
  data.push_back(Point(-115, -341));
  data.push_back(Point(92, 289));
  data.push_back(Point(60, 336));
  data.push_back(Point(-105, -467));
  data.push_back(Point(135, 701));
  data.push_back(Point(-90, -394));
  data.push_back(Point(-184, -551));
  data.push_back(Point(150, 774));

  LOG(INFO) << MaxPoints(data);
  return 0;
}

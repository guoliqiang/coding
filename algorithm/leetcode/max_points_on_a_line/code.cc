// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-25 20:32:40
// File  : code.cc
// Brief :

/*
Given n points on a 2D plane, find the maximum number of points that lie on the same straight line.
*/

/*
 * 27 / 27 test cases passed.
 * Status: Accepted
 * Runtime: 40 ms
 * Submitted: 0 minutes ago
 * 数据貌似有点弱阿
 * */

#include "base/public/common_ojhead.h"

struct Point {
  int x;
  int y;
  Point() : x(0), y(0){}
  Point(int a, int b): x(a), y(b) {}
};

namespace algorithm {
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

// n^2
namespace NB {
int GcdSub(int x, int y) {
  if (!x) return y;
  if (!y) return x;
  return GcdSub(y, x % y);
}

// x == 0 时符号和y保持一致，否则和x保持一致
int Gcd(int x, int y) {
  int t = GcdSub(abs(x), abs(y));
  if (x == 0) return y < 0 ? -t : t;
  return x < 0 ? -t : t;
}

/*
y = dy/dx * x + b
*/

int MaxPoint(std::vector<Point> & v) {
  int max = 0;
  for (int i = 0; i < v.size(); i++) {
    std::map<std::pair<int, int>, int> m;
    int same = 1;
    int cmax = 0;
    for (int j = i + 1; j < v.size(); j++) {
      int dx = v[j].x - v[i].x;
      int dy = v[j].y - v[i].y;
      int gcd = Gcd(dx, dy);
      if (gcd == 0) {
        same++;
        continue;
      }
      dx /= gcd;
      dy /= gcd;
      std::pair<int, int> t = std::make_pair(dx, dy);
      if (m.count(t)) m[t]++;
      else m[t] = 1;
      cmax = std::max(cmax, m[t]);
    }
    max = std::max(max, cmax + same);
  }
  return max;
}
}

namespace my {
using namespace std;

int gcd(int x, int y) {
    if (!x) return y;
    if (!y) return x;
    return gcd(y, x % y);
}

bool Cmp(const Point & a, const Point & b) {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
}

class Solution {
public:
    int maxPoints(vector<Point>& points) {
        std::sort(points.begin(), points.end(), Cmp);

        int n = points.size();
        int ans = 0;
        for (int i = 0; i < n; i++) {
            std::map<std::pair<int, int>, int> m;
            int same = 0;
            int max = 0;
            for (int j = i; j < n; j++) {
                int dx = points[j].x - points[i].x;
                int dy = points[j].y - points[i].y;
                int t = gcd(fabs(dx), fabs(dy));
                if (t == 0) {
                    same++;
                    continue;
                }
                dx /= t;
                dy /= t;
                std::pair<int, int> cur(dx, dy);
                if (!m.count(cur)) m[cur] = 0;
                m[cur]++;
                max = std::max(max, m[cur]);
            }
            ans = std::max(ans, max + same);
        }
        return ans;
    }
};
using namespace std;

class Solution2 {
public:
    int maxPoints(vector<Point>& points) {
        std::sort(points.begin(), points.end(), Cmp);

        int n = points.size();
        int ans = 0;
        for (int i = 0; i < n; i++) {
            std::map<std::pair<int, int>, int> m;
            int same = 0;
            int max = 0;
            for (int j = i; j < n; j++) {
                int dx = points[j].x - points[i].x;
                int dy = points[j].y - points[i].y;
                int t = gcd(fabs(dx), fabs(dy));
                if (t == 0) {
                    same++;
                    continue;
                }
                dx /= t;
                dy /= t;
                if (dx * dy >= 0) {
                  dx = fabs(dx);
                  dy = fabs(dy);
                } else if (dx * dy < 0 && dx > 0) {
                  dx = -dx;
                  dy = - dy;
                }
                std::pair<int, int> cur(dx, dy);
                if (!m.count(cur)) m[cur] = 0;
                m[cur]++;
                max = std::max(max, m[cur]);
            }
            ans = std::max(ans, max + same);
        }
        return ans;
    }
};
}  // namespace my


namespace practice {
using namespace std;

int gcd(int x, int y) {
    if (!x) return y;
    if (!y) return x;
    return gcd(y, x % y);
}
int MaxPoints(vector<Point>& points) {
    int ans = 0;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        int same = 1;
        int max = 0;
        std::map<std::pair<int, int>, int> dict;
        for (int j = i + 1; j < n; j++) {
            int dx = points[j].x - points[i].x;
            int dy = points[j].y - points[i].y;

            int t = gcd(fabs(dx), fabs(dy));
            if (t == 0) {
                same++;
                continue;
            }
            dx /= t;
            dy /= t;
            if (dx * dy >= 0) {
                dx = fabs(dx);
                dy = fabs(dy);
            } else if (dx > 0 && dy < 0) {
                dx = -dx;
                dy = -dy;
            }
            std::pair<int, int> key(dx, dy);
            if (!dict.count(key)) dict[key] = 0;
            dict[key]++;
            max = std::max(max, dict[key]);
        }
        ans = std::max(ans, max + same);
    }
    return ans;
}
}  // namespace practice

/*
 * (0,9),(138,429),(115,359),(115,359),(-30,-102),(230,709),(-150,-686),(-135,-613),(-60,-248),(-161,-481),(207,639),(23,79),(-230,-691),(-115,-341),(92,289),(60,336),(-105,-467),(135,701),(-90,-394),(-184,-551),(150,774)
 * */

int main(int argc, char** argv) {
  std::vector<Point> data;
  data.push_back(Point(0, 0));
  data.push_back(Point(1, 1));
  data.push_back(Point(1, -1));

  LOG(INFO) << practice::MaxPoints(data);
  return 0;
}

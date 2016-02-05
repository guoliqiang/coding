// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-26 11:52:10
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
using namespace std;

struct Node {
    int x;
    int y;
    int left;
    Node() {}
    Node(int x, int y, int left) : x(x), y(y), left(left){}
};

bool Cmp(const Node & x, const Node & y) {
    if (x.x == y.x) {
      if (x.left == 1 && y.left == 1) return x.y > y.y;
      else if (x.left == 2 && y.left == 2) return x.y < y.y;
      else return x.left < y.left;
    } else return x.x < y.x;
}

class Solution {
public:
    vector<pair<int, int> > getSkyline(vector<vector<int> >& buildings) {
        int n = buildings.size();
        std::vector<Node> vec;
        for (int i = 0; i < n; i++) {
            vec.push_back(Node(buildings[i][0], buildings[i][2], 1));
            vec.push_back(Node(buildings[i][1], buildings[i][2], 2));
        }
        std::sort(vec.begin(), vec.end(), Cmp);
        std::priority_queue<int, std::vector<int>, std::less<int> > queue;
        std::map<int, int> dict;
        queue.push(0);

        int pre = 0;
        std::vector<std::pair<int, int> > ans;
        for (int i = 0; i < vec.size(); i++) {
            if (vec[i].left == 1) {
                queue.push(vec[i].y);
            } else {
                if (!dict.count(vec[i].y)) dict[vec[i].y] = 0;
                dict[vec[i].y]++;
            }
            while (dict.count(queue.top()) && dict[queue.top()] > 0) {
                dict[queue.top()]--;
                queue.pop();
            }
            int cur = queue.top();
            if (pre != cur) {
                ans.push_back(std::make_pair(vec[i].x, cur));
                pre = cur;
            }
        }
        return ans;

    }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}

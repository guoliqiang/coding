// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 01:43:28
// File  : code.cc
// Brief :

/*
 * Accepted   132k   204MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
std::pair<int, int> Next(const std::pair<int, int> & cur, int k) {
  std::pair<int, int> rs = cur;
  switch(k) {
    case 8: {  // north
      rs.second++;
      break;
    }
    case 2: {  // sourth
      rs.second--;
      break;
    } 
    case 6: {  // east
      rs.first++;
      break;
    }
    case 4: {  // west
      rs.first--;
      break;
    }
    case 9: {  // northeast
      rs.first++;
      rs.second++;
      break;
    }
    case 7: {  // northwest
      rs.first--;
      rs.second++;
      break;
    }
    case 3: {  // sourtheast
      rs.first++;
      rs.second--;
      break;
    }
    case 1: {  // sourthwest
      rs.first--;
      rs.second--;
      break;
    }
    default:
      break;
  }
  return rs;
}

void Read() {
  int c = 0;
  while (scanf("%d", &c) != EOF) {
    getchar();
    for (int i = 0; i < c; i++) {
      std::pair<int, int> cur = std::make_pair(0, 0);
      long long rs = 0;
      char ch;
      while ((ch = getchar()) != '5') {
        std::pair<int, int> next = Next(cur, ch - '0');
        // LOG(INFO) << ch << " [" << cur.first << ","
        // << cur.second << "] [" << next.first << "," << next.second << "]";
        rs += cur.first * next.second - cur.second * next.first;
        cur = next;
      }
      getchar();
      if (rs < 0) rs *= -1;
      if (rs % 2 == 0) printf("%lld\n", rs / 2);
      else printf("%lld.5\n", rs / 2);
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}

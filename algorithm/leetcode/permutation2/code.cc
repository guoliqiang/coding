// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 20:17:00
// File  : code.cc
// Brief :

/*
 *
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 300 milli secs
 * Progress: 30/30 test cases passed.
 * */

#include <vector>
#include <set>
#include "base/public/common_head.h"
#include "base/public/string_util.h"

namespace algorithm {

void PermuteSub(std::vector<int> & num, int n, std::set<std::vector<int> > * rs) {
  if (n >= num.size()) {
    rs->insert(num);
  } else {
    // 不能剔除干净 如 [2 2 1 1]
    // 所以改用set保存结果了
    for (int i = n; i < num.size(); i++) {
      if(i != n && num[i] == num[n]) {
        // 注意这种方法不能剔除干净，自己记忆中这个算法是可以的，一定要改过来
        // 原因是两个相同的元素虽然没有交换，但m<=>a，m'<=>b 等同于：m <=> b, m' <=> a
        // 求有重复元素的subset与permutation一定要用jlu的模板
        continue;
      } else {
        std::swap(num[n], num[i]);
        PermuteSub(num, n + 1, rs);
        std::swap(num[n], num[i]);
      }
    }
  }
}

std::vector<std::vector<int> > Permute(std::vector<int> & num) {
  std::vector<std::vector<int> > rs;
  std::set<std::vector<int> > foo;
  PermuteSub(num, 0, &foo);
  for (std::set<std::vector<int> >::iterator i = foo.begin();
       i != foo.end(); i++) {
    rs.push_back(*i);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

namespace twice {
void Permutation(std::vector<int> & vec, std::vector<int> & used, std::vector<int> & path,
                 int cur, int cnt, std::vector<std::vector<int> > & rs) {
  if (cur == cnt){
      rs.push_back(path);
  }
  for (int i = 0; i < vec.size(); i++) {
      if (used[i] > 0) {
          used[i]--;
          path.push_back(vec[i]);
          Permutation(vec, used, path, cur + 1, cnt, rs);
          used[i]++;
          path.pop_back();
      }
  }
}

std::vector<std::vector<int> > Permutation(std::vector<int> & num) {
    std::map<int, int> tmap;
    for(int i = 0; i < num.size(); i++) {
        if (tmap.count(num[i])) tmap[num[i]]++;
        else tmap[num[i]] = 1;
    }
    std::vector<int> vec;
    std::vector<int> used;
    int cnt = 0;
    for (std::map<int, int>::iterator i = tmap.begin(); i != tmap.end(); i++) {
        vec.push_back(i->first);
        used.push_back(i->second);
        cnt += i->second;
    }
    std::vector<int> path;
    std::vector<std::vector<int> > rs;
    Permutation(vec, used, path, 0, cnt, rs);
    return rs;
}
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(1);
  num.push_back(2);
  std::vector<std::vector<int> > foo = Permute(num);
  for (int i = 0; i < foo.size(); i++) {
    LOG(INFO) << JoinVector(foo[i]);
  }
  return 0;
}

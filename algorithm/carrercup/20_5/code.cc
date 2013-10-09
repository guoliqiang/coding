// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 22:16:58
// File  : code.cc
// Brief :

/*
 * 如果想做到O(1)，先算出来，在用hash保存
 * 两个键值作为key的hash trick可以有：
 * 1.链接时key1_key2始终保持key1 < key2，查询时也要按此方式链接
 * 2.自己实现hash table，hash(key1) = i  hash(key2) = j  d[i][j] 存储value
 *   注意处理hash冲突
 * */
#include "base/public/common_head.h"

namespace algorithm {

int Min(std::vector<std::string> & page, std::vector<std::string> & words) {
  std::map<std::string, int> dict;
  std::map<std::string, int> tmap;
  int count = 0;
  int b = 0;
  int e = 0;
  int min = page.size();
  
  for (int i = 0; i < words.size(); i++) {
    if (tmap.count(words[i])) dict[words[i]]++;
    else dict[words[i]] = 1;
    tmap[words[i]] = 0;
  }

  for (e = 0; e < page.size(); e++) {
    if (!dict.count(page[e])) continue;
    if (dict.count(page[e])) {
      if (tmap[page[e]] < dict[page[e]]) count++;
      tmap[page[e]]++;
    }
    if (count == words.size()) {
      while(!dict.count(page[b]) || tmap[page[b]] > dict[page[b]]) {
        b++;
        if (tmap.count(page[e])) tmap[page[e]]--;
      }
      min = std::min(min, e - b + 1);
    }
  }
  return min;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> page;
  page.push_back("what");
  page.push_back("is");
  page.push_back("your");
  page.push_back("name");
  page.push_back("my");
  page.push_back("name");
  page.push_back("is");
  page.push_back("seven");
  std::vector<std::string> words;
  words.push_back("is");
  words.push_back("seven");
  LOG(INFO) << Min(page, words);
  return 0;
}

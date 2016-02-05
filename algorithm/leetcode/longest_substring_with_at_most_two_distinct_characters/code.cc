// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-27 11:36:41
// File  : code.cc
// Brief :
// https://leetcode.com/discuss/21929/share-my-c-solution
#include "base/public/common_ojhead.h"

namespace algorithm {
int LengthOfLongestSubstringTwoDistinct(std::string & s, int k) {
  std::map<char, int> dict;
  int ans = 0;
  int cnt = 0;
  int b = 0;
  int e = 0;
  while (e < s.size()) {
    if (!dict.count(s[e])) {
      dict[s[e]] = 0;
      cnt++;
    }
    dict[s[e]]++;
    while (cnt > k) {
      dict[s[b]]--;
      if (dict[s[b]] == 0) cnt--;
      b++;
    }
    ans = std::max(ans, e - b + 1);
    e++;
  }
  return ans;
}
}  // namespace algorithm

namespace extend {
int LengthOfLongestSubstringTwoDistinct(const std::string & s, int k) {
  std::map<char, int> dict;
  int ans = 0;
  int b = -1;
  for (int i = 0; i < s.size(); i++) {
    dict[s[i]] = i;

    while (dict.size() > k) {
      int min = INT_MAX;
      char ch = '\0';
      for (std::map<char, int>::iterator i = dict.begin(); i != dict.end(); i++) {
        if (i->second < min) {
          min = i->second;
          ch = i->first;
        }
      }
      // LOG(INFO) << min << " " << ch;
      b = dict[ch];
      dict.erase(ch);
    }
    ans = std::max(ans, i - b);
  }
  return ans;
}

// 适用于stream
int LengthOfLongestSubstringTwoDistinct2(const std::string & s, int k) {
  std::map<char, int> dict;
  std::priority_queue<std::pair<int, char>, std::vector<std::pair<int, char> >,
             std::greater<std::pair<int, char> > > queue;
  int ans = 0;
  int b = -1;
  for (int i = 0; i < s.size(); i++) {
    dict[s[i]] = i;
    queue.push(std::make_pair(i, s[i]));

    while (dict.size() > k) {
      while (dict[queue.top().second] != queue.top().first) {
        queue.pop();
      }
      b = queue.top().first;
      dict.erase(queue.top().second);
      queue.pop();
    }
    ans = std::max(ans, i - b);
  }
  return ans;
}

// 还可以不用堆
// 比如， ababac, m = 2
// 当出现c时，字典里面有{"b":3,"a":4}. 我只记录某个字母最后一次出现的位置。现在我可以知道，
// 最后一个b出现在位置3，从位置4开始，只有m-1个字母。
// 然后这个地方，找到b，要扫一遍这个table。但是如果用OrderDict结合LRU cache的思想，
// 每次更新一个字母的位置时，把它放到字典的最后，你就知道，字典中第一个，永远是我要找的那个.
int LengthOfLongestSubstringTwoDistinct3(const std::string & s, int k) {
  std::map<char, int> dict;
  std::list<char> list;
  std::map<char, std::list<char>::iterator> iter;
  int ans = 0;
  int b = -1;
  for (int i = 0; i < s.size(); i++) {
    if (dict.count(s[i])) {
      list.erase(iter[s[i]]);
    }
    list.push_back(s[i]);
    std::list<char>::iterator foo = list.end();
    foo--;
    iter[s[i]] = foo;
    dict[s[i]] = i;

    while (dict.size() > k) {
      b = dict[*(list.begin())];
      iter.erase(*(list.begin()));
      dict.erase(*(list.begin()));
      list.pop_front();
    }
    ans = std::max(ans, i - b);
  }
  return ans;
}
}  // namespace extend

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << extend::LengthOfLongestSubstringTwoDistinct("aaaaaaaaaaaaa", 2);
  LOG(INFO) << extend::LengthOfLongestSubstringTwoDistinct2("aaaaaaaaaaaaa", 2);
  LOG(INFO) << extend::LengthOfLongestSubstringTwoDistinct3("aaabccccddddddddddddeeeeeeeeee", 2);
  return 0;
}

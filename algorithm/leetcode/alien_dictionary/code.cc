// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-18 16:39:01
// File  : code.cc
// Brief :

/*

There is a new alien language which uses the latin alphabet. However, the order among letters are unknown to you. You receive a list of words from the dictionary, where words are sorted lexicographically by the rules of this new language.Derive the order of letters in this language.

For example,
Given the following words in dictionary,

[
  "wrt",
  "wrf",
  "er",
  "ett",
  "rftt"
]
The correct order is: "wertf".

Note:
You may assume all letters are in lowercase.
If the order is invalid, return an empty string.
There may be multiple valid order of letters, return any one of them is fine.
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string AlienOrder(std::vector<std::string> & words) {
  std::map<char, std::set<char> > graph;
  std::set<char> dict;
  std::map<char, int> cnt;

  for (int i = 0; i < words.size(); i++) {
    dict.insert(words[i].begin(), words[i].end());
    if (i == words.size() - 1) continue;

    std::string & s1 = words[i];
    std::string & s2 = words[i + 1];
    for (int j = 0; j < std::min(s1.size(), s2.size()); j++) {
      if (s1[j] == s2[j]) continue;

      if (!graph.count(s1[j])) graph[s1[j]] = std::set<char>();
      graph[s1[j]].insert(s2[j]);
      break;
    }
  }

  for (std::set<char>::iterator i = dict.begin(); i != dict.end(); i++) {
    cnt[*i] = 0;
  }

  for (std::map<char, std::set<char> >::iterator i = graph.begin(); i != graph.end(); i++) {
    std::set<char> & set = i->second;
    for (std::set<char>::iterator j = set.begin(); j != set.end(); j++) {
      cnt[*j]++;
    }
  }

  std::queue<char> queue;
  std::string ans;
  for (std::map<char, int>::iterator i = cnt.begin(); i != cnt.end(); i++) {
    if (i->second == 0) queue.push(i->first);
  }

  while (queue.size()) {
    char cur = queue.front();
    queue.pop();
    ans.push_back(cur);
    std::set<char> & set = graph[cur];
    for (std::set<char>::iterator i = set.begin(); i != set.end(); i++) {
      cnt[*i]--;
      if (cnt[*i] == 0) queue.push(*i);
    }
  }
  return ans.size() == dict.size() ? ans : "";
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("wrt");
  vec.push_back("wrf");
  vec.push_back("er");
  vec.push_back("ett");
  vec.push_back("rftt");
  LOG(INFO) << AlienOrder(vec);
  return 0;
}

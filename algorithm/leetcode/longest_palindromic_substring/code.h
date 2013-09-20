// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 20:43:19
// File  : code.h
// Brief : Manacher’s Algorithm
//         http://leetcode.com/2011/11/longest-palindromic-substring-part-ii.html
//         better doc: http://larrylisblog.net/WebContents/images/LongestPalindrom.pdf

/*
 *  origin str:
 *    abcbabb
 *  changed str:
 *    0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
 *    ^  #  a  #  b  #  c  #  b  #  a  #  b  #  b  #  $
 *  p:0  1  2  1  2  1  6  1  2  1  2  1  2  3  2  1  0
 *
 *  p[i] = 1 + len(i左侧与i右侧‘相同’的字符串)
 *  e.g. p[6] = 1 + len("#a#b#") because of "#a#b#" '=' "#b#a#"
 *
 *  --------------------------------------------------------------
 *       /|\          |            /|\             |            /|\
 *        |           |             |              |             |
 *        |           |        center_index        |             |
 *        |           |                            |             |
 *   left_center_bound|                            |      right_center_bound
 *                    |                            |
 *          j = 2 * center_index - i               i
 *  
 *  j和i关于center_index对称.
 *
 *  if i < right_center_bound:
 *     if p[j] < right_center_bound - i : p[i] = p[j]
 *     else p[i] = right_center_bound - i
 *     也就是：p[i] = min(p[j], right_center_bound - i)
 *     由于 else 部分p[i]值可能小于实际值，因此接下来需要对p[i]向两边扩展
 *
 *  if i >= right_center_bound:
 *     p[i] = 1
 *     p[i]值可能小于实际值，因此接下来需要对p[i]向两边扩展
 *
 *
 */ 

#ifndef  __CODE_H_
#define  __CODE_H_

#include <algorithm>
#include "base/public/logging.h"

namespace algorithm {

std::string PreProcess(std::string & str) {
  if (str.size() == 0) return "^$";
  std::string rs = "^";
  for (int i = 0; i< str.length(); i++) {
    rs.append("#");
    rs.append(&str[i], 1);
  }
  rs.append("#$");
  return rs;
}

// used 36 ms

std::string LongestPalindrome(std::string & str_i) {
  std::string str = PreProcess(str_i);
  int * p = new int[str.size()];
  memset(p, 0, sizeof(int) * str.size());
  int center_index = 0;
  int center_right_bound_index = 0;
  int max_index = 0;
  int max_value = 0;
  for (int i = 1; i < str.size() - 1; i++) {
    int mirror_i = 2 * center_index - i;
    p[i] =  (center_right_bound_index > i) ? std::min(p[mirror_i], center_right_bound_index - i) : 1;
    while (str[i - p[i]] == str[i + p[i]]) p[i]++;
    if (p[i] > max_value) {
      max_value = p[i];
      max_index = i;
    }
    if (p[i] + i - 1 > center_right_bound_index) {
      center_index = i;
      center_right_bound_index = i + p[i] - 1;
    }
  }
  delete [] p;
  LOG(INFO) << max_index << ":" << max_value;
  return  max_value > 0 ?
          str_i.substr((max_index - max_value + 2) / 2 - 1, max_value - 1) : "";
}

}  // namespace algorithm

#endif  //  __CODE_H_

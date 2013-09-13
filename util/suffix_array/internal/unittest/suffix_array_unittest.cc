// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-13 11:51:47
// File  : suffix_array_unittest.cc
// Brief :
#include "../../public/suffix_array.h"

using namespace util;  // NOLINT


int main(int argc, char** argv) {
  // 回文串
  std::string str = "aabaaaab$baaaabaa";
  SuffixArray foo(str);
  // foo.OutRank();
  // foo.OutSa();
  // foo.OutHeight();
  // foo.OutRMQ();
  int index = 0;
  int len = foo.GetRMQMax(1, str.size(), &index);
  LOG(INFO) << len << " " << foo.get_sa()[index];

  // 可重叠的最长重复子串
  // <=> KMP next数组的最大值
  str = "aabaaaab";
  SuffixArray foo2(str);
  index = 0;
  len = foo2.GetRMQMax(1, str.size(), &index);
  LOG(INFO) << len << " " << foo2.get_sa()[index];

  // 求两个字符串的最长公共子串
  // 方法2：dp
  // 不同于LCS问题（最长公共子序列）
  str = "aaba$abac";
  SuffixArray foo3(str);
  index = 0;
  len = foo3.GetRMQMax(1, str.size(), &index);
  LOG(INFO) << len << " " << foo3.get_sa()[index];

  // 求字符串的不可重叠的最长重复子串
  // http://blog.csdn.net/leolin_/article/details/7217117
  str = "aabaabac";
  SuffixArray foo4(str);
  LOG(INFO) << " " << foo4.NotOverlapLen();

  // 计算不相同子串的个数
  // http://blog.csdn.net/xiaoxiaoluo/article/details/7678221
  str = "abc";
  SuffixArray foo5(str);
  LOG(INFO) << foo5.DiffSubStringNum();
  return 0;
}

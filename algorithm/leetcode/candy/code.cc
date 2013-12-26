// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 22:16:34
// File  : code.cc
// Brief :

/*
There are N children standing in a line. Each child is assigned a rating value.
You are giving candies to these children subjected to the following requirements:
Each child must have at least one candy.
Children with a higher rating get more candies than their neighbors.
What is the minimum candies you must give?

*/

#include "base/public/common_head.h"

namespace NB {

/*
 *
 * 25 / 25 test cases passed.
 * Status: Accepted
 * Runtime: 140 ms
 * Submitted: 0 minutes ago
 *
 * */

int Candy(std::vector<int> & r) {
  int n = r.size();
  std::vector<int> v(n, 1);
  int k = 2;
  for (int i = 0; i < n; i++) {
    if (i - 1 >=0 && r[i] > r[i - 1]) {
      v[i] = std::max(k++, v[i]);
    } else k = 2;
  }
  k = 2;
  for (int i = n - 1; i >= 0; i--) {
    if (i + 1 < n && r[i] > r[i + 1]) {
      v[i] = std::max(k++, v[i]);
    } else k = 2;
  }
  int rs = 0;
  for (int i = 0; i < n; i++) {
    rs += v[i];
  }
  return rs;
}
}  // namespace

namespace algorithm {

/*
 * there is a bug in this function, but now i can not find it.
 *
 * */
void Candy(std::vector<int> & ratings, int b, int e, std::vector<int> & v) {
  if (b > e) return;
  if (b == e) {
    v[b] = 1;
  } else {
    int mid = b + (e - b) / 2;
    Candy(ratings, b, mid, v);
    Candy(ratings, mid + 1, e, v);
    if (ratings[mid] < ratings[mid + 1] && v[mid] < v[mid + 1]) return;
    if (ratings[mid] == ratings[mid + 1]) return;
    LOG(INFO) << "mid:" << mid << " b:" << b << " e:" << e;
    LOG(INFO) << ratings[mid] << " " << ratings[mid + 1] << " " << v[mid] << " " << v[mid + 1];
    if (ratings[mid] > ratings[mid + 1]) {
      HERE(INFO);
      int t = v[mid + 1] - v[mid];
      v[mid] += (t + 1);
      for (int i = mid - 1; i >= 0; i--) {
        if (ratings[i] > ratings[i + 1] && v[i] <= v[i + 1]) v[i] += (v[i + 1] - v[i] + 1);
      }
    } else {
      HERE(INFO);
      int t = v[mid] - v[mid + 1];
      v[mid + 1] += (t + 1);
      for (int i = mid + 2; i<= e; i++) {
        if (ratings[i] > ratings[i - 1] && v[i] <= v[i - 1]) v[i] += (v[i - 1] - v[i] + 1);
      }
    }
  }
}

int Candy(std::vector<int> & ratings) {
  std::vector<int> v(ratings.size(), 0);
  Candy(ratings, 0, ratings.size() - 1, v);
  int rs = 0;
  for (int i = 0; i < v.size(); i++) rs += v[i];
  for (int i = 1; i < v.size() - 1; i++) {
    if (ratings[i] > ratings[i - 1] && v[i] < v[i - 1]) LOG(INFO) << i << " " << ratings[i] << " " << v[i];
    if (ratings[i] > ratings[i + 1] && v[i] < v[i + 1]) LOG(INFO) << i << " " << ratings[i] << " " << v[i];
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

// 这个最简洁
namespace twice {
int Dp(std::vector<int> & ratings) {
  int n = ratings.size();
  std::vector<int> dp(n, 1); //  初始值必须为1
  for (int i = 1; i < n; i++) {
    if (ratings[i] > ratings[i - 1]) dp[i] = std::max(dp[i], dp[i - 1] + 1);
  }
  for (int i = n - 2; i >= 0; i--) {
    if (ratings[i] > ratings[i + 1]) dp[i] = std::max(dp[i], dp[i + 1] +1);
  }
  int rs = 0;
  for (int i = 0; i <n; i++) rs += dp[i];
  return rs;
}
}  // namespace twice

int main(int argc, char** argv) {
  
  std::vector<int> r;
  r.push_back(1);
  r.push_back(2);
  // r.push_back(1);
  LOG(INFO) << Candy(r);
  std::string s = "370,28,37,15,379,458,342,346,328,257,81,400,426,461,349,362,315,410,427,458,264,322,326,350,369,383,184,343,402,274,184,124,154,73,491,33,31,333,379,212,91,313,112,369,274,461,83,441,223,362,399,339,185,77,189,406,312,373,249,66,147,433,191,153,358,182,187,242,368,418,454,311,231,418,180,358,379,115,151,454,478,403,293,163,480,482,69,145,207,318,63,207,103,254,360,313,289,399,55,9,318,361,320,401,279,352,111,10,319,263,464,297,18,109,460,350,91,381,495,151,51,59,358,6,165,218,320,306,118,375,315,288,89,487,41,368,191,5,379,11,120,195,308,138,305,121,488,248,2,336,399,406,395,257,412,412,328,232,71,446,460,238,86,49,226,127,417,269,132,148,280,104,344,441,242,1,414,231,101,416,419,1,174,314,110,87,78,438,319,149,236,279,388,322,328,466,302,98,235,286,246,368,391,442,161,133,295,75,216,397,343,135,250,18,301,360,105,232,299,424,381,35,56,121,210,384,439,12,482,175,298,81,395,41,23,408,175,171,483,243,68,178,231,318,48,32,178,153,116,329,430,498,217,338,471,279,222,411,291,57,438,441,490,185,483,365,93,10,36,428,105,104,106,336,274,155,221,305,160,337,134,442,335,203,280,159,482,355,70,125,412,360,67,254,45,402,119,138,412,156,66,17,112,24,206,387,179,427,192,192,264,178,134,452,382,415,111,216,270,33,342,34,393,261,288,290,163,407,428,427,63,346,444,176,370,2,415,402,429,107,94,194,285,80,146,19,495,109,236,117,494,78,151,387,191,439,29,354,199,457,281,114,155,77,290,25,80,57,279,361,164,225,55,302,306,53,321,153,14,57,271,8,487,422,247,178,214,276,384,413,233,17,27,240,95,170,118,27,227,397,388,244,475,296,46,281,201,367,434,216,277,57,224,264,332,472,295,398,248,179,311,334,197,190,74,144,212,192,23,292,442,263,388,417,411,434,50,113,153,336,329,430,394,53,47,226,377,194,124,126,225,287,312,274,329,238,418,394,283,293,186,225,409,426,494,320,360,44,433,365,380,262,148,126,316,47,204,45,241,180,23,466,467,335,241,149,426,11,43,209,157,81,286,66,7,280,386,219,176,320";
  std::vector<std::string> split;
  SplitString(s, ',', &split);
  r.clear();
  for (int i = 0; i < split.size(); i++) {
    r.push_back(StringToInt(split[i]));
  }
  LOG(INFO) << Candy(r);
  LOG(INFO) << NB::Candy(r);
  return 0;
}

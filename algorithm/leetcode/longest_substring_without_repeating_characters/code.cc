// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 10:07:41
// File  : code.cc
// Brief :

/*
#include<string>
#include<set>

/ 344 milli secs
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        int rs = 0;
        for (int i = 0; i < s.size(); i++) {
            std::set<char> foo;
            int j = i;
            while (j >= 0) {
                if (foo.count(s[j])) break;
                foo.insert(s[j]);
                j--;
            }
            if (i - j > rs) rs = i - j;
        }
        return rs;
    }
};
 */

/*
一下代码是从一个str重复的子串中寻找未出现重复字符的最长重复子串
时间复杂度 O(n * log(n))
*/

#include "base/public/logging.h"
#include <map>
#include <set>
#include <algorithm>

// height[i] >= height[i - 1] -1 (i is the subscript of string:, 1,2...)
// "abcdf"
//  height[cdf] >= height[bcdf] - 1
//  Proof: if height[bcdf] = 0 abosul
//
void Height(int * height_, int * rank_, int * sa_, std::string & str_) {
  int j = 0;
  for (int i = 1; i < str_.size(); i++) {
    if (rank_[i] == 1) continue;
    while (str_[i + j] ==
           str_[sa_[rank_[i] - 1] + j] &&
           sa_[rank_[i] - 1] + j < str_.size()) {
      j++;
    }
    height_[rank_[i]] = j;
    if (j > 0) j--;
  }
}

void BucketSort(int j, int * sum_, int * sa_, int * rank_, std::string & str_) {
  int size = str_.size();
  int * tsa = new int[size];
  memset(tsa, 0, size * sizeof(tsa[0]));
  memset(sum_, 0, 0xffff * sizeof(sum_[0]));

  // when i + j > size the rank_[i + j]] value is 0
  for (int i = 1; i < size; i++) {
    CHECK(rank_[i + j] < 0xffff);
    sum_[rank_[i + j]]++;
  }

  // i must begin from 1
  // sum_[0] is usefull becuase of rank_[i + j] can be 0.
  for (int i = 1; i < 0xffff; i++) {
    sum_[i] += sum_[i - 1];
  }

  // sort using the second key
  for (int i = size - 1; i >= 1; i--) {
    CHECK(sum_[rank_[i + j]] < size);
    tsa[sum_[rank_[i + j]]--] = i;
  }
  // tsa is the result after sorting using second key

  memset(sum_, 0, 0xffff * sizeof(sum_[0]));
  for (int i = 1; i < size; i++) {
    CHECK(rank_[i] < size);
    sum_[rank_[i]]++;
  }
  for (int i = 1; i < 0xffff; i++) {
    sum_[i] += sum_[i - 1];
  }
  // sort using the first key
  for (int i = size - 1; i > 0; i--) {
    CHECK(sum_[rank_[tsa[i]]] < size);
    sa_[sum_[rank_[tsa[i]]]--] = tsa[i];
  }
  return;
  delete [] tsa;
}

std::string Palindrome(int * height_, int * rank_,
                       int * sa_, std::string & str_) {
  int index = 0;
  int rs = 0;
  for (int i = 1; i < str_.size(); i++) {
    if (rs <= height_[i]) {
      int tmp = str_.size() / 2;
      int foo1 = sa_[i];
      int foo2 = sa_[i-1];
      if (foo1 > tmp && foo2 > tmp) continue;
      if (foo1 < tmp && foo2 < tmp) continue;
      if (foo1 > tmp) foo1 -= tmp;
      if (foo2 > tmp) foo2 -= tmp;
      if (foo1 + foo2 + height_[i] -2 != tmp -1) continue;
      // LOG(INFO) << foo1 + foo2 + height_[i] -2 << " " << tmp -1;
      // LOG(INFO) << foo1 << " " << foo2 << " " << tmp;
      rs = height_[i];
      index = i;
      // LOG(INFO) << str_.substr(sa_[i]);
      // LOG(INFO) << str_.substr(sa_[i-1]);
      // LOG(INFO) << rs;
    }
  }

  return str_.substr(sa_[index], rs);
}

int GetResult(int * height_, int * rank_, int * sa_, std::string & str_) {
  int rs = 0;
  int size = str_.size();
  // for (int i = 1; i < size; i++) {
  //   LOG(INFO) << i << ": sa:" << sa_[i] << " rank:"
  //             << rank_[i] << " height:" << height_[i];
  // }
  for (int i = 1; i < size; i++) {
    if (height_[i] > 0) {
      // LOG(INFO) << "[" << i << "] height:" << height_[i];
      std::set<char> foo;
      int cnt = 0;
      for (int j = 0; j < height_[i]; j++) {
        // LOG(INFO) << height_[i] << " " << str_[sa_[i] + j];
        if (foo.count(str_[sa_[i] + j])) {
          if (cnt > rs) rs = cnt;
          break;
        }
        foo.insert(str_[sa_[i] + j]);
        cnt++;
      }
      if (cnt > rs) rs = cnt;
    }
  }
  return rs;
}

std::string BuildString(std::string & str) {
  std::string str_;
  str_.resize(str.size() + 1);
  for (int i = 0; i < str.size(); i++)
    str_[i + 1] = str[i];
  
  int size = str_.size();
  int * sa_ = new int[size];
  int * rank_ = new int[size * 2];
  int * sum_ = new int[0xffff];
  int * trank = new int[size];
  int * height_ = new int[size];
  
  memset(sa_, 0, sizeof(int) * size);
  memset(rank_, 0, sizeof(int) *size * 2);
  memset(sum_, 0, sizeof(int) * 0xffff);
  memset(height_, 0, sizeof(int) * size);

  for (int i = 1; i< size; i++) {
    trank[i]= str_[i];
  }
  for (int i = 1; i < size; i++) {
    CHECK(trank[i] < 0xffff) << trank[i] << " " << str_[i];
    sum_[static_cast<int>(trank[i])]++;
  }
  for (int i = 2; i < 0xffff; i++) {
    sum_[i] += sum_[i - 1];
  }

  for (int i = size - 1; i >= 1; i--) {
    sa_[sum_[static_cast<int>(trank[i])]--] = i;
  }

  // build first rank and sa
  // at begin rank may have same values.
  rank_[sa_[1]] = 1;
  int p = 1;
  for (int i = 2; i < size; i++) {
    if (trank[sa_[i]] != trank[sa_[i - 1]]) p++;
    rank_[sa_[i]] = p;
  }

  for (int j = 1; j < size; j *= 2) {
    BucketSort(j, sum_, sa_, rank_, str_);
    trank[sa_[1]] = 1;
    p = 1;
    for (int i = 2; i < size; i++) {
      if ((rank_[sa_[i]] != rank_[sa_[i-1]]) ||
          (rank_[sa_[i] + j ] != rank_[sa_[i - 1] + j])) {
        p++;
      }
      trank[sa_[i]] = p;
    }
    for (int i = 1; i < size; i++) {
      rank_[i] = trank[i];
      CHECK(rank_[i] >= 0) << i << ":" << rank_[i];
    }
  }
  Height(height_, rank_, sa_, str_);
  std::string rs = Palindrome(height_, rank_, sa_, str_);
  delete [] height_;
  delete [] sum_;
  delete [] sa_;
  delete [] rank_;
  delete [] trank;
  return rs;
}


int Build(std::string & str) {
  std::string str_;
  str_.resize(str.size() + 1);
  for (int i = 0; i < str.size(); i++)
    str_[i + 1] = str[i];
  
  int size = str_.size();
  int * sa_ = new int[size];
  int * rank_ = new int[size * 2];
  int * sum_ = new int[0xffff];
  char * trank = new char[size];
  int * height_ = new int[size];
  
  memset(sa_, 0, sizeof(int) * size);
  memset(rank_, 0, sizeof(int) * size * 2);
  memset(sum_, 0, sizeof(int) * 0xffff);
  memset(height_, 0, sizeof(int) * size);

  for (int i = 1; i< size; i++) {
    trank[i]= str_[i];
  }
  for (int i = 1; i < size; i++) {
    sum_[static_cast<int>(trank[i])]++;
  }
  for (int i = 2; i < 0xffff; i++) {
    sum_[i] += sum_[i - 1];
  }

  for (int i = size - 1; i >= 1; i--) {
    sa_[sum_[static_cast<int>(trank[i])]--] = i;
  }

  // build first rank and sa
  // at begin rank may have same values.
  rank_[sa_[1]] = 1;
  int p = 1;
  for (int i = 2; i < size; i++) {
    if (trank[sa_[i]] != trank[sa_[i - 1]]) p++;
    rank_[sa_[i]] = p;
  }

  for (int j = 1; j < size; j *= 2) {
    BucketSort(j, sum_, sa_, rank_, str_);
    trank[sa_[1]] = 1;
    p = 1;
    for (int i = 2; i < size; i++) {
      if ((rank_[sa_[i]] != rank_[sa_[i-1]]) ||
          (rank_[sa_[i] + j ] != rank_[sa_[i - 1] + j])) {
        p++;
      }
      trank[sa_[i]] = p;
    }
    for (int i = 1; i < size; i++) {
      rank_[i] = trank[i];
    }
  }
  Height(height_, rank_, sa_, str_);
  int rs = GetResult(height_, rank_, sa_, str_);
  delete [] height_;
  delete [] sum_;
  delete [] sa_;
  delete [] rank_;
  delete [] trank;
  return rs;
}


/*
 * O(n) 140ms
 *
int lengthOfLongestSubstring(std::string s) {
  if (s.size() == 0) return 0;
  std::map<char, int> foo;
  int rs = 0;
  int * dp = new int[s.size()];
  memset(dp, 0, s.size() * sizeof(int));
  dp[0] = 1;
  rs = 1;
  foo[s[0]] = 0;
  for (int i = 1; i< s.size(); i++) {
    // LOG(INFO) << s[i] << " :" <<  foo[s[i]];
    if (!foo.count(s[i]))  foo[s[i]] = -1;
    if (i - foo[s[i]] - 1> dp[i - 1]) dp[i] = dp[i - 1] +1;
    else dp[i] = i - foo[s[i]];
    if (dp[i] > rs) rs = dp[i];
    foo[s[i]] = i;
  }
  delete [] dp;
  return rs;
}
*/

/*
// official answer

int lengthOfLongestSubstring(std::string s) {
  int n = s.length();
  int i = 0, j = 0;
  int maxLen = 0;
  bool exist[256] = { false };
  while (j < n) {
    if (exist[static_cast<int>(s[j])]) {
      maxLen = std::max(maxLen, j-i);
      LOG(INFO) << j << " : " << s[j] << ":" << maxLen << " j - i:" << j - i << " i:" << i;
      while (s[i] != s[j]) {
        exist[static_cast<int>(s[i])] = false;
        i++;
      }
      LOG(INFO) << "[i]:" << i;
      i++;
      j++;
    } else {
      LOG(INFO) << j << " : [" << s[j] << "] set as true";
      exist[static_cast<int>(s[j])] = true;
      j++;
    }
  }
  maxLen = std::max(maxLen, n-i);
  return maxLen;
}
*/

std::string Reverse(std::string & str) {
  std::string rs;
  for (int i = str.size() - 1; i >= 0; i--) {
    rs.append(&str[i], 1);
  }
  return rs;
}

int main(int argc, char** argv) {
  std::string str = "esbtzjaaijqkgmtaajpsdfiqtvxsgfvijpxrvxgfumsuprzlyvhclgkhccmcnquukivlpnjlfteljvykbddtrpmxzcrdqinsnlsteonhcegtkoszzonkwjevlasgjlcquzuhdmmkhfniozhuphcfkeobturbuoefhmtgcvhlsezvkpgfebbdbhiuwdcftenihseorykdguoqotqyscwymtjejpdzqepjkadtftzwebxwyuqwyeegwxhroaaymusddwnjkvsvrwwsmolmidoybsotaqufhepinkkxicvzrgbgsarmizugbvtzfxghkhthzpuetufqvigmyhmlsgfaaqmmlblxbqxpluhaawqkdluwfirfngbhdkjjyfsxglsnakskcbsyafqpwmwmoxjwlhjduayqyzmpkmrjhbqyhongfdxmuwaqgjkcpatgbrqdllbzodnrifvhcfvgbixbwywanivsdjnbrgskyifgvksadvgzzzuogzcukskjxbohofdimkmyqypyuexypwnjlrfpbtkqyngvxjcwvngmilgwbpcsseoywetatfjijsbcekaixvqreelnlmdonknmxerjjhvmqiztsgjkijjtcyetuygqgsikxctvpxrqtuhxreidhwcklkkjayvqdzqqapgdqaapefzjfngdvjsiiivnkfimqkkucltgavwlakcfyhnpgmqxgfyjziliyqhugphhjtlllgtlcsibfdktzhcfuallqlonbsgyyvvyarvaxmchtyrtkgekkmhejwvsuumhcfcyncgeqtltfmhtlsfswaqpmwpjwgvksvazhwyrzwhyjjdbphhjcmurdcgtbvpkhbkpirhysrpcrntetacyfvgjivhaxgpqhbjahruuejdmaghoaquhiafjqaionbrjbjksxaezosxqmncejjptcksnoq";
  std::string rstr = Reverse(str);
  str = str + "$" + rstr;
  LOG(INFO) << str;
  LOG(INFO) << BuildString(str);
  str = "bbbbb";
  str = str + "$" + str;
  LOG(INFO) << BuildString(str);
  // LOG(INFO) << lengthOfLongestSubstring(str);
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 10:57:02
// File  : code.cc
// Brief :

/*
 *Given a digit string, return all possible letter combinations that the number could represent.

 A mapping of digit to letters (just like on the telephone buttons) is given below.


 Input:Digit string "23"
 Output: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].
 Note:
 Although the above answer is in lexicographical order, your answer could be in any orde.
 * */

#include <algorithm>
#include <vector>
#include "base/public/logging.h"
#include "base/public/string_util.h"



namespace algorithm {
std::vector<std::vector<char> > phone;

void Build() {
  phone.resize(10);
  std::vector<char> foo;
  phone[0] = foo;
  phone[1] = foo;

  foo.clear();
  foo.push_back('a'); foo.push_back('b'); foo.push_back('c');
  phone[2] = foo;

  foo.clear();
  foo.push_back('d'); foo.push_back('e'); foo.push_back('f');
  phone[3] = foo;

  foo.clear();
  foo.push_back('g'); foo.push_back('h'); foo.push_back('i');
  phone[4] = foo;

  foo.clear();
  foo.push_back('j'); foo.push_back('k'); foo.push_back('l');
  phone[5] = foo;
  
  foo.clear();
  foo.push_back('m'); foo.push_back('n'); foo.push_back('o');
  phone[6] = foo;
  
  foo.clear();
  foo.push_back('q'); foo.push_back('p'); foo.push_back('r'); foo.push_back('s');
  phone[7] = foo;
  
  foo.clear();
  foo.push_back('t'); foo.push_back('u'); foo.push_back('v');
  phone[8] = foo;
  
  foo.clear();
  foo.push_back('w'); foo.push_back('x'); foo.push_back('y'); foo.push_back('z');
  phone[9] = foo;
}

void Get(std::vector<int> & num, std::vector<int> & index, int n, std::vector<std::string> * rs) {
  if (n >= num.size()) {
    std::string instance;
    for (int i = 0; i< num.size(); i++) {
      // LOG(INFO) << index[i];
      instance += phone[num[i]][index[i]];
    }
    // LOG(INFO) << "instance:" << instance;
    rs->push_back(instance);
  } else {
    for (int i = 0; i < phone[num[n]].size(); i++) {
      index[n] = i;
      Get(num, index, n + 1, rs);
    }
  }
}

std::vector<std::string> LetterCombinations(std::string & digits) {
  Build();
  std::vector<int> num;
  std::vector<int> index(digits.size(), 0);
  for (int i = 0; i < digits.size(); i++) {
    num.push_back(digits[i] - '0');
  }
  std::vector<std::string> rs;
  Get(num, index, 0, &rs);
  return rs;
}

}  // namespace algorithm

namespace other {
using namespace std;
void letterCombinations_aux(int step, string& path,
                            vector<string>& ans, const string& digits) {
  //pay attention to this kind of statement
  const static string strT[10] = {"","","abc","def","ghi","jkl","mno","qprs","tuv","wxyz"};
  if(step == digits.size()) {
    ans.push_back(path);
    return;
  }
  for(int i = 0; i < strT[digits[step]-'0'].size(); ++i) {
    path.push_back(strT[digits[step]-'0'][i]);
    letterCombinations_aux(step+1, path, ans, digits);
    path.resize(path.size() - 1);
  }
}

vector<string> letterCombinations(string digits) {
  string path;
  vector<string> ans;
  int step = 0;
  letterCombinations_aux(step, path, ans, digits);
  return ans;
}
}  // other

// 直接回溯
// 之前求subset是二元，这个就是多元的.
namespace twice {
std::string phone[] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

void Trace(std::string digits, int k, std::string & path, std::vector<std::string> & rs) {
  if (k == digits.size()) {
    rs.push_back(path);
  } else {
    int j = digits[k] - '0';
    for (int i = 0; i < phone[j].size(); i++) {
      path.push_back(phone[j][i]);
      Trace(digits, k + 1, path, rs);
      // path.pop_back();  // pop_back() 可以在leetcode上编译通过
      path.erase(path.size() - 1);
    }
  }
}
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::string digits = "234";
  std::vector<std::string> rs = LetterCombinations(digits);
  LOG(INFO) << rs.size();
  // LOG(INFO) << JoinVector(rs);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }

  digits = "23";
  rs = LetterCombinations(digits);
  LOG(INFO) << rs.size();
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }

  rs = other::letterCombinations(digits);
  LOG(INFO) << rs.size();
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i];
  }
  return 0;
}

// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 11:16:46
// File  : code.cc
// Brief :
//

#include "base/public/logging.h"

/*  ac vertion
 *
bool Match(const char * str, const char * pattern, const char * str_b) {
  // LOG(INFO) << "begin";
  // if (*str != '\0') LOG(INFO) << "str:" << *str;
  // if (*pattern != '\0') LOG(INFO) << " pattern :" << *pattern;
  // LOG(INFO) << "end";
  if (*str == '\0' && *pattern == '\0') {
    // LOG(INFO) << "return";
    return true;
  }
  if (*str == '\0' && *pattern == '*' && *(pattern + 1) == '\0') {
    // LOG(INFO) << "return";
    return true;
  }
  
  // if (*str == '\0' && *pattern == '*' && *(pattern + 1) != '\0') {
  //   LOG(INFO) << "return";
  //   return false;
  // }
  // if (*str == '\0' && (*(pattern + 1) != '*' || (*(pattern + 1)) == '\0')) {
  //   LOG(INFO) << pattern;
  //   LOG(INFO) << "return";
  //   return false;
  // }

  if (*pattern == '\0') {
    // LOG(INFO) << "return";
    return false;
  }
  
  if (*(pattern + 1) != '\0' && *(pattern + 1) == '*'){
    bool rs = Match(str, pattern + 2, str_b);
    if (rs) return true;
  }

  if (*pattern == '.' && str != '\0') {
    return  Match(str + 1, pattern +1, str_b);
  } else if (*pattern == '*') {
      // LOG(INFO) << "str:" << *str << " pattern :" << *pattern;
      bool rs = Match(str, pattern + 1, str_b);
      if (rs) return true;
      if ((str > str_b && *str == *(str - 1)) ||
          (*(pattern - 1) == '.' && *str != '\0'))
        return Match(str + 1, pattern, str_b);
  } else if (*pattern == *str) {
    return Match(str + 1, pattern + 1, str_b);
    // bool rs = Match(str + 1, pattern + 1, str_b);
    // if (rs) return true;
    // if (*(pattern + 1) != '\0' && *(pattern + 1) == '*'){
    //   return Match(str, pattern + 2, str_b);
    // }
  } else {
    return false;
    // if (*(pattern + 1) != '\0' && *(pattern + 1) == '*') {
    //   return Match(str, pattern + 2, str_b);
    // } else {
    //  return false;
    // }
  }
  return false;
}
*/

bool Match(const char * str, const char * pattern, const char * str_b) {
  if (*str == '\0' && *pattern == '\0') return true;
  if (*str != '\0' && *pattern == '\0') return false;
  
  // str = "aa" pattern = "c*aa"
  // *号前面的符号一次都不匹配
  if (*(pattern + 1) != '\0' && *(pattern + 1) == '*'){
    bool rs = Match(str, pattern + 2, str_b);
    if (rs) return true;
  }

  if (*pattern == '.' && *str != '\0') {
    return  Match(str + 1, pattern +1, str_b);
  } else if (*pattern == '*') {
    // 这里会处理这样的情况 : *str == '\0' left pattern = "*c*b*" / "*"
    bool rs = Match(str, pattern + 1, str_b);  // * 号前面的符号匹配一次
    if (rs) return true;
    if (((str > str_b && *str == *(str - 1)) || (*(pattern - 1) == '.')) &&
        *str != '\0') // * 号前面的符号匹配2~n次
      return Match(str + 1, pattern, str_b);
  } else if (*str != '\0' && *pattern == *str) {
    return Match(str + 1, pattern + 1, str_b);
  } else {
    return false;
  }
  return false;
}

namespace twice {
bool IsMatch(const char * s, const char * p) {
  if (*s == '\0' && *p == '\0') return true;
  if (*p == '\0') return false;
  if (*(p + 1)== '*' && IsMatch(s, p + 2)) return true;
  
  if (*p == '.' && *s != '\0') {  // 含义 . 能匹配任何字符但不能匹配'\0'
      return IsMatch(s + 1, p + 1);
  } else if (*p == '*') {
      if (IsMatch(s, p + 1)) return true;
      if (*s == *(p - 1) || ( *s != '\0' && *(p - 1) == '.')) // 同理 . 能匹配任何字符但不能匹配'\0'
        return IsMatch(s + 1, p);
      
      return false;
  } else if (*p == *s) {
      return IsMatch(s + 1, p + 1);
  } else {
      return false;
  }
}
}  // namespace twice

int main(int argc, char** argv) {
  std::string str = "";
  std::string pattern = ".";
  
  str = "abbabaaaaaaacaa";
  pattern = "a*.*b.a.*c*b*a*c*";
  LOG(INFO) << str << " " << pattern << " " << Match(str.data(), pattern.data(), str.data());
  
  
  str = "";
  pattern = ".";
  LOG(INFO) << str << " " << pattern << " " << Match(str.data(), pattern.data(), str.data());
  

  str = "a";
  pattern = "ab*";
  LOG(INFO) << str << " " << pattern << " " << Match(str.data(), pattern.data(), str.data());
  
  str = "ab";
  pattern = ".*c";
  LOG(INFO) << str << " " << pattern << " " << Match(str.data(), pattern.data(), str.data());
  
  str = "aa";
  pattern = "a*";
  LOG(INFO) << str << " " << pattern << " " << Match(str.c_str(), pattern.c_str(), str.data());
  
  str = "aaa";
  pattern = "aa";
  LOG(INFO) << str << " " << pattern << " " << Match(str.c_str(), pattern.c_str(), str.data());
  
  str = "aa";
  pattern = "a*";
  LOG(INFO) << str << " " << pattern << " " << Match(str.c_str(), pattern.c_str(), str.data());
  
  str = "aa";
  pattern = ".*";
  LOG(INFO) << str << " " << pattern << " " << Match(str.c_str(), pattern.c_str(), str.data());
  
  str = "ab";
  pattern = ".*";
  LOG(INFO) << str << " " << pattern << " " << Match(str.c_str(), pattern.c_str(), str.data());
  
  str = "aab";
  pattern = "c*a*b";
  LOG(INFO) << str << " " << pattern << " " << Match(str.c_str(), pattern.c_str(), str.data());
  return 0;
}

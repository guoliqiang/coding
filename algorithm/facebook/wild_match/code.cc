// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-08 10:34:19
// File  : code.cc
// Brief :

/*
 * You are given an array of strings (a dictionary). The task is to implement two functions. The first is setup(),  
 * which preprocesses the dictionary to your liking. The second is isMember(), that, given a word, returns whether  
 * or not the word exists in the dictionary. The word given to isMember() may contain dots ('.'), which match  
 * exactly one character, but any character value, as shown in the example below. Words may contain any number of 
 * dots in any position. 
 * 
 * setup({"foo", "ba", "baz", NULL});
 * isMember("foo"); //returns true
 * isMember("garply"); //returns false
 * isMember("f.o"); //returns true (it matches foo)
 * isMember(".."); // returns false (there are no two-letter words) 
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
class FB {
  public:
    void setup(std::vector<std::string> & vec) {
      dict.clear();
      for (int i = 0; i < vec.size(); i++) {
        dict.push_back(vec[i]);
      }
    }
    
    bool isMember(std::string word) {
      for (int i = 0; i < dict.size(); i++) {
        if (Match(dict[i].c_str(), word.c_str())) return true;
      }
      return false;
    }
    
    bool Match(const char * s, const char * p) {
      if (*s == '\0' && *p == '\0') return true;
      if (*p == '\0') return false;
      
      if (*p == '.' && *s != '\0') {
        return Match(s + 1, p + 1);
      } else if (*p == *s) {
        return Match(s + 1, p + 1);
      } else {
        return false;
      }
    }
    
  private:
    std::vector<std::string> dict;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
 std::vector<std::string> vec;
 vec.push_back("foo");
 vec.push_back("bar");
 vec.push_back("ba");
 FB foo;
 foo.setup(vec);
 LOG(INFO) << foo.isMember("foo");     // 1
 LOG(INFO) << foo.isMember("garply");  // 0
 LOG(INFO) << foo.isMember("f.o");     // 1
 LOG(INFO) << foo.isMember("..");      // 0
 return 0;
}

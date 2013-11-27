// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 16:55:10
// File  : code.cc
// Brief :

/*
 * Given a roman numeral, convert it to an integer.
 *
 * Input is guaranteed to be within the range from 1 to 3999.
 *
 * 个位数举例
 * Ⅰ 1,Ⅱ 2, Ⅲ 3, Ⅳ 4, Ⅴ 5, Ⅵ 6, Ⅶ  7, Ⅷ 8, Ⅸ 9
 * 十位数举例
 * Ⅹ 10, XX 20 XXX 30, XL,40, L 50, LX 60, LXX 70,  LXXX 80, XC 90 
 * 百位数举例
 * C 100, CC 200, CCC 300, CD 400, D 500, DC 600, DCC 700, DCCC 800, CM 900
 * 千位数举例
 * M 1000, MM 2000, MMM 3000
 * */

#include <map>
#include "base/public/logging.h"

const char * bit[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
// begin with I V
const char * ten[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
// begin with X L
const char * hundred[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
// beign with C D
const char * thourand[] = {"", "M", "MM", "MMM" };
// beign with M

const char * * ptr[] = {bit, ten, hundred, thourand};

std::map<std::string, int> roman;

void BuildMap() {
  for(int i = 1; i < 10; i++) {
    roman.insert(std::make_pair(bit[i], i));
  }

  for(int i = 1; i < 10; i++) {
    roman.insert(std::make_pair(ten[i], i * 10));
  }

  for(int i = 1; i < 10; i++) {
    roman.insert(std::make_pair(hundred[i], i * 100));
  }

  for(int i = 1; i < 5; i++) {
    roman.insert(std::make_pair(thourand[i], i * 1000));
  }
}

std::string Int2Roman(int v) {
  std::string rs = "";
  int i = 0;
  while (v) {
    if (v % 10 != 0) rs = ptr[i][v % 10] + rs;
    i++;
    v /= 10;
  }
  return rs;
}


enum STATE {BIT = 0, TEN = 1, HUN = 2, THO = 3, END = 4} state;

void StateOfTHO(std::string & str, int & index) {
  if (index < str.size() && str[index] == 'M') {
    index++;
  } else {
    state = END;
  }
}

void StateOfHUN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'C' || str[index] == 'D' ||  str[index] == 'M')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfTEN(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'X' || str[index] == 'L' ||  str[index] == 'C')) {
    index ++;
  } else {
    state = END;
  }
}

void StateOfBIT(std::string & str, int & index) {
  if (index < str.size() && 
      (str[index] == 'I' || str[index] == 'V' ||  str[index] == 'X')) {
    index ++;
  } else {
    state = END;
  }
}

void Start(std::string & str) {
  if (str.size() == 0) state = END;
  else {
    switch(str[0]) {
      case 'I':
      case 'V':
        state = BIT;
        break;
      case 'X':
      case 'L':
        state = TEN;
        break;
      case 'C':
      case 'D':
        state = HUN;
        break;
      case 'M':
        state = THO;
        break;
      default:
        state = END;
    }
  }
}

bool Token(std::string & str, std::string * rs) {
  if (str.size() == 0) return false;
  int index = 0;
  Start(str);
  while (state != END) {
    switch(state) {
      case BIT:
        StateOfBIT(str, index);
        break;
      case TEN:
        StateOfTEN(str, index);
        break;
      case HUN:
        StateOfHUN(str, index);
        break;
      case THO:
        StateOfTHO(str, index);
        break;
      case END:
       break;
    }
  }
  CHECK(index >= 0);
  *rs = str.substr(0, index);
  str = str.substr(index);
  return true;
}

int Roman2Int(std::string str) {
  std::string token = "";
  int rs = 0;
  while (Token(str, &token)) {
    LOG(INFO) << token;
    CHECK(roman.count(token));
    if (roman.count(token))rs += roman[token];
  }
  return rs;
}

int main(int argc, char** argv) {
  BuildMap();
  LOG(INFO) << Int2Roman(333);
  std::string str = "I";
  LOG(INFO) << Roman2Int(str);
  return 0;
}

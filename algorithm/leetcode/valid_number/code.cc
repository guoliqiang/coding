// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 09:48:00
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 1479/1479 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 84/84 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

enum STATE {
  L_START = 0,
  L_ERROR = 1,
  L_SUCCEED = 2,
  L_B_NUMBER = 3,
  L_D_NUMBER = 4,
  L_E_NUMBER = 5,
  L_DOT = 6,
  L_S_DOT = 7,
  L_E = 8,
  L_PLUS = 9,
  L_MINUS = 10,
  L_E_PLUS = 11,
  L_E_MINUS = 12,
  L_E_SPACE = 13
} state;


bool ReachEnd(const char* p) {
  if (*p == '\0') {
    state = L_SUCCEED;
    return true;
  }
  return false;
}


#define CASE_NUM  case '0':\
                  case '1':\
                  case '2':\
                  case '3':\
                  case '4':\
                  case '5':\
                  case '6':\
                  case '7':\
                  case '8':\
                  case '9'

#define NEXT(p) (p)++;\
                break

void AfterSTART(const char* & p) {
  switch (*p) {
    case ' ':
    case '\t':
      NEXT(p);
    case '+': {
      state = L_PLUS;
      NEXT(p);
    }
    case '-': {
      state = L_MINUS;
      NEXT(p);
    }
    CASE_NUM : {
      state = L_B_NUMBER;
      NEXT(p);
    }
    case '.': {
      state = L_S_DOT;
      NEXT(p);
    }
    default:
     state = L_ERROR;
  }
}

void AfterSDOT(const char * & p) {
  switch (*p) {
    CASE_NUM : {
      state = L_D_NUMBER;
      NEXT(p);
    }
    default:
     state = L_ERROR;
  }
}

void AfterESPACE(const char * & p) {
  if (ReachEnd(p)) return;
  switch (*p) {
    case ' ':
    case '\t': {
      NEXT(p);
    }
    default:
     state = L_ERROR;
  }
}

void AfterPLUS(const char* & p) {
  switch(*p) {
    CASE_NUM : {
      state = L_B_NUMBER;
      NEXT(p);
    }
    case '.': {
      state = L_S_DOT;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}

void AfterMINUS(const char* & p) {
  AfterPLUS(p);
}

void AfterBNUMBER(const char* & p) {
  if (ReachEnd(p)) return;
  switch(*p) {
    CASE_NUM : {
      NEXT(p);
    }
    case '.': {
      state = L_DOT;
      NEXT(p);
    }
    case 'e':
    case 'E': {
      state = L_E;
      NEXT(p);
    }
    case ' ':
    case '\t': {
      state = L_E_SPACE;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}

void AfterDOT(const char* & p) {
  if (ReachEnd(p)) return;
  switch(*p) {
    CASE_NUM : {
      state = L_D_NUMBER;
      NEXT(p);
    }
    case ' ':
    case '\t': {
      state = L_E_SPACE;
      NEXT(p);
    }
    case 'e':
    case 'E': {
      state = L_E;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}

void AfterE(const char* & p) {
  switch(*p) {
    CASE_NUM : {
      state = L_E_NUMBER;
      NEXT(p);
    }
    case '+': {
      state = L_E_PLUS;
      NEXT(p);
    }
    case '-': {
      state = L_E_MINUS;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}

void AfterDNUMBER(const char* & p) {
  if (ReachEnd(p)) return;
  switch(*p) {
    CASE_NUM : {
      NEXT(p);
    }
    case ' ':
    case '\t': {
      state = L_E_SPACE;
      NEXT(p);
    }
    case 'e':
    case 'E': {
      state = L_E;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}

void AfterENUMBER(const char* & p) {
  if (ReachEnd(p)) return;
  switch(*p) {
    CASE_NUM : {
      NEXT(p);
    }
    case ' ':
    case '\t': {
      state = L_E_SPACE;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}

void AfterEPLUS(const char* & p) {
  switch (*p) {
    CASE_NUM : {
      state = L_E_NUMBER;
      NEXT(p);
    }
    default:
      state = L_ERROR;
  }
}


void AfterEMINUS(const char* & p) {
  AfterEPLUS(p);
}

bool IsValidNum(const char* p) {
  state = L_START;
  while (true) {
    switch(state) {
      case L_ERROR:
        return false;
      
      case L_SUCCEED:
        return true;
      
      case L_START: {
        AfterSTART(p);
        break;
      }

      case L_B_NUMBER:{
        AfterBNUMBER(p);
        break;
      }

      case L_PLUS:{
        AfterPLUS(p);
        break;
      }

      case L_MINUS:{
        AfterMINUS(p);
        break;
      }

      case L_E:{
        AfterE(p);
        break;
      }

      case L_DOT:{
        AfterDOT(p);
        break;
      }

      case L_E_PLUS:{
        AfterEPLUS(p);
        break;
      }

      case L_E_MINUS:{
        AfterEMINUS(p);
        break;
      }

      case L_D_NUMBER:{
        AfterDNUMBER(p);
        break;
      }

      case L_E_NUMBER:{
        AfterENUMBER(p);
        break;
      }

      case L_E_SPACE: {
        AfterESPACE(p);
        break;
      }

      case L_S_DOT: {
        AfterSDOT(p);
        break;
      }
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int a = 10;
  LOG(INFO) << a;
  std::string str = "";

  str = "1.e3";
  LOG(INFO) << str << ":" << IsValidNum(str.c_str());
  
  return 0;
  str = " 0.1 ";
  LOG(INFO) << str << ":" << IsValidNum(str.c_str());
  
  str = "abc";
  LOG(INFO) << str << ":" << IsValidNum(str.c_str());
  
  str = "1 a";
  LOG(INFO) << str << ":" << IsValidNum(str.c_str());
  
  str = "2e 10";
  LOG(INFO) << str << ":" << IsValidNum(str.c_str());
  
  return 0;
}

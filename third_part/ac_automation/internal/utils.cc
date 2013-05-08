// Copyright 2011 Jike Inc. All Rights Reserved.
// Author: Ji Jian (jijian@jike.com)
// Date  : 2011-12-16 16:42:48
// File  : utils.cpp
#include <unistd.h>
#include <ctype.h>
#include <vector>
#include <string>
#include "../public/utils.h"
#include "../../../base/public/utf_string_conversion_utils.h"
#include "../../../base/public/string_util.h"

using std::string;
using std::vector;
using std::basic_string;
using base::ReadUnicodeCharacter;
using base::WriteUnicodeCharacter;

namespace ac_automaton {

// since the character out of 0x0000~0xFFFF is rare, so we just drop them
// '\r', '\n' is converted to ' ', so we can handle it as whole string
bool ToString16(const string &utf8Str, basic_string<uint16> *pStr) {
  pStr->clear();
  int32 strLen = static_cast<int>(utf8Str.length());
  uint32 c;
  for (int32 charIdx = 0; charIdx < strLen; ++charIdx) {
    if (!ReadUnicodeCharacter(utf8Str.c_str(), strLen, &charIdx, &c)) {
      return false;
    }
    // use space to denote rare characters
    if (c & 0xFFFF0000) {
      continue;
    }
    // little endian
    pStr->push_back(static_cast<uint16>(c));
  }
  return true;
}

basic_string<uint16> ToString16(uint16 c) {
  basic_string<uint16> str;
  str.push_back(c);
  return str;
}

string ToUTF8String(const basic_string<uint16> &str) {
  string utf8Str;
  for (size_t i = 0; i < str.length(); ++i) {
    WriteUnicodeCharacter(str[i], &utf8Str);
  }
  return utf8Str;
}

string ToUTF8String(uint16 c) {
  string utf8Str;
  WriteUnicodeCharacter(c, &utf8Str);
  return utf8Str;
}

string ToUTF8String(const vector<uint16> &vec) {
  basic_string<uint16> str(vec.begin(), vec.end());
  return ToUTF8String(str);
}

string ToUTF8String(const vector<basic_string<uint16> > &vec) {
  string utf8Str;
  for (size_t i = 0; i < vec.size(); ++i) {
    utf8Str += ToUTF8String(vec[i]);
  }
  return utf8Str;
}

}   // namespace ac_automaton


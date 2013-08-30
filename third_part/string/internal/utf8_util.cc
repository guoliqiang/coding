// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yangzhang@yunrang.com

#include "third_part/string/public/utf8_util.h"

#include <string>

#include "base/public/hash_tables.h"
#include "base/public/string_util.h"
#include "base/public/utf_string_conversion_utils.h"
#include "third_part/icu/include/unicode/uchar.h"

using std::string;
using base::hash_set;

namespace util {
namespace utf8_util {
bool IsSingleUTF8(const string& input) {
  if (input.empty()) {
    return false;
  }
  if (input.size() == 1) {
    return IsStringUTF8(input);
  }
  int start = 0;
  int code_point = -1;
  U8_NEXT(input.c_str(), start, static_cast<int>(input.size()), code_point);
  return (code_point >= 0) && (start == input.size());
}

bool CountUtf8(const std::string& input, int* utf8_num) {
  return CountUtf8(input, NORMAL, utf8_num);
}

bool CountUtf8(const std::string& input,
               CountOption option,
               int* utf8_num) {
  CHECK(utf8_num) << "invalid input utf8_num";
  int start = 0;
  int utf8_count = 0;
  while (start < input.size()) {
    int code_point = -1;
    int end = start;
    U8_NEXT(input.c_str(), end, static_cast<int>(input.size()), code_point);
    if (code_point < 0) {
      VLOG(7) << "break at position " << start;
      return false;
    }
    switch (option) {
      case SKIP_WHITESPACES:
        if (!ContainsOnlyWhitespaceASCII(input.substr(start, end - start))) {
          utf8_count++;
        }
        break;
      default:
        utf8_count++;
        break;
    }
    start = end;
  }
  *utf8_num = utf8_count;
  return true;
}

bool SplitSingleUTF8(const std::string& input,
                     std::vector<std::string>* utf8s) {
  int urf8_num = 0;
  if (!CountUtf8(input, &urf8_num)) {
    return false;
  }
  if (urf8_num <= 0) {
    return false;
  }
  utf8s->clear();
  utf8s->reserve(urf8_num);
  const char *src = input.data();
  int32 src_len = static_cast<int32>(input.length());
  int32 last_index = 0;
  int32 next_index = 0;
  while (next_index < src_len) {
    int32 code_point;
    U8_NEXT(src, next_index, src_len, code_point);
    if (!base::IsValidCharacter(code_point))
      return false;
    utf8s->push_back(input.substr(last_index, next_index - last_index));
    last_index = next_index;
  }
  return true;
}

bool TrimUTF8(const string& input,
              const string& trim_str,
              string* output) {
  if (!IsSingleUTF8(trim_str)) {
    return false;
  }
  int token_pos = 0;
  int last_token_pos = 0;
  int first_good_utf8_pos = 0;
  int last_good_utf8_pos = 0;
  bool matched_from_start = true;
  string word;
  while (token_pos < input.size()) {
    int code_point = -1;
    last_token_pos = token_pos;
    U8_NEXT(input.c_str(),
            token_pos,
            static_cast<int>(input.size()),
            code_point);
    if (code_point < 0) {
      VLOG(7) << "U8_NEXT's start is minus.";
      return false;
    }
    word.assign(input, last_token_pos, token_pos - last_token_pos);
    if (word == trim_str) {
      if (matched_from_start) {
        first_good_utf8_pos = token_pos;
      }
    } else {
      matched_from_start = false;
      last_good_utf8_pos = token_pos;
    }
  }
  if (0 == first_good_utf8_pos &&
      last_good_utf8_pos == input.size()) {
    return false;
  }
  output->assign(input,
                 first_good_utf8_pos,
                 last_good_utf8_pos - first_good_utf8_pos);
  return true;
}

bool RemoveUTF8Char(const string& input,
                    const string& remove_str,
                    string* output) {
  if (!IsSingleUTF8(remove_str)) {
    return false;
  }
  int token_pos = 0;
  int last_token_pos = 0;
  string remained("");
  string word("");
  while (token_pos < input.size()) {
    int code_point = -1;
    last_token_pos = token_pos;
    U8_NEXT(input.c_str(),
            token_pos,
            static_cast<int>(input.size()),
            code_point);
    if (code_point < 0) {
      VLOG(7) << "U8_NEXT's start is minus.";
      return false;
    }
    word.assign(input, last_token_pos, token_pos - last_token_pos);
    if (word != remove_str) {
      remained.append(word);
    }
  }
  if (remained.empty() || remained.size() == input.size()) {
    return false;
  }
  output->swap(remained);
  return true;
}

bool RemoveUTF8Chars(const string& input,
                     const string& remove_str,
                     string* output) {
  // Build the remove char set.
  hash_set<string> remove_char_set;
  int token_pos = 0;
  int last_token_pos = 0;
  string word("");
  while (token_pos < remove_str.size()) {
    int code_point = -1;
    last_token_pos = token_pos;
    U8_NEXT(remove_str.c_str(), token_pos,
              static_cast<int>(remove_str.size()), code_point);
    if (code_point < 0) {
      VLOG(7) << "U8_NEXT's start is minus.";
      return false;
    }
    word.assign(remove_str, last_token_pos, token_pos - last_token_pos);
    remove_char_set.insert(word);
  }

  token_pos = 0;
  last_token_pos = 0;
  word.clear();
  string remained("");
  while (token_pos < input.size()) {
    int code_point = -1;
    last_token_pos = token_pos;
    U8_NEXT(input.c_str(),
            token_pos,
            static_cast<int>(input.size()),
            code_point);
    if (code_point < 0) {
      VLOG(7) << "U8_NEXT's start is minus.";
      return false;
    }
    word.assign(input, last_token_pos, token_pos - last_token_pos);
    if (remove_char_set.find(word) == remove_char_set.end()) {
      remained.append(word);
    }
  }

  output->swap(remained);
  return true;
}

void RemoveNonUTF8Chars(const std::string& input,
                        std::string* output) {
  if (IsStringUTF8(input)) {
    output->assign(input);
    return;
  }
  output->clear();
  const char *src = input.data();
  int32 src_len = static_cast<int32>(input.length());
  int32 last_index = 0;
  int32 next_index = 0;
  bool islastvalid = true;
  while (next_index < src_len) {
    int32 code_point;
    U8_NEXT(src, next_index, src_len, code_point);
    bool isvalid = base::IsValidCharacter(code_point);
    if (islastvalid && isvalid) {
      string toadd = input.substr(last_index, next_index - last_index);
      output->append(input, last_index, next_index - last_index);
    }
    islastvalid = isvalid;
    if (next_index - last_index == 1) {
      islastvalid = true;
    }
    last_index = next_index;
  }
  return;
}
}
}

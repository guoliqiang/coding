// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: yangzhang@yunrang.com (yangzhang)
#ifndef UTIL_STRING_UTF8_UTIL_H_
#define UTIL_STRING_UTF8_UTIL_H_
#include <string>
#include <vector>

namespace util {
namespace utf8_util {

enum CountOption {
  NORMAL = 0,
  SKIP_WHITESPACES,
};

bool IsSingleUTF8(const std::string& utf8);

bool CountUtf8(const std::string& input, int* utf_num);

bool CountUtf8(const std::string& input,
               CountOption option,
               int* utf_num);
bool SplitSingleUTF8(const std::string& input,
                     std::vector<std::string>* utf8s);

// trim the single utf8 character from both start and end
bool TrimUTF8(const std::string& input,
              const std::string& trim_str,
              std::string* output);

// remove all matched utf8 character from input
bool RemoveUTF8Char(const std::string& input,
                    const std::string& remove_str,
                    std::string* output);

bool RemoveUTF8Chars(const std::string& input,
                     const std::string& remove_str,
                     std::string* output);

void RemoveNonUTF8Chars(const std::string& input,
                        std::string* output);

}  // namespace utf8_util
}  // namespace util

#endif  // UTIL_STRING_UTF8_UTIL_H_

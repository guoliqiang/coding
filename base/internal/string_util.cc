// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 10:57:18
// File  : string_util.cc
// Brief :
#include "../public/string_util.h"
namespace base {
template <class STR>
TrimPositions TrimStringT(const STR & input,
                          const typename STR::value_type trim_chars[],
                          TrimPositions positions,
                          STR * output) {
  const typename STR::size_type last_char = input.length() - 1;
  const typename STR::size_type first_good_char = (positions & TRIM_HEAD) ?
                 input.find_first_not_of(trim_chars) : 0;
  const typename STR::size_type last_good_char = (positions & TRIM_TAIL) ?
                 input.find_last_not_of(trim_chars) : last_char;
  if (input.empty() ||
     first_good_char == STR::npos ||
     last_good_char == STR::npos) {
    output->clear();
    return input.empty() ? TRIM_NONE : positions;
  }
  *output = input.substr(first_good_char, last_good_char - first_good_char + 1);
  return static_cast<TrimPositions>(
          ((first_good_char == 0) ? TRIM_NONE : TRIM_HEAD) |
          ((last_good_char == 0) ? TRIM_NONE : TRIM_TAIL));
}
bool TrimString(const std::string & input,
                const char trim_chars[],
                std::string * output) {
  return TrimStringT(input, trim_chars, TRIM_ALL, output) != TRIM_NONE;
}
}  // namespace

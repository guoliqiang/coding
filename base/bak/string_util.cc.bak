// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 10:57:18
// File  : string_util.cc
// Brief :
#include "../public/string_util.h"
#include <vector>
#include <errno.h>
#include <stdio.h>
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
//
bool TrimString(const std::string & input,
                const char trim_chars[],
                std::string * output) {
  return TrimStringT(input, trim_chars, TRIM_ALL, output) != TRIM_NONE;
}
// vsnprintf wrapper
inline int VsnPrintf(char * buffer, size_t size,
                     const char * format, va_list args) {
  return ::vsnprintf(buffer, size, format, args);
  // vsnprintf's return value:the number of characters
}
// for std::string
inline int vsnprintfT(char * buffer, size_t size,
                      const char * format, va_list args) {
  return VsnPrintf(buffer, size, format, args);
}
//
template <class StringType>  // only used in this file
static void StringAppendVT(StringType * dst,
                           const char * format, va_list ap) {
  typename StringType::value_type stack_buf[1024];
  va_list ap_copy;
  GG_VA_COPY(ap_copy, ap);
  errno = 0;
  // see http://baike.baidu.com/view/3485007.htm
  int result = vsnprintfT(stack_buf, arraysize(stack_buf), format, ap_copy);
  va_end(ap_copy);
  if (result >=0 && result < static_cast<int>(arraysize(stack_buf))) {
    dst->append(stack_buf, result);
    return;
  }
  int mem_len = arraysize(stack_buf);
  while (true) {
    if (result < 0) {
      // On Windows, vsnprintfT always returns the number of characters in a
      // fully-formatted string, so if we reach this point, something else is
      //  wrong and no amount of buffer-doubling is going to fix it.
      if (errno != 0 && errno != EOVERFLOW) {
        // If an error other than overflow occurred
        // LOG(WARING) << "Unable to printf the requested string due to error."
        return;
      }
      // doubling the size
      mem_len *= 2;
    } else {
      // we need exactly "result + 1" characters.
      mem_len = result + 1;
    }
    if (mem_len > 32 * 1024 * 1024) {
      // That should be plenty, don't try anything larger.  This protects
      // against huge allocations when using vsnprintfT implementations that
      // return -1 for reasons other than overflow without setting errno.
      // LOG(WARNING) << "Unable to printf the requested string due to size.";
      return;
    }
    std::vector<typename StringType::value_type> mem_buf(mem_len);
    // NOTE: You can only use a va_list once.  Since we're in a while loop, we
    // need to make a new copy each time so we don't use up the original.
    GG_VA_COPY(ap_copy, ap);
    result = vsnprintfT(&mem_buf[0], mem_len, format, ap_copy);
    va_end(ap_copy);
    if (result >= 0 && result < mem_len) {
      dst->append(&mem_buf[0], result);
      return;
    }
  }
}
//
void StringAppendV(std::string * dst, const char * format, va_list ap) {
  StringAppendVT(dst, format, ap);
}
//
const std::string StringPrintf(const char * format, ...) {
  va_list ap;
  va_start(ap, format);
  // Accrding the position of fromat in this stack of the function
  // ap can point to the start of the variable argument(...)
  std::string _return;
  StringAppendV(&_return, format, ap);
  // ap point tO NULL
  va_end(ap);
  // see http://blog.sina.com.cn/s/blog_477815290100cxtk.html
  return _return;
}
//
const std::string& SStringPrintf(std::string* dst, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  dst->clear();
  StringAppendV(dst, format, ap);
  va_end(ap);
  return *dst;
}
}  // namespace

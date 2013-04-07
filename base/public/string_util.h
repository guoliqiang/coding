// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 10:51:44
// File  : string_util.h
// Brief :

#ifndef  BASE_PUBLIC_STRING_UTIL_H_
#define  BASE_PUBLIC_STRING_UTIL_H_
#include "./basictypes.h"
#include <stdarg.h>
#include <string>
namespace base {
enum TrimPositions {
  TRIM_NONE     = 0,
  TRIM_HEAD     = 1 << 0,
  TRIM_TAIL     = 1 << 1,
  TRIM_ALL      = TRIM_HEAD | TRIM_TAIL
};
bool TrimString(const std::string & input,
                const char trim_chars[],
                std::string * output);
// StringPrintf
// http://wenku.baidu.com/view/b8c67adda58da0116c1749f4.html
const std::string StringPrintf(const char * format, ...) PRINTF_FORMAT(1, 2);
const std::string& SStringPrintf(std::string* dst, const char* format, ...) PRINTF_FORMAT(2, 3);
void StringAppendV(std::string * dst,
                   const char * fromat, va_list) PRINTF_FORMAT(2, 0);
int VsnPrintf(char * buffer, size_t size, const char * format, va_list args)
    PRINTF_FORMAT(3, 0);
}  // namespace
#endif  // BASE_PUBLIC_STRING_UTIL_H_

#ifndef RUNE_H_
#define RUNE_H_

#include <string>
#include "base/public/basictypes.h"

namespace base {
// Code-point values in Unicode 4.0 are 21 bits wide.
// http://www.cnblogs.com/chenwenbiao/archive/2011/08/11/2134503.html
enum {
  Bit1    = 7,
  Bitx    = 6,
  Bit2    = 5,
  Bit3    = 4,
  Bit4    = 3,
  Bit5    = 2,

  T1      = ((1<<(Bit1+1))-1) ^ 0xFF,     // 0000 0000
  Tx      = ((1<<(Bitx+1))-1) ^ 0xFF,     // 1000 0000
  T2      = ((1<<(Bit2+1))-1) ^ 0xFF,     // 1100 0000
  T3      = ((1<<(Bit3+1))-1) ^ 0xFF,     // 1110 0000
  T4      = ((1<<(Bit4+1))-1) ^ 0xFF,     // 1111 0000
  T5      = ((1<<(Bit5+1))-1) ^ 0xFF,     // 1111 1000

  Rune1   = (1<<(Bit1+0*Bitx))-1,         // 0000 0000 0111 1111
  Rune2   = (1<<(Bit2+1*Bitx))-1,         // 0000 0111 1111 1111
  Rune3   = (1<<(Bit3+2*Bitx))-1,         // 1111 1111 1111 1111
  Rune4   = (1<<(Bit4+3*Bitx))-1,         
  // 0001 1111 1111 1111 1111 1111

  Maskx   = (1<<Bitx)-1,                  // 0011 1111
  Testx   = Maskx ^ 0xFF,                 // 1100 0000

  Bad     = 0xFFFD,
};


inline int utfcharsize(const char *str) {
  // note: int and int64
  int c, c1, c2, c3;
  int64 l;

  // one character sequence
  //       00000-0007F => T1
  c = *(unsigned char*)str;
  if (c < Tx) {
    return 1;
  }

  // two character sequence
  //      0080-07FF => T2 Tx
  c1 = *(unsigned char*)(str+1) ^ Tx;
  if (c1 & Testx)
    goto bad;
  if (c < T3) {
    if (c < T2)
      goto bad;
    l = ((c << Bitx) | c1) & Rune2;
    if (l <= Rune1)
      goto bad;
    return 2;
  }

  // three character sequence
  //      0800-FFFF => T3 Tx Tx
  c2 = *(unsigned char*)(str+2) ^ Tx;
  if (c2 & Testx)
    goto bad;
  if (c < T4) {
    l = ((((c << Bitx) | c1) << Bitx) | c2) & Rune3;
    if (l <= Rune2)
      goto bad;
    return 3;
  }

  // four character sequence (21-bit value)
  //      10000-1FFFFF => T4 Tx Tx Tx
  c3 = *(unsigned char*)(str+3) ^ Tx;
  if (c3 & Testx)
    goto bad;
  if (c < T5) {
    l = ((((((c << Bitx) | c1) << Bitx) | c2) << Bitx) | c3) & Rune4;
    if (l <= Rune3)
      goto bad;
    return 4;
  }

  // Support for 5-byte or longer UTF-8 would go here, but
  // since we don't have that, we'll just fall through to bad.

  // bad decoding
  bad:
  return 1;
}

inline int UTF8Length(const std::string str) {
  const char * ptr = str.c_str();
  int len = str.length();
  int rs = 0;
  do {
    int foo = utfcharsize(ptr);
    len -= foo;
    ptr += foo;
    rs ++;
  } while (len > 0);
  return rs;
}

inline int UTF8Length(const char * ptr) {
  std::string foo(ptr);
  return UTF8Length(foo);
}

}
#endif  // 


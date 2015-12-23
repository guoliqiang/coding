// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-07-27 10:27:11
// File  : bit_util.h
// Brief :

#ifndef  __BIT_UTIL_H_
#define  __BIT_UTIL_H_

// http://wenku.baidu.com/link?url=LXUP6BM5wp9H-aJj8NE61vUXq-
// 7R3JfPRNZfE0FBftMLiTJtNjefSFBtgO20pMp8JEgmeVYh3_NUscjoGgJCNZjgZ2XrOtmJX4HaDAxjZFq
#define clear_block(addr, size) \
__asm__ __volatile__("cld\n\t" \
  "rep\n\t" \
  "stosl" \
  ::"a" (0), "c" (size/4), "D" (reinterpret_cast<int64>(addr)))

#define set_bit(nr, addr) ({\
register int res ; \
__asm__ __volatile__("btsl %2,%3\n\tsetb %%al": \
  "=a" (res):"0" (0), "r" (nr), "m" (*(addr))); \
  res;})

#define clear_bit(nr, addr) ({\
register int res ; \
__asm__ __volatile__("btrl %2,%3\n\tsetnb %%al": \
  "=a" (res):"0" (0), "r" (nr), "m" (*(addr))); \
  res;})

#define find_first_zero(addr) ({ \
int __res; \
__asm__ __volatile__("cld\n" \
  "1:\tlodsl\n\t" \
  "notl %%eax\n\t" \
  "bsfl %%eax,%%edx\n\t" \
  "je 2f\n\t" \
  "addl %%edx,%%ecx\n\t" \
  "jmp 3f\n" \
  "2:\taddl $32,%%ecx\n\t" \
  "cmpl $8192,%%ecx\n\t" \
  "jl 1b\n" \
  "3:" \
  :"=c" (__res):"c" (0), "S" (addr)); \
__res;})

#endif  //  __BIT_UTIL_H_

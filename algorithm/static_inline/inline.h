// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-05-11 00:03:05
// File  : inline.h
// Brief :

#ifndef  __INLINE_H_
#define  __INLINE_H_

namespace algorithm {
// inline function declare and define must stay tegother!
inline void GlobalInline(int) {
}

class FooInline {
 public:
  inline void FooInlineFunc(int) {
  }
};
}  // namespace algorithm

#endif  //__INLINE_H_

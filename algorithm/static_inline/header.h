// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-05-10 23:13:31
// File  : header.h
// Brief :

// http://stackoverflow.com/questions/185624/
// static-variables-in-an-inlined-function

#ifndef  __HEADER_H_
#define  __HEADER_H_

#include "base/public/logging.h"

namespace algorithm {

inline void InlineFunc(int num) {
  static int value = 10;
  LOG(INFO) << "change value from " << value << " to " << num;
  value = num;
}

static void StaticFunc(int num) {
  static int value = 10;
  LOG(INFO) << "change value from " << value << " to " << num;
  value = num;
}


inline static void StaticInlineFunc(int num) {
  static int value = 10;
  LOG(INFO) << "change value from " << value << " to " << num;
  value = num;
}

}  // namespace algorithm

#endif  //__HEADER_H_

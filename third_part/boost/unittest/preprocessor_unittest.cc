// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 00:24:14
// File  : preprocessor_unittest.cc
// Brief :

#include "base/public/basictypes.h"
#include "third_part/boost/include/boost/preprocessor/library.hpp"

#define MAP_EACH_PROC(r, data, elem) (BOOST_PP_STRINGIZE(elem))(elem)

#define MAP_IMPL(...) \
  BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_FOR_EACH(\
  MAP_EACH_PROC, 0, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

int main(int argc, char** argv) {
#pragma message(PRINT_MACRO(MAP_IMPL(x, y, z)))
  return 0;
}

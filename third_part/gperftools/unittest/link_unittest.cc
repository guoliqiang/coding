// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2015-06-15 19:50:40
// File  : link_unittest.cc
// Brief :

#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include "base/public/logging.h"

void Foo();
void Bar();

#define ATTRIBUTE_WEAK      __attribute__((weak))
#define ATTRIBUTE_SECTION(name) __attribute__ ((section (#name)))
# define DECLARE_ATTRIBUTE_SECTION_VARS(name) \
     extern char __start_##name[] ATTRIBUTE_WEAK; \
     extern char __stop_##name[] ATTRIBUTE_WEAK

# define ATTRIBUTE_SECTION_START(name) (reinterpret_cast<void*>(__start_##name))
# define ATTRIBUTE_SECTION_STOP(name) (reinterpret_cast<void*>(__stop_##name)) 
 
void F1() ATTRIBUTE_SECTION(google_malloc);
void F1(){ }

void F2() ATTRIBUTE_SECTION(google_malloc);
void F2(){ }

DECLARE_ATTRIBUTE_SECTION_VARS(google_malloc);

// use the first one in the link sequence
int main(int argc, char** argv) {
  Foo();
  Bar();
  if (ATTRIBUTE_SECTION_START(google_malloc) ==
      ATTRIBUTE_SECTION_STOP(google_malloc)) {
    LOG(INFO) << "equal";
  } else {
    LOG(INFO) << "noequal";
  }
  CHECK(false);
  return 0;
}

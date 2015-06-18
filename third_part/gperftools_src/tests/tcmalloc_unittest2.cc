#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include "third_part/gperftools_src/gperftools/malloc_extension_c.h"
#include "third_part/gperftools_src/gperftools/malloc_hook_c.h"

#define FAIL(msg) do {                          \
  fprintf(stderr, "FATAL ERROR: %s\n", msg);    \
  exit(1);                                      \
} while (0)


void TestMallocHook(void) {
  void* result[5];
  if (MallocHook_GetCallerStackTrace(result, sizeof(result)/sizeof(*result),
                                     0) < 2) {
  }
}


int main(int argc, char** argv) {
  TestMallocHook();
  return 0;
}

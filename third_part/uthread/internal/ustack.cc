// Platform specific stack allocation routines.
// See the copyright notice in uthread.h.

#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "third_part/uthread/public/ustack.h"
#include "base/public/logging.h"

namespace uthread {

const static int UTHREAD_STACK_SIZE = 16 * 1024;

// http://see.xidian.edu.cn/cpp/html/138.html
void * uthread_alloc_stack(int * size) {
  int page_size = static_cast<int>(sysconf(_SC_PAGESIZE));
  void * stack;
  int rem;
  if (*size <= 0) *size = UTHREAD_STACK_SIZE;
  rem = *size % page_size;
  if (rem != 0) *size += page_size - rem;
  stack = mmap(NULL, *size, PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_GROWSDOWN | MAP_ANONYMOUS, -1, 0);
  CHECK(stack != MAP_FAILED) << "mmap failed!";
  VLOG(5) << "alloc stack size = " << *size;
  return stack;
}

void uthread_free_stack(void * stack, int size) {
  munmap(stack, size);
}

}  // namespace uthread

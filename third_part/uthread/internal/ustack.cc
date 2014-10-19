// Platform specific stack allocation routines.
// See the copyright notice in uthread.h.

#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "../public/ustack.h"
#include "../public/uassert.h"

#define UTHREAD_STACK_SIZE (16*1024)

void * uthread_alloc_stack(int *size) {
  int page_size = static_cast<int>(sysconf(_SC_PAGESIZE));
  void *stack;
  int rem;
  if (*size <= 0) *size = UTHREAD_STACK_SIZE;
  rem = *size % page_size;
  if (rem != 0) *size += page_size - rem;
  stack = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
               MAP_GROWSDOWN | MAP_ANONYMOUS, -1, 0);
  uthread_assert(stack != MAP_FAILED);
  return stack;
}

void uthread_free_stack(void *stack, int size) {
  munmap(stack, size);
}

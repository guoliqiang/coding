// Platform specific execution context swapping routines.
// See the copyright notice in uthread.h.

#include "../public/uswap.h"
#include <pthread.h>
#include "../public/ustack.h"

static pthread_key_t uthread_key;

static void uthread_init_key(void) {
  pthread_key_create(&uthread_key, NULL);
}

void * uthread_context_data(void) {
  return pthread_getspecific(uthread_key);
}

void uthread_context_exit(void) {
  pthread_setspecific(uthread_key, NULL);
}

void uthread_context_init(uthread_context_t *self, void *data) {
  static pthread_once_t once = PTHREAD_ONCE_INIT;
  self->uc_stack.ss_sp = NULL;
  self->uc_stack.ss_size = 0;
  pthread_once(&once, uthread_init_key);
  pthread_setspecific(uthread_key, data);
}

void uthread_context_setup(uthread_context_t *other, int stack_size,
    void (*func)(void *), void *data) {
  getcontext(other);
  other->uc_stack.ss_sp = uthread_alloc_stack(&stack_size);
  other->uc_stack.ss_size = stack_size;
  other->uc_link = NULL;
  makecontext(other, (void (*)(void))func, 1, data);
}

void uthread_context_swap(uthread_context_t *self, uthread_context_t *other) {
  swapcontext(self, other);
}

void uthread_context_jump(uthread_context_t *other) {
  setcontext(other);
}

void uthread_context_close(uthread_context_t *other) {
  uthread_free_stack(other->uc_stack.ss_sp, other->uc_stack.ss_size);
}

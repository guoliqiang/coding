// Platform specific execution context swapping routines.
// See the copyright notice in uthread.h.

#ifndef uswap_H
#define uswap_H

#include <ucontext.h>

namespace uthread {

typedef ucontext_t uthread_context_t;
void uthread_context_init(uthread_context_t *ctx, void *data);
// note: this function behaves in a platform specific way!
void *uthread_context_data(void);
void uthread_context_setup(uthread_context_t *ctx, int stack_size,
                           void (*start_func)(void *), void *data);
void uthread_context_swap(uthread_context_t *current, uthread_context_t *other);
void uthread_context_jump(uthread_context_t *ctx);
void uthread_context_close(uthread_context_t *ctx);
void uthread_context_exit(void);

}  // namespace utread

#endif

#ifndef ustack_H
#define ustack_H

namespace uthread {

void * uthread_alloc_stack(int *size);
void uthread_free_stack(void *stack, int size);

}  // namespace uthread

#endif

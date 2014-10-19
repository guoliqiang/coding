#ifndef ustack_H
#define ustack_H

void *uthread_alloc_stack(int *size);
void uthread_free_stack(void *stack, int size);

#endif

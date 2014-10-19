#ifndef uthread_H
#define uthread_H

// uthread - A user mode threading library
// version: 0.1.1, Sun Jan 7, 2007
// author:  Wim Couwenberg
// website: http://members.chello.nl/~w.couwenberg/uthread.html
// See the copyright notice at the end of this file.

// uthread_state_t
enum {
	UTHREAD_NONSIGNALED,
	UTHREAD_SIGNALED,
	UTHREAD_CLOSED
};

typedef struct uthread_t * uthread_h;
typedef void (*uthread_func_t)(void *args);

void uthread_init(void);

int uthread_wait(uthread_h object);
void uthread_close(uthread_h object);
int uthread_run(void);

uthread_h uthread_create(int stack_size, uthread_func_t start_func, void *args);

void uthread_exit(void);
uthread_h uthread_self(void);
void uthread_yield(void);

uthread_h uthread_event_create(void);
void uthread_event_broadcast(uthread_h event);

uthread_h uthread_queue_create(void);
void uthread_queue_add(uthread_h queue, uthread_h object);
int uthread_queue_get(uthread_h queue, uthread_h *object, int *state);
void uthread_queue_reset(uthread_h queue);

#endif

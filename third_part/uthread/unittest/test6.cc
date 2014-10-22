#include <stdlib.h>
#include "../public/uthread.h"
using namespace uthread;

#define SWITCHES 10000000

static void func(void *arg)
{
	int i;

	for (i = 0; i < SWITCHES; ++i)
		uthread_yield();
}

static int events;
static uthread_h th1;
static uthread_h th2;
static uthread_h th3;
static uthread_h que;

int main(void)
{
	uthread_init();

	th1 = uthread_create(0, func, NULL);
	th2 = uthread_create(0, func, NULL);
	th3 = uthread_create(0, func, NULL);

	que = uthread_queue_create();
	uthread_queue_add(que, th1);
	uthread_queue_add(que, th2);
	uthread_queue_add(que, th3);

	for (events = 0; events < 3; ++events)
	{
		uthread_h object;
		int state;

		while (uthread_queue_get(que, &object, &state) == 0)
			uthread_wait(que);
	}

	uthread_exit();
	return 0;
}

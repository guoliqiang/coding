
#include <stdio.h>
#include "../public/uthread.h"

using namespace uthread;
static void func(void *arg)
{
	int i;
	uthread_h ev = (uthread_h)arg;

	printf("in func.\n");

	for (i = 1; i <= 10; ++i)
	{
		printf("\titeration %d.\n", i);
		uthread_wait(ev);
	}

	printf("func done.\n");
}

int main(void)
{
	uthread_h ev;
	uthread_h th;
	int rc;

	uthread_init();

	ev = uthread_event_create();
	th = uthread_create(0, func, ev);

	while (rc = uthread_run(), rc != UTHREAD_SIGNALED)
	{
		printf("back in main, but thread still active.\n");
		printf("signalling event...\n");
		uthread_event_broadcast(ev);
	}

	printf("back in main, all is quiet...\n");
	printf("closing event.\n");
	uthread_close(ev);
	printf("closing thread.\n");
	uthread_close(th);
	printf("exiting...\n");
	uthread_exit();

	return 0;
}

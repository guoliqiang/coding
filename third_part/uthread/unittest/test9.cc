
#include <stdio.h>
#include "../public/uthread.h"

using namespace uthread;
static void func(void *arg)
{
	(void)arg;

	printf("in thread %p.  Let's wait...\n", uthread_self());

	/* a nice deadlock */
	uthread_wait(uthread_self());
}

int main(void)
{
	int rc;
	uthread_h th;

	uthread_init();
	th = uthread_create(0, func, NULL);

	printf("waiting for thread %p.\n", th);
	rc = uthread_wait(th);

	if (rc == UTHREAD_SIGNALED)
		printf("In main: a miracle!?!\n");
	else
		printf("In main: shot himself did he?\n");

	printf("closing thread %p.\n", th);
	uthread_close(th);

	uthread_exit();
	return 0;
}

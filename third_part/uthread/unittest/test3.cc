
#include <stdio.h>
#include "../public/uthread.h"

struct func_arg
{
	const char *msg;
	uthread_h event;
};

void func(void *arg)
{
	struct func_arg *fa = (struct func_arg *)arg;
	printf("executing func, message is \"%s\"\n", fa->msg);
	printf("broadcast on event %p...\n", fa->event);
	uthread_event_broadcast(fa->event);
}

int main(void)
{
	struct func_arg fa;
	uthread_h th;
	uthread_h ev;
	int result;

	uthread_init();

	th = uthread_create(0, func, &fa);
	printf("created thread %p\n", th);

	ev = uthread_event_create();
	printf("created event %p\n", ev);

	fa.msg = "hi there!";
	fa.event = ev;

	printf("waiting for event...\n");
	result = uthread_wait(ev);

	printf("back in main, wait returned %d\n", result);
	printf("closing event %p\n", ev);
	uthread_close(ev);

	printf("exiting uthread...\n");
	uthread_exit();

	printf("done\n");
	return 0;
}

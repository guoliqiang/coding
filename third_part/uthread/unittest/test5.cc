
#include <stdlib.h>
#include <stdio.h>
#include "../public/uthread.h"

using namespace uthread;
struct func_arg
{
	const char *name;
	int count;
	int exit_when_done;
};

void func(void *arg)
{
	int i;
	struct func_arg *fa = (struct func_arg *)arg;
	uthread_h self = uthread_self();

	printf("%s: thread %p is counting to %d\n", fa->name, self, fa->count);

	for (i = 1; i <= fa->count; ++i)
	{
		printf("%s:\t%d\n", fa->name, i);
		uthread_yield();
	}

	if (fa->exit_when_done)
	{
		printf("%s: exit.\n", fa->name);
		uthread_exit();
	}

	printf("%s: done.\n", fa->name);
}

uthread_h th1;
uthread_h th2;
uthread_h que;

struct func_arg arg1;
struct func_arg arg2;

int main(void)
{
	int events;

	printf("initializing uthread.\n");
	uthread_init();

	printf("creating two threads.\n");

	arg1.name = "func 1";
	arg1.count = 8;
	arg1.exit_when_done = 1;
	th1 = uthread_create(0, func, &arg1);

	arg2.name = "func 2";
	arg2.count = 3;
	arg2.exit_when_done = 0;
	th2 = uthread_create(0, func, &arg2);
	printf("threads %p and %p.\n", th1, th2);

	printf("creating a queue.\n");
	que = uthread_queue_create();
	printf("queue %p.\n", que);

	printf("adding threads to queue.\n");
	uthread_queue_add(que, th1);
	uthread_queue_add(que, th2);

	for (events = 0; events < 2; )
	{
		uthread_h object;
		int state;

		printf("waiting for events from queue.\n");

		uthread_wait(que);
		printf("fetching events...\n");

		while (uthread_queue_get(que, &object, &state))
		{
			++events;
			printf("\tevent %d from %p, result %d\n", events, object, state);

			if (state == UTHREAD_SIGNALED)
			{
				printf("checking if thread %p is still signaled...\n", object);
				state = uthread_wait(object);

				if (state == UTHREAD_SIGNALED)
					printf("\t...yes.\n");
				else
					printf("\t...no? State is %d\n", state);

				printf("\tclosing object %p.\n", object);
				uthread_close(object);
			}
			else if (state == UTHREAD_CLOSED)
			{
				printf("\tobject %p was closed.\n", object);
			}
			else
			{
				printf("\tunexpected result from wait... stopping.\n");
				uthread_exit();
				exit(1);
			}
		}
	}

	printf("closing queue.\n");
	uthread_close(que);

	printf("exiting uthread.\n");
	uthread_exit();

	return 0;
}

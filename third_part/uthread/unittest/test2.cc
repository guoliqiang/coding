
#include <stdio.h>
#include "../public/uthread.h"

void func(void *arg)
{
	printf("executing func, message is \"%s\"\n", (char *)arg);
	printf("exiting...\n");
	uthread_exit();
}

int main(void)
{
	uthread_h th;
	int result;

	uthread_init();

	th = uthread_create(0, func, (void *)"hi there!");
	printf("created thread %p\n", th);

	printf("waiting for thread...\n");
	result = uthread_wait(th);

	printf("back in main, wait returned %d, exiting\n", result);
	uthread_exit();

	printf("done\n");
	return 0;
}

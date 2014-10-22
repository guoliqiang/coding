#include <stdio.h>
#include "third_part/uthread/public/uthread.h"
#include "base/public/logging.h"

using namespace uthread;

void func(void *arg) {
	printf("executing func, message is \"%s\" %p \n", (const char *)arg,
      uthread_self());
}

int main(void)
{
	uthread_h th;
	int result;

	uthread_init();

	th = uthread_create(0, func, (void *)"hi there!");
	printf("created thread %p\n", th);

	printf("waiting for thread...%p\n", th);
	result = uthread_wait(th);

	printf("back in main, wait returned %d, exiting\n", result);
	uthread_exit();

	printf("done\n");
	return 0;
}

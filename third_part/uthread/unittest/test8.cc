
#include <stdio.h>
#include "../public/uthread.h"

using namespace uthread;
static int smash = 0;

static void ping(void *arg)
{
	uthread_h self = uthread_self();

	(void)arg;

	while (smash == 0)
	{
		printf("thread %p: ping.\n", self);
		uthread_yield();
	}

	printf("thread %p: oww...\n", self);
}

static void pong(void *arg)
{
	int i;
	uthread_h self = uthread_self();

	(void)arg;

	for (i = 0; i < 10; ++i)
	{
		printf("thread %p: pong.\n", self);
		uthread_yield();
	}

	printf("thread %p: smash!\n", self);
	smash = 1;

	printf("player pong exits (optimistically).\n");
	uthread_exit();
}

int main(void)
{
	uthread_h pinger;
	uthread_h ponger;
	int rc;

	uthread_init();

	printf("setting up players...\n");
	pinger = uthread_create(0, ping, NULL);
	ponger = uthread_create(0, pong, NULL);

	printf("starting game!\n");
	rc = uthread_run();

	if (rc == UTHREAD_SIGNALED)
		printf("game ended.\n");
	else
		printf("game suspended???  That doesn't sound right...\n");

	printf("Player ping exits.\n");
	uthread_close(pinger);

	uthread_exit();
	return 0;
}

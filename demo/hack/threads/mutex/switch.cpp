#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils/TimeStamp.h"
#include "utils/StatVector.h"

/*
 * Measures the context switch speed using pthread conditonal variable and the
 * x86_64 rdtsc instruction. Actual measurement includes both the context switch
 * and conditonal variable signaling time. Granularity is CPU cycle.
 */

static double start;
static double end;
static size_t COUNTER = 0;
static pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t COND = PTHREAD_COND_INITIALIZER;

static void *_poster(void *args)
{
	pthread_mutex_lock(&MUTEX);
	COUNTER = 1;
	pthread_mutex_unlock(&MUTEX);

	TimeStamp clk;
	start = clk.now();
	pthread_cond_signal(&COND);

	return NULL;
}

static void *_waiter(void *args)
{
	pthread_mutex_lock(&MUTEX);
	TimeStamp clk;
	while (COUNTER != 1lu)
		pthread_cond_wait(&COND,&MUTEX);
	end = clk.now();
	COUNTER = 0;
	pthread_mutex_unlock(&MUTEX);

	return NULL;
}

int main(int argc, char const* argv[])
{
	const int n = 10000;
	StatVector s(n);
	for (int i = 0; i < n; i++) {
		pthread_t t1;
		pthread_t t2;
		pthread_create(&t1,NULL,_waiter,NULL);
		pthread_create(&t2,NULL,_poster,NULL);
		pthread_join(t1,NULL);
		pthread_join(t2,NULL);
		s.insert(end - start);
		//printf("%5d: %.0f cycles\n",i,end - start);
	}
	printf("%d trials, median = %.0f cycles\n",n,s.median());

	return 0;
}

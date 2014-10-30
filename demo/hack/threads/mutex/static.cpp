#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Demonstrate the usage of statically allocated pthread mutex
 */

static size_t COUNTER = 0;
static pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

static void *_increment_counter(void *args)
{
	int label = *((int*)args);
	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	pthread_mutex_lock(&MUTEX);

	printf("label = %d, pid(%lu) = %d, tid(%lu) = %lu\n",
			label,sizeof(pid),pid,sizeof(tid),tid);
	printf("\tthread %d: COUNTER = %lu\n",label,COUNTER);
	COUNTER++;
	printf("\tthread %d: COUNTER = %lu\n",label,COUNTER);

	pthread_mutex_unlock(&MUTEX);

	return NULL;
}

int main(int argc, char const* argv[])
{
	pid_t pid = getpid();
	printf("PID = %d\n",pid);

	pthread_t t1;
	pthread_t t2;

	int label1 = 1;
	int label2 = 2;

	pthread_create(&t1,NULL,_increment_counter,&label1);
	pthread_create(&t2,NULL,_increment_counter,&label2);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf("COUNTER = %lu\n",COUNTER);

	return 0;
}

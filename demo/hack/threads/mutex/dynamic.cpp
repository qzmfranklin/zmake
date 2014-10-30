#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Demonstrate the usage of dynamically allocated pthread mutex
 */

typedef struct st_thread_data {
	int label;
	pthread_mutex_t *mutex;
} thread_data;

static size_t COUNTER = 0;

static void *_increment_counter(void *args)
{
	thread_data *data = (thread_data*) args;
	int label = data->label;
	pthread_mutex_t *mutex = data->mutex;

	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	pthread_mutex_lock(mutex);

	printf("label = %d, pid(%lu) = %d, tid(%lu) = %lu\n",
			label,sizeof(pid),pid,sizeof(tid),tid);
	printf("\tthread %d: COUNTER = %lu\n",label,COUNTER);
	COUNTER++;
	printf("\tthread %d: COUNTER = %lu\n",label,COUNTER);

	pthread_mutex_unlock(mutex);

	return NULL;
}

int main(int argc, char const* argv[])
{
	pid_t pid = getpid();
	printf("PID = %d\n",pid);

	pthread_t t1;
	pthread_t t2;

	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex,NULL);
	thread_data data1 = {1,&mutex};
	thread_data data2 = {2,&mutex};

	pthread_create(&t1,NULL,_increment_counter,&data1);
	pthread_create(&t2,NULL,_increment_counter,&data2);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_mutex_destroy(&mutex);

	printf("COUNTER = %lu\n",COUNTER);

	return 0;
}

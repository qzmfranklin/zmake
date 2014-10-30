#include <stdio.h>
#include <pthread.h>

/*
 * After pthread_create(), who is scheduled to run first? The parent or the
 * child?
 */

static int FLAG;
static size_t TOTAL = 0;
static size_t CHILD_FIRST = 0;
static size_t PARENT_FIRST = 0;
static pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

static void *child(void *args)
{
	pthread_mutex_lock(&MUTEX);
	if (FLAG) {
		TOTAL++;
		CHILD_FIRST++;
		FLAG = 0;
	}
	pthread_mutex_unlock(&MUTEX);
	return NULL;
}

static void *parent(void *args)
{
	pthread_t t1;
	pthread_create(&t1,NULL,child,NULL);

	pthread_mutex_lock(&MUTEX);
	if (FLAG) {
		TOTAL++;
		PARENT_FIRST++;
		FLAG = 0;	
	}
	pthread_mutex_unlock(&MUTEX);

	pthread_join(t1,NULL);
	return NULL;
}

int main(int argc, char const* argv[])
{
	const int n = 5*100*1000;
	for (int i = 0; i < n; i++) {
		FLAG = 1;
		pthread_t t1;
		pthread_create(&t1,NULL,parent,NULL);
		pthread_join(t1,NULL);
	}
	printf("TOTAL = %lu, "
			"CHILD_FIRST = %lu (%.3f%%), "
			"PARENT_FIRST = %lu (%.3f%%)\n",
			TOTAL,
			CHILD_FIRST,  100.0 * CHILD_FIRST  / TOTAL,
			PARENT_FIRST, 100.0 * PARENT_FIRST / TOTAL);
	return 0;
}

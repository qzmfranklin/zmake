#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "TimeStat.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static const int QUEUE_SIZE=500;
struct queue_t {
	int data[QUEUE_SIZE];
	int start;
	int end;
	int size;
};

struct queue_t *queue_create(void)
{
	struct queue_t *q;
	q=(struct queue_t *)malloc(sizeof(struct queue_t));
	assert(q);
	memset(q,0,sizeof(struct queue_t));
	return q;
}

void queue_destroy(struct queue_t *q)
{
	free(q);
}

void queue_push(struct queue_t *q, int val)
{
	assert(q->size<QUEUE_SIZE);
	q->data[q->end]=val;
	q->end = (q->end+1) % QUEUE_SIZE;
	q->size++;
}

void queue_pop(struct queue_t *q)
{
	assert(q->size);
	q->start = (q->start+1) % QUEUE_SIZE;
	q->size--;
} 

int queue_top(struct queue_t *q)
{
	assert(q->size);
	return q->data[q->start];
} 

struct magic3_t {
	int prev;
	int base[3];
	struct queue_t *queue[3];
};

struct magic3_t *magic3_create(const int base[3])
{
	struct magic3_t *m;
	m=(struct magic3_t*)malloc(sizeof(struct magic3_t));
	assert(m);
	m->prev = 1;
	for (int i = 0; i < 3; i++) {
		m->base[i] = base[i];
		m->queue[i]= queue_create();
	}
	return m;
}

void magic3_destroy(struct magic3_t *m)
{
	for (int i = 0; i < 3; i++)
		queue_destroy(m->queue[i]);
	free(m);
}

// pop the smallest of the the three queues
static int reap3(struct queue_t *q[3])
{
	int a[3];
	for (int i = 0; i < 3; i++)
		a[i] = queue_top(q[i]);
	if (a[0]==a[1]) {
		if (a[0]==a[2]) {
			queue_pop(q[0]);
			queue_pop(q[1]);
			queue_pop(q[2]);
			return a[0];
		} else if (a[0]<a[2]) {
			queue_pop(q[0]);
			queue_pop(q[1]);
			return a[0];
		} else { 
			queue_pop(q[2]);
			return a[2];
		}
	} else if (a[0]<a[1]) {
		if (a[0]<a[2]) {
			queue_pop(q[0]);
			return a[0];
		} else { 
			if (a[0]==a[2])
				queue_pop(q[0]);
			queue_pop(q[2]);
			return a[2];
		}
	} else {
		if (a[1]<a[2]) {
			queue_pop(q[1]);
			return a[1];
		} else {
			if (a[1]==a[2])
				queue_pop(q[1]);
			queue_pop(q[2]);
			return a[2];
		}
	}
}

int magic3_next(struct magic3_t *m)
{
	for (int i = 0; i < 3; i++)
		queue_push(m->queue[i], m->prev * m->base[i]);
	m->prev = reap3(m->queue);
	return m->prev;
}


int main(int argc, char const* argv[])
{
	printf("Generate the sequence of magic numbers, i.e., numbers\n");
	printf("of the form 2^m*3^n*5^p in the increasing order.\n");

	const int base[3]={2,3,5};
	const int num=200;

	TimeStat clock(num+1);
	struct magic3_t *m = magic3_create(base);
	for (int i = 0; i < num; i++) {
		clock.tic();
		int mag = magic3_next(m);
		double cyc = clock.toc();
		printf("[%6d] [%.0f cycles] %15d \n",i+1,cyc,mag);
	}
	magic3_destroy(m);

	printf("num=\t%d\n",num);
	printf("cycles per magic number:\n");
	printf("\tmin\tmedian\tmax\n");
	printf("\t%.0f\t%.0f\t%.0f\n",clock.min(),clock.median(),clock.max());


	return 0;
}

#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>

/*
 * Min Binary Heap
 */
class heap {
private:
	int status;
	int size;
	int numitem;
	int *data;

public:
	heap(const int n = 10) {
		data = new int[n];
		assert(data);
		size = n;
		numitem = 0;
		status = 1;
	}

	~heap() {
		if (status == 1)
			free(data);
	}

	int top() const { return data[0]; }

	void pop();

	void push(const int val);

	void resize(const int n);

	void print();

private:
	void sink(int k);
	void rise(int k);

};


#endif /* end of include guard */

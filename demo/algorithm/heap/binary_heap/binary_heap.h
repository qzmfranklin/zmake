#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include "../heap.h"

template<typename T>
class binary_heap: public heap<T> {
private:
	int status;
	int size;
	int numitem;
	int *data;

public:
	binary_heap(const int n = 10) {
		data = new int[n];
		assert(data);
		size = n;
		numitem = 0;
		status = 1;
	}

	virtual ~binary_heap() {
		if (status == 1)
			free(data);
	}

	virtual T top() const { return data[0]; }

	virtual void pop();

	virtual void push(const T val);

	void resize(const int n);

	void print_debug();

private:
	void sink(int k);
	void rise(int k);

};

template<typename T> 
inline void binary_heap<T>::sink(int k)
{
	while (1) {
		if (2*k+1 > numitem-1)
			break;
		if (2*k+2 > numitem-1) {
			if (data[k] > data[2*k+1])
				::std::swap(data[k], data[2*k+1]);
			break;
		}
		const int left  = data[2*k + 1];
		const int right = data[2*k + 2];
		if (data[k] <= ::std::min(left, right))
			break;
		if (left < right) {
			::std::swap(data[k], data[2*k+1]);
			k = 2*k + 1;
		} else {
			::std::swap(data[k], data[2*k+2]);
			k = 2*k + 2;
		}
	}
}

template<typename T> 
inline void binary_heap<T>::pop()
{
	data[0] = data[numitem - 1];
	numitem--;
	sink(0);
}

template<typename T> 
inline void binary_heap<T>::rise(int k)
{
	while (k) {
		int j = (k - 1) / 2;
		if (data[j] > data[k])
			::std::swap(data[j], data[k]);
		else
			break;
		k = j;
	}
}

template<typename T> 
inline void binary_heap<T>::push(const T val)
{
	if (numitem == size) {
		size = size * 1.3 + 1;
		resize(size);
	}
	data[numitem] = val;
	rise(numitem);
	numitem++;
}

template<typename T> 
inline void binary_heap<T>::resize(const int n)
{
	int *tmp = new int[n];
	memcpy(tmp, data, sizeof(int) * ::std::min(n,numitem));
	free(data);
	data = tmp;
	size = n;
}

template<typename T> 
inline void binary_heap<T>::print_debug()
{
	for (int i = 0; i < numitem; i++)
		fprintf(stderr,"[%5d] %5d\n",i,data[i]);
}

#endif /* end of include guard */

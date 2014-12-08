#include <string.h>
#include <assert.h>
#include <algorithm>
#include "heap.h"

void heap::sink(int k)
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

void heap::pop()
{
	data[0] = data[numitem - 1];
	numitem--;
	sink(0);
}

void heap::rise(int k)
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

void heap::push(const int val)
{
	if (numitem == size) {
		size = size * 1.3 + 1;
		resize(size);
	}
	data[numitem] = val;
	rise(numitem);
	numitem++;
}

void heap::resize(const int n)
{
	int *tmp = new int[n];
	memcpy(tmp, data, sizeof(int) * ::std::min(n,numitem));
	free(data);
	data = tmp;
	size = n;
}

void heap::print()
{
	for (int i = 0; i < numitem; i++)
		printf("[%5d] %5d\n",i,data[i]);
}

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "fib_heap.h"

int main(int argc, char const* argv[])
{
	const int n = 30;
	int a[n];
	for (int i = 0; i < n; i++)
		a[i] = rand() % (13 * n);
	fib_heap<int> h;
	printf("Origianl input:\n");
	for (int i = 0; i < n; i++) {
		//printf("[%3d] %d\n",i,a[i]);
		h.push(::std::move(a[i]));
	}
	printf("\n");

	::std::sort(a,a+n);
	printf("Expected output:\n");
	for (int i = 0; i < n; i++)
		printf("%d\n",a[i]);
	printf("\n");

	printf("fib_heap output:\n");
	for (int i = 0; i < n; i++) {
		printf("%d\n",h.top());
		h.pop();
	}
	return 0;
}

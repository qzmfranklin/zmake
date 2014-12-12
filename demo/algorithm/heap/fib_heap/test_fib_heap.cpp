#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "fib_heap.h"

int main(int argc, char const* argv[])
{
	const int n = 10;
	int a[n];
	for (int i = 0; i < n; i++)
		a[i] = rand() % (10 * n);
	fib_heap<int> h;
	for (int i = 0; i < n; i++)
		h.push(a[i]);

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

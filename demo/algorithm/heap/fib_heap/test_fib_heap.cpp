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
		a[i] = rand() % (11 * n);
	fib_heap<int> h;
	::std::vector< fib_heap<int>::node* > v(n);
	printf("Origianl input:\n");
	for (int i = 0; i < n; i++) {
		printf("[%3d] %d\n",i,a[i]);
		h.push(a[i]);
		v[i] = h.last();
	}
	printf("\n");

	/*
	 *::std::sort(a,a+n);
	 *printf("Expected output:\n");
	 *for (int i = 0; i < n; i++)
	 *        printf("%d\n",a[i]);
	 *printf("\n");
	 */

	h.decrease_key(v[0], 10);
	h.decrease_key(v[1], 12);

	printf("fib_heap output:\n");
	for (int i = 0; i < n; i++) {
		printf("%d\n",h.top());
		h.pop();
	}
	return 0;
}

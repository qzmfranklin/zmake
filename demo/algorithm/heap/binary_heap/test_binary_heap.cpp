#include <stdlib.h>
#include <stdio.h>
#include "binary_heap.h"

int main(int argc, char const* argv[])
{
	const int n = 30;
	int a[n];
	for (int i = 0; i < n; i++)
		a[i] = rand() % (10 * n);
	binary_heap<int> h(10);
	for (int i = 0; i < n; i++)
		h.push(::std::move(a[i]));
	for (int i = 0; i < n; i++) {
		printf("%d\n",h.top());
		h.pop();
	}
	return 0;
}

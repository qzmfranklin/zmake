#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

int main(int argc, char const* argv[])
{
	const int n = 30;
	int a[n];
	for (int i = 0; i < n; i++)
		a[i] = rand() % (10 * n);
	heap h;
	for (int i = 0; i < n; i++)
		h.push(a[i]);
	//h.print();
	for (int i = 0; i < n; i++) {
		printf("%d\n",h.top());
		h.pop();
	}
	return 0;
}

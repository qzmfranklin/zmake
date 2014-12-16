#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <cmath>
#include <complex>
#include <vector>
#include <list>
#include "fib_heap.h"

int main(int argc, char const* argv[])
{
	const int n = 30;
	auto *v = new ::std::complex<double>[n];
	for (int i = 0; i < n; i++)
		v[i] = 10.0 * rand() / RAND_MAX
			+ 10.0i * rand() / RAND_MAX;

	fib_heap<double> queue;
	::std::vector< fib_heap<double>::node* > index;
	index.reserve(n);
	for (int i = 0; i < n; i++) {
		queue.push(DBL_MAX);
		index[i] = queue.last();
	}

	//::std::list

	delete []v;
	return 0;
}

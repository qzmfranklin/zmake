#include "binsearch.h"

int main(int argc, char const* argv[])
{
	const int N=20;
	int a[N];

	for (int i = 0; i < N; i++)
		a[i] = i;

	int index = binsearch(3,N,a);
	printf("index = %d\n",index);

	return 0;
}

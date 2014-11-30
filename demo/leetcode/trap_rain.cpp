#include <stdio.h>
#include <stdlib.h>
#include <vector>

static int trap_rain(const int *a, const int n)
{
	int i = 0;
	int j = n - 1;
	int lmax = 0;
	int rmax = 0;
	int sum = 0;
	while (i < j) {
		lmax = ::std::max(lmax,a[i]);
		rmax = ::std::max(rmax,a[j]);
		if (lmax < rmax)
			sum += lmax - a[i++];
		else
			sum += rmax - a[j--];
	}
	return sum;
}

int main(int argc, char const* argv[])
{
	const int a[] = {0,1,0,2,1,0,1,3,2,1,2,1};
	const int n = sizeof(a) / sizeof(int);
	printf("trap rain = %d\n",trap_rain(a,n));
	return 0;
}

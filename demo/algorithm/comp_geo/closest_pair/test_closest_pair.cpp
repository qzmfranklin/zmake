#include <stdlib.h>
#include <assert.h>
#include "../geo_utils.h"
#include "closest_pair.h"

int main(int argc, char const* argv[])
{
	const int N = 70*1000*1000;
	//const int N = 371;
	double *data = (double*)malloc(sizeof(double)*2*N);
	assert(data);
	for (int i = 0; i < 2*N; i++)
		data[i] = 100000.0*rand()/RAND_MAX;

	printf("Generated %d points\n",N);

	double _Complex pos[2];

	/*
	 *printf("n^2 method:\n");
	 *double d1 = closest_pair0(N,data,pos);
	 *printf("d_min = %.4f\n",d1);
	 *print_array(2,pos);
	 */

	printf("nlgn method:\n");
	double d2 = closest_pair(N,data,pos);
	printf("d_min = %.4f\n",d2);
	print_array(2,pos);

	free(data);
	return 0;
}

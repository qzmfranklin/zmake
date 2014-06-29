#include <complex.h>
#include "comp_geo/geo_utils.h"
#include "convex_hull.h"


int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;

	read_ascii(stdin,&N,&data);
	int pos[2];
	double dist_max = convhull_maxdist_pair(N,data,pos);

	printf("Found max distance:  ");
	printf("| [%3d] - [%3d] | = %7.2f\n",
			pos[0],pos[1],dist_max);

	free(data);
	return 0;
}

#include "comp_geo/geo_utils.h"
#include "convex_hull.h"
#include <complex.h>

int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;

	//read_binary(stdin,&N,&data);
	read_ascii(stdin,&N,&data);
	//print_array(N,data);

	graham_scan(&N,&data);
	
	//output_binary(stdout,N,data);
	output_ascii(stdout,N,data);

	free(data);

	return 0;
}

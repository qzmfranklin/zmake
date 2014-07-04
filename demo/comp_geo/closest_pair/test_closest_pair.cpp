#include "comp_geo/geo_utils.h"
#include "closest_pair.h"

int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;
	read_ascii(stdin,&N,&data);

	double _Complex pos[2];

	printf("n^2 method:\n");
	double d1 = closest_pair0(N,data,pos);
	printf("d_min = %.4f\n",d1);
	print_array(2,pos);

	printf("nlgn method:\n");
	double d2 = closest_pair(N,data,pos);
	printf("d_min = %.4f\n",d2);
	print_array(2,pos);

	free(data);
	return 0;
}

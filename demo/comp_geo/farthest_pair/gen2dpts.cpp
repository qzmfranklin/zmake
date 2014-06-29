#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <complex.h>
#include "comp_geo/geo_utils.h"

static struct {
	double xmin;
	double xmax;
	double ymin;
	double ymax;
} limits;

static size_t N;
static double _Complex *data;

static void init(int argc, char const *argv[])
{
	if (argc<5) {
		printf("Usage: gen2dpts.exe [xmin] [xmax] [ymin] [ymax] [N]\n");
		exit(1);
	}
	limits.xmin = atof(argv[1]);
	limits.xmax = atof(argv[2]);
	limits.ymin = atof(argv[3]);
	limits.ymax = atof(argv[4]);
	N = atoi(argv[5]);
}

static void binary(FILE *fd)
{
	fwrite(&N,sizeof(size_t),1,fd);
	for (int i = 0; i < N; i++) {
		const double buf[2] = {
			((double)rand())/RAND_MAX * (limits.xmax-limits.xmin) + limits.xmin,
			((double)rand())/RAND_MAX * (limits.ymax-limits.ymin) + limits.ymin};
		fwrite(buf,sizeof(double),2,fd);
	}
}

static void ascii(FILE *fd)
{
	fprintf(fd,"%lu\n",N);
	for (int i = 0; i < N; i++) {
		const double buf[2] = {
			((double)rand())/RAND_MAX * (limits.xmax-limits.xmin) + limits.xmin,
			((double)rand())/RAND_MAX * (limits.ymax-limits.ymin) + limits.ymin};
		fprintf(fd,"%23.16E %23.16E\n",buf[0],buf[1]);
	}
}

int main(int argc, char const* argv[])
{
	init(argc,argv);

	//binary(stdout);
	ascii(stdout);


	return 0;
}

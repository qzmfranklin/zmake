#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <complex>
#include <vector>
#include "algorithm/heap/fib_heap/fib_heap.h"
#include "../geo_utils.h"

using ::std::pair;
using ::std::make_pair;

struct node {
	int index;
	int connect;
	double distance;

	node(const double dist, const int ind):
		distance(dist), index(ind), connect(-INT_MAX) {}

	bool operator<(const node &rhs) const
	{ return distance < rhs.distance; }
};

int main(int argc, char const* argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Usage: mst.exe [infile]\n");
		exit(1);
	}
	FILE *fp = fopen(argv[1],"r");
	size_t n;
	::std::complex<double> *v;
	read_ascii(fp, &n, (void**)&v);
	fclose(fp);

	fib_heap<node> h;
	for (int i = 0; i < n; i++)
		h.push( node(DBL_MAX, i) );

	int prev = h.top().index;
	h.pop();
	for (int i = 1; i < n; i++) {
		for (auto &u: h) {
			node &p = u->key;
			const int curr = p.index;
			const double curr_dist = ::std::norm( v[curr] - v[prev] );
			if (curr_dist < p.distance) {
				p.connect = prev;
				p.distance = curr_dist;
				h.decrease_key(u, p);
			}
		}
		node &tmp = h.top();
		printf("%8d %8d\n", tmp.index, tmp.connect);
		prev = tmp.index;
		h.pop();
	}

	return 0;
}

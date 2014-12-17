#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <cmath>
#include <complex>
#include <vector>
#include "fib_heap.h"

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
	const int n = 30;
	auto *v = new ::std::complex<double>[n];
	for (int i = 0; i < n; i++)
		v[i] = ::std::complex<double>(
				10.0 * rand() / RAND_MAX,
				10.0 * rand() / RAND_MAX
				);

	fib_heap<node> h;
	for (int i = 0; i < n; i++)
		h.push( node(DBL_MAX, i) );

	::std::vector<int> node_list;
	node_list.reserve(n);
	::std::vector< pair<int,int> > edge_list;
	edge_list.reserve(n - 1);

	node_list[0] = h.top().index;
	h.pop();

	for (int i = 1; i < n; i++) {
		const int prev = node_list[i-1];
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
		node_list[i] = tmp.index;
		edge_list[i-1] = make_pair( tmp.index, tmp.connect );
		h.pop();
	}

	return 0;
}

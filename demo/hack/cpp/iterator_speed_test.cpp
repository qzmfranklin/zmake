#include "utils/Table.h"
#include "utils/TimeStat.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
	//printf("This program tests the speed of C++ iterators against the C raw pointers\n");

	const size_t m = 3;
	const size_t n = 4;
	const char *rows[m] = {"C ptr", "shared_ptr", "C++ itr"};
	const char *cols[n] = {"min", "mean", "median", "max"};

	// row-major storage
	double data[m*n];

	const size_t num = 100000;
	size_t counter = 0;

	{ // C ptr
		TimeStat g(num);
		int *a = (int*)malloc(sizeof(int) * num);
		for (int i = 0; i < num; i++)
			a[i] = i;
		int mock;
		for (int i = 0; i < num; i++) {
			g.tic();
			mock = a[i];
			g.toc();
		}
		free(a);
		data[counter++] = g.min();
		data[counter++] = g.mean();
		data[counter++] = g.median();
		data[counter++] = g.max();
	}

	{ // shared_ptr
		TimeStat g(num);
		std::vector<std::shared_ptr<int>> a;
		a.resize(num);
		for (int i = 0; i < num; i++)
			a[i] = std::make_shared<int>(i);
		int mock;
		for (int i = 0; i < num; i++) {
			const std::shared_ptr<int> ptr = a[i];
			g.tic();
			mock = *ptr;
			g.toc();
		}
		data[counter++] = g.min();
		data[counter++] = g.mean();
		data[counter++] = g.median();
		data[counter++] = g.max();
	}

	{ // C++ itr
		TimeStat g(num);
		std::vector<int> a;
		a.resize(num);
		for (int i = 0; i < num; i++)
			a[i] = i;
		int mock;
		for (int i = 0; i < num; i++) {
			g.tic();
			mock = a[i];
			g.toc();
		}
		data[counter++] = g.min();
		data[counter++] = g.mean();
		data[counter++] = g.median();
		data[counter++] = g.max();
	}

	Table table;
	table.dim(m, n);
	table.rows(rows);
	table.cols(cols);
	table.data(data);

	char banner[BUFSIZ];
	snprintf(banner, BUFSIZ, "");
	table.print(banner);

	return 0;
}

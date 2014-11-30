#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <set>
#include <unordered_map>
#include <string.h>

static void print_raw_board(const char *a, const int n)
{
	// top frame
	for (int j = 0; j < n; j++)
		printf("----");
	printf("-\n");
	// middle
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n; j++)
			printf("| %c ",a[j+i*n] ? a[j+i*n] : ' ');
		printf("|\n");
		for (int j = 0; j < n; j++)
			printf("|---");
		printf("|\n");
	}
	// last row
	for (int j = 0; j < n; j++)
		printf("| %c ",a[j+(n-1)*n] ? a[j+(n-1)*n] : ' ');
	printf("|\n");
	for (int j = 0; j < n; j++)
		printf("----");
	printf("-\n");
}

static void print_solution_board(const char *a, const int n, const char *sol)
#define output(k) { \
	if (a[k]) \
		printf("| %c ",a[k]); \
	else { \
		if (sol[k]) \
			printf("|*%c ",sol[k]); \
		else \
			printf("|   "); \
	} }
{
	// top frame
	for (int j = 0; j < n; j++)
		printf("----");
	printf("-\n");
	// middle
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n; j++)
			output(j+i*n);
		printf("|\n");
		for (int j = 0; j < n; j++)
			printf("|---");
		printf("|\n");
	}
	// last row
	for (int j = 0; j < n; j++)
		output(j+(n-1)*n);
	printf("|\n");
	for (int j = 0; j < n; j++)
		printf("----");
	printf("-\n");
}
#undef output

static char *sudoku_solver(const char *in, const int n)
{
	char *out = (char*) malloc(n*n);
	assert(out);
	memcpy(out,in,n*n);

	print_solution_board(in,n,out);
}

int main(int argc, char const* argv[])
{
	const char a[] = {
		'5','3', 0 , 0 ,'7', 0 , 0 , 0 , 0 ,
		'6', 0 , 0 ,'1','9','5', 0 , 0 , 0 ,
		 0 ,'9','8', 0 , 0 , 0 , 0 ,'6', 0 ,
		'8', 0 , 0 , 0 ,'6', 0 , 0 , 0 ,'3',
		'4', 0 , 0 ,'8', 0 ,'3', 0 , 0 ,'1',
		'7', 0 , 0 , 0 ,'2', 0 , 0 , 0 ,'6',
		 0 ,'6', 0 , 0 , 0 , 0 ,'2','8', 0 ,
		 0 , 0 , 0 ,'4','1','9', 0 , 0 ,'5',
		 0 , 0 , 0 , 0 ,'8', 0 , 0 ,'7','9'
	};
	const int n = sqrt(sizeof(a)) + 1E-6;

	print_raw_board(a,n);

	sudoku_solver(a,n);

	return 0;
}

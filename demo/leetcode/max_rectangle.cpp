#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stack>
#include <limits.h>

// @a: m x n, row major
static void print_matrix(const bool *a, const int m, const int n)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%c  ",a[j+i*n] ? 'X' : ' ');
		printf("\n");
	}
}

static int hist_max_area(const int *a, const int n)
{
	::std::stack<int> s;
	int retval = -INT_MAX;
	int i = 0;
	while (i < n)
		if (s.empty()  ||  a[i] > a[s.top()])
			s.push(i++);
		else {
			const int tmp = a[s.top()];
			s.pop();
			retval = ::std::max(retval,
					(s.empty() ? i : (i - s.top() - 1)) * tmp);
		}
	while (!s.empty()) {
		const int tmp = a[s.top()];
		s.pop();
		retval = ::std::max(retval,
				(s.empty() ? i : (i - s.top() - 1)) * tmp );
	}
	return retval;
}

static void print_b(const int *b, const int m, const int n)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%3d ",b[j+i*n]);
		printf("\n");
	}
}

static int max_rectangle(const bool *a, const int m, const int n)
{
	int *b = (int*) malloc(sizeof(int)*m*n);
	assert(b);
	for (int j = 0; j < n; j++)
		b[j] = a[j] ? 1 : 0;
	for (int i = 1; i < m; i++)
		for (int j = 0; j < n; j++)
			b[j+i*n] = a[j+i*n] ? b[j+i*n-n] + 1 : 0;

	//print_b(b,m,n);

	int retval = -INT_MAX;
	for (int i = 0; i < m; i++)
		retval = ::std::max(retval, hist_max_area(b+i*n,n));

	free(b);
	return retval;
}

int main(int argc, char const* argv[])
{
	srand(time(NULL));
	const int m = 12;
	const int n = 12;
	bool a[m*n];
	for (int i = 0; i < m*n; i++)
		a[i] = rand() & 1;

	print_matrix(a,m,n);

	printf("max rectangle = %d\n",max_rectangle(a,m,n));

	return 0;
}

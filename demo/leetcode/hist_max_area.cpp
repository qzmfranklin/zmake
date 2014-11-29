#include <stack>
#include <limits.h>

int hist_max_area(const int *a, const int n)
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

int main(int argc, char const* argv[])
{
	const int a[] = {1,2,3,6,2,5,4,5,1,6};
	const int n = sizeof(a)/sizeof(int);
	printf("hist_max_area = %d\n",hist_max_area(a,n));
	return 0;
}

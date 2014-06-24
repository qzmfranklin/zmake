#include "binsearch.h"

// return the index, return -1 upon failure
template<typename T> 
int binsearch(const T &key, const int n, const T *a);
{
	if (n<=0) return -1;
	int i=0, j=n-1, k;
	while (j-i>1) {
		k = (i+j)/2;
		if (a[k]>key)
			j=k;
		else if (a[k]<key)
			i=k;
		else
			return k;
	}
	if (a[i]==key)
		return i;
	if (j>i && a[j]==key)
		return j;
	return -1;
}

#include <stdio.h>
#include "heapsort.h"
/******************************************************************************/
static void swp(int *a, int *b);
/******************************************************************************/
static int swp_count=0;

static void swp(int *a, int *b)
{
	swp_count++;
	printf("\t%3d <-> %3d\n",*a,*b);
	int c = *a;
	*a = *b;
	*b = c;
	return;
}
static void sink2(const int size, int *a, const int n)
{
	//fprintf(stderr,"sink2\n");
	int *pt = a+n;
	int *c1 = a+2*n+1;
	int *c2 = a+2*n+2;
	if (2*n+1>=size)
		return;
	else if (2*n+2==size) {
		if (*c1>*pt) {
			swp(pt,c1);
			sink2(size,a,2*n+1); 
		}
	} else {
		if (*c1>*pt && *c1>*c2) {
			swp(pt,c1);
			sink2(size,a,2*n+1);
		} else if (*c2>*pt && *c2>*c1) {
			swp(pt,c2);
			sink2(size,a,2*n+2);
		}
	}
}
static void crtheap2(const int size, int *a)
{ 
	printf("crtheap\n");
	if (size<2) return; 

	int n=size/2;
	while (--n>-1) {
		printf("n = %d\n",n);
		sink2(size,a,n);
	}

	return;
}
static void sink3(const int size, int *a, const int n)
{
	//fprintf(stderr,"sink3\n");
	int *pt = a+n;
	int *c1 = a+3*n+1;
	int *c2 = a+3*n+2;
	int *c3 = a+3*n+3;
	if (3*n+1>=size)
		return;
	else if (3*n+2==size) {
		if (*c1>*pt) {
			swp(pt,c1);
			sink3(size,a,3*n+1); 
		}
	} else if (3*n+3==size) {
		if (*c1>*pt && *c1>*c2) {
			swp(pt,c1);
			sink3(size,a,3*n+1);
		} else if (*c2>*pt && *c2>*c1) {
			swp(pt,c2);
			sink3(size,a,3*n+2);
		}
	} else { 
		if (*c1>*pt && *c1>*c2 && *c1>*c3) {
			swp(pt,c1);
			sink3(size,a,3*n+1);
		} else if (*c2>*pt && *c2>*c1 && *c2>*c3) {
			swp(pt,c2);
			sink3(size,a,3*n+2);
		} else if (*c3>*pt && *c3>*c1 && *c3>*c2) {
			swp(pt,c3);
			sink3(size,a,3*n+3);
		}
	}
}
static void crtheap3(const int size, int *a)
{ 
	printf("crtheap\n");
	if (size<2) return; 

	int n=size/3;
	while (--n>-1) {
		printf("n = %d\n",n);
		sink3(size,a,n);
	}

	return;
}
static void sink4(const int size, int *a, const int n)
{
	//fprintf(stderr,"sink\n");
	int *pt = a+n;
	int *c1 = a+4*n+1;
	int *c2 = a+4*n+2;
	int *c3 = a+4*n+3;
	int *c4 = a+4*n+4;
	if (4*n+1>=size)
		return;
	else if (4*n+2==size) {
		if (*c1>*pt) {
			swp(pt,c1);
			sink4(size,a,4*n+1); 
		}
	} else if (4*n+3==size) {
		if (*c1>*pt && *c1>*c2) {
			swp(pt,c1);
			sink4(size,a,4*n+1);
		} else if (*c2>*pt && *c2>*c1) {
			swp(pt,c2);
			sink4(size,a,4*n+2);
		}
	} else if (4*n+4==size) { 
		if (*c1>*pt && *c1>*c2 && *c1>*c3) {
			swp(pt,c1);
			sink4(size,a,4*n+1);
		} else if (*c2>*pt && *c2>*c1 && *c2>*c3) {
			swp(pt,c2);
			sink4(size,a,4*n+2);
		} else if (*c3>*pt && *c3>*c1 && *c3>*c2) {
			swp(pt,c3);
			sink4(size,a,4*n+3);
		}
	} else { 
		if (*c1>*pt && *c1>*c2 && *c1>*c3 && *c1>*c4) {
			swp(pt,c1);
			sink4(size,a,4*n+1);
		} else if (*c2>*pt && *c2>*c1 && *c2>*c3 && *c2>*c4) {
			swp(pt,c2);
			sink4(size,a,4*n+2);
		} else if (*c3>*pt && *c3>*c1 && *c3>*c2 && *c3>*c4) {
			swp(pt,c3);
			sink4(size,a,4*n+3);
		} else if (*c4>*pt && *c4>*c1 && *c4>*c2 && *c4>*c3) {
			swp(pt,c4);
			sink4(size,a,4*n+4);
		}
	}
}
static void crtheap4(const int size, int *a)
{ 
	printf("crtheap\n");
	if (size<2) return; 

	int n=size/4;
	while (--n>-1) {
		printf("n = %d\n",n);
		sink4(size,a,n);
	}

	return;
}
/******************************************************************************/
void heapsort(int size, int *a)
{
	fprintf(stderr,"heapsort\n");

	if (!size) return;

	swp_count=0;

	crtheap2(size,a); 

	do {
		printf("pop\n");
		swp(a,a+size-1);
		sink2(size-1,a,0);
	}
	while (--size>0);

	printf("swp_count=%d\n",swp_count);

	return;
}

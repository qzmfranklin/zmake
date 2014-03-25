#include <stdio.h>
#include <math.h>
#include "utils/utils.h"
/******************************************************************************/
void print_diffmat(const int n, const double *a, const double shift);
void get_diff(const int n, const double *a, const double diff, double *val, int *row, int *col);
/******************************************************************************/
int main(int argc, char *argv[])
{
	const int N=8;
	const double a[]={0.0,27.2,30.1,97.9,102.4,190.9,200.5,210.3};

	print_d(N,a);
	printf("\n");
	print_diffmat(N,a,0.0);

	double diff;
	printf("Enter the float number you wish to search:\n");
	scanf("%lf",&diff);

	print_diffmat(N,a,diff);
	/*printf("diff_par = %8.1f\n",diff_par);*/

	int row,col;
	double val;
	get_diff(N,a,diff,&val,&row,&col);
	printf("  [%4d,%4d](%8.1lf)\n",row,col,val);

	return 0;
} 

void print_diffmat(const int n, const double *a, const double shift)
{
	printf("Diff matrix:\n");
	for (int i = 0; i < n-1; i++) {
		for (int j = 0; j < i; j++)
			printf("         ");
		for (int j = i+1; j < n; j++)
			printf("%-8.1f ",a[j]-a[i]-shift);
		printf("\n");
	}
} 

/**************************************/
void get_diff(const int n, const double *a, const double diff, double *val, int *row, int *col)
{
	int num_step=1;

	int i=0,j=1;
	int _i=0,_j=1;
	double d = a[1] - a[0] - diff;
	double _val=d;
	printf("  [%4d,%4d](%8.1f)\n",i,j,d);
	while(1) {
		if (d<0.0) {
			if (j==n-1) break;
			j++;
		} else if (d>0.0 && j-i==1) {
			if (j==n-1) break;
			i++;
			j++;
		} else {
			if (i==n-2) break;
			i++;
		}
		d = a[j] - a[i] - diff;
		printf("=>[%4d,%4d](%8.1f)\n",i,j,d);
		if (fabs(d)<fabs(_val)) {
			_val = d;
			_i = i;
			_j = j;
		}
		num_step++;
	}
	*val = _val;
	*row = _i;
	*col = _j;
	printf("num_step = %d\n",num_step);
}

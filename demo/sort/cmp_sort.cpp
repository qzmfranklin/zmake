#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "sort/heapsort/heapsort.h"
//#include "sort/mergesort/mergesort.h"
#include "sort/quicksort/quicksort.h"
#include "utils/Table.h"
#include "utils/TimeStat.h"

enum SORT_TYPE {
	HEAP2=0,
	HEAP3,
	HEAP4,
	//MERGE,
	QUICK,
	QUICKNR,
	QSORT
};

class cmpsort {
	private:
		int status;
		int num;
		int *data;
		int *bk;
	public:
		cmpsort() { status=0; }
		~cmpsort() { if (status==1) reset(); }

		void set(const int n) {
			assert(status==0);
			srand(time(0));
			num=n;
			data=(int*)malloc(sizeof(int)*n);
			assert(data);
			bk=(int*)malloc(sizeof(int)*n);
			assert(bk);
			for (int i = 0; i < n; i++)
				bk[i] = rand()%INT_MAX;
			status=1;
		}
		void reset() {
			assert(status==1);
			free(data);
			free(bk);
			status=0;
		}

		void print_bk() {
			printf("bk=\n");
			for (int i = 0; i < num; i++)
				printf("[%10d] %10d\n",i,bk[i]);
		}
		void print_data() {
			printf("data=\n");
			for (int i = 0; i < num; i++)
				printf("[%10d] %10d\n",i,data[i]);
		}

		double nlogn() { return num*log(1.0*num)/log(2.0); }

		double test(const int type, const int n) {
			assert(status==1); 
			TimeStat clk(n);
			for (int j = 0; j < n; j++) {
				for (int i = 0; i < num; i++)
					data[i]=bk[i]; 
				switch ((enum SORT_TYPE)type) {
				case HEAP2:
					clk.tic();
					heapsort2(num,data);
					clk.toc();
					break;
				case HEAP3:
					clk.tic();
					heapsort3(num,data);
					clk.toc();
					break;
				case HEAP4:
					clk.tic();
					heapsort4(num,data);
					clk.toc();
					break;
				//case MERGE:
					//clk.tic();
					//mergesort(num,data);
					//clk.toc();
					//break;
				case QUICK:
					clk.tic();
					quicksort(num,data);
					clk.toc();
					break;
				case QUICKNR:
					clk.tic();
					quicksort_nr(num,data);
					clk.toc();
					break;
				case QSORT:
					clk.tic();
					qsort(data,num,sizeof(int),compare);
					clk.toc();
					break;
				default:
					break;
				}
			}
			return clk.median();
		}
	private:
		static int compare (const void * a, const void * b) 
		{ return ( *(int*)a - *(int*)b ); }
};

int main(int argc, char const* argv[])
{
	if (argc<1) {
		printf("Usage: cmp_sort.exe [num_trials]\n");
		exit(1);
	}
	int N;
	sscanf(argv[1],"%d",&N);

	const int m = 6;
	const int n = 2;
	const char* rows[m] = {"heap2","heap3","heap4","quick","quick_nr","qsort"};
	const char* cols[n] = {"median","nrl'd median"};
	double data[m*n];

	cmpsort t;
	t.set(N);
	for(int i=0; i < m; i++) {
		//printf("%d\n",i);
		data[i*n+0] = t.test(i,30);
		data[i*n+1] = data[i*n+0]/t.nlogn();
		//t.print_data();
	}

	Table table;
	table.dim(m, n);
	table.rows(rows);
	table.cols(cols);
	table.data(data);
	char banner[200];
	sprintf(banner,"array_size = %d\n",N);
	strcat(banner,"\n\tCompare heapsort and quicksort,");
	strcat(banner,"\n\tTime is in CPU cycles");
	strcat(banner,"\n\tnrl'd = normalized, i.e., divided by n*lg_2(n)");
	table.print(banner);

	//printf("WARNING: Tune the num and the unroll pars in time_vecsin in lecture.\n");
	return 0;
}

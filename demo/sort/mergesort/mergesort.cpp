#include "mergesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static void print_array(const int n, const int *a)
{
	printf("array =");
	for (int i = 0; i < n; i++)
		printf("%d,",a[i]);
	printf("\n");
}

// merge *a (na) and *b (nb) into *c(=*a) (nc=na+nb)
static void merge0(const int na, int *restrict a, 
		const int nb, int *restrict b,
		double *work) 
{
	//fprintf(stderr,"merge0\n");
	int *wk = (int*)work; // cast workspace to int
	int i=0,j=0,k=0;
	while ((i<na)&&(j<nb))
		wk[k++] = (a[i]<b[j]) ? a[i++] : b[j++];
	if (j==nb)
		for (int idx = i; idx < na; idx++)
			a[nb+idx] = a[idx];
	for (int idx = 0; idx < k; idx++)
		a[idx] = wk[idx];
}

static inline void merge(int *restrict a, const int na, 
		const int nb, double *work)
{
	int *b = a+na;
	merge0(na,a,nb,b,work);
}

static struct st_glob *init_glob(const int size, int *a)
{
	//fprintf(stderr,"init_glob\n");
	struct st_glob *glist = (struct st_glob*) 
		malloc(sizeof(struct st_glob));
	assert(glist);

	int offset=0, length=1;
	struct st_glob *g = glist;
	//g->prev   = NULL;
	for (int i = 1; i < size ; i++)
		if (a[i]>=a[i-1])
			length++;
		else {
			g->offset = offset;
			g->length = length;
			g->next   = (struct st_glob*) 
				malloc(sizeof(struct st_glob));
			assert(g->next);
			//g->next->prev = g;
			g         = g->next;
			offset   += length;
			length    = 1;
		}
	g->offset = offset;
	g->length = length;
	g->next   = NULL;

	return glist;
}

static void print_glob(struct st_glob *glist) 
{
	//fprintf(stderr,"print_glob\n");
	printf("glist = \n");
	while (glist) {
		struct st_glob *next = glist->next;
		printf("%p [%3d,%3d] %p\n",
				glist,
				glist->offset,
				glist->length,
				glist->next);
		glist = next;
	}
}

static void free_glob(struct st_glob *glist)
{
	//fprintf(stderr,"free_glob\n");
	while (glist) {
		struct st_glob *next = glist->next;
		free(glist);
		glist = next;
	}
}

// merge g with g->next
static void merge_glob(struct st_glob *g)
{
	struct st_glob *g_next = g->next;
	assert(g_next);
	struct st_glob *g_next2= g_next->next;

	g->length = g->length + g_next->length;
	g->next   = g_next2;
	//if (g_next2) 
		//g_next2->prev = g;

	free(g_next);
}

void mergesort(const int size, int *a)
{
	//fprintf(stderr,"mergesort\n");
	struct st_glob *glist = init_glob(size,a);
	//print_glob(glist);
	
	double *work = (double*)malloc(sizeof(int)*size);
	assert(work);
	while (glist->next) {
		struct st_glob *g = glist;
		while (g) {
			if (g->next == NULL) break;
			//fprintf(stderr,"\tmerge(%3d:%3d+%-3d)\n",
				//g->offset,g->length,g->next->length);
			merge(  a + g->offset, 
				g->length, 
				g->next->length,
				work );
			merge_glob(g);
			g = g->next;
		}
	}

	free(glist);
	free(work);
}

/*
 *int main(int argc, char const* argv[])
 *{
 *        const int n=15;
 *        int a[n]={11,7,3,9,14,0,12,5,15,10,2,8,1,4,6};
 *
 *        print_array(n,a);
 *        mergesort(n,a); 
 *        print_array(n,a);
 *
 *        return 0;
 *}
 */

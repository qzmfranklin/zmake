#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// compute the prefix function of string *a
static int *prefix_pi(const int n, const char *restrict p)
{
	//fprintf(stderr,"prefix_pi\n");
	int *pi = (int*)malloc(sizeof(int)*n);
	assert(pi);
	pi[0] = -1;
	int j = -1;
	for (int i = 1; i < n; i++) {
		while (j>-1 && p[j+1]!=p[i])
			j=pi[j];
		if (p[j+1]==p[i])
			j++;
		pi[i] = j;
	}

	//for (int i = 0; i < n; i++)
		//fprintf(stderr,"pi[%3d] = %d\n",i,pi[i]);
	return pi;
}

/*
 * *t=text   *p=pattern    
 * *t, *p, and *pos are all allocated externally
 * Upon returning, *pos holds the starting offsets of matches.
 * The return value is the number of matches.
 */
static int strmatch_kmp(const int tlen, const char *restrict t,
		const int plen, const char *restrict p,
		int *restrict pos)
{
	//fprintf(stderr,"strmatch_kmp\n");
	int *pi = prefix_pi(plen,p);

	int j = -1, k = -1;
	for (int i = 0; i < tlen; i++) {
		while (j>-1 && p[j+1]!=t[i])
			j=pi[j];
		if (p[j+1]==t[i])
			j++;
		if (j==plen-1)
			pos[++k] = i-j;
		//fprintf(stderr,"%3d %3d %3d\n",i,j,k);
	}

	free(pi);
	return k+1;
}

int main(int argc, char const* argv[])
{
	const char text[] = "abc dabedababcdabecdef";
	const char patt[] = "dabe";
	int pos[3];
	int num = strmatch_kmp(sizeof(text)-1,text,
			sizeof(patt)-1,patt,pos);
	for (int i = 0; i < num; i++)
		printf("text[%3d]\n",pos[i]);
	return 0;
}

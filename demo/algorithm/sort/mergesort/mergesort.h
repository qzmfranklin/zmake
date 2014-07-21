#ifndef _MERGESORT_H_
#define _MERGESORT_H_
#ifdef __cplusplus
extern "C" {
#endif

// increasing order, natural merge sort
void mergesort(const int size, int *a);

//  16 bytes on x86_64
struct st_glob {
	int offset;
	int length;
	//struct st_glob *prev;
	struct st_glob *next;
};

	
#ifdef __cplusplus
}
#endif
#endif /* end of include guard */

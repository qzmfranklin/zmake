#ifndef _CPUCYC_H_
#define _CPUCYC_H_

#ifdef __cplusplus
extern "C" {
#endif

// only for x86 architecture
typedef struct cpucyc {
	uint32_t a; // low
	uint32_t d; // high
} cpucyc_t;

static inline void tic(cpucyc_t *t)
{
	asm volatile("rdtsc":"=a"(t->a),"=d"(t->d));
}

static inline double convert(cpucyc_t *t1, cpucyc_t *t2)
{
	double val;
	val  = t2->d - t1->d;
	val *= pow(2.0,32.0);
	val += t2->a - t1->a;
	return val;
}

#ifdef __cplusplus
}
#endif

#endif

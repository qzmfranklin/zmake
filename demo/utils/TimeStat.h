/*
 * Copyright Zhongming Qu, 2014
 */

/*
 * Copyright Divakar Viswanath, 2009-2014
 */

/*     
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as 
 * published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _TIMESTAT_H_
#define _TIMESTAT_H_

#include <cmath>
#include "StatVector.h"

class TimeStat: public StatVector {
	private:
		unsigned int a1, d1;
		unsigned int a2, d2;
	public:
		TimeStat(const int n): StatVector(n) {}
		inline void tic(){//does not use cpuid
			asm volatile("rdtsc" : "=a" (a1), "=d" (d1));
		}
		inline double toc(){//does not use cpuid
			asm volatile("rdtsc" : "=a" (a2), "=d" (d2));
			double tmp=convert(a1, d1, a2, d2);
			insert(tmp);
			return tmp;
		}
	private:
		inline double convert(unsigned int A1, unsigned int D1, 
				unsigned int A2, unsigned int D2){
			double val;
			val = D2-D1;
			val *= pow(2.0, 32.0);
			val = (val + A2) - A1;
			return val;
		}
};

#endif

#include "second.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
	#include <cblas.h>
}

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(*arr))

double dgemm_blas(unsigned int n, unsigned int iterations);

int main() {
	unsigned int i, j, n, iterations;
	double time, gflop, gflops;

	int N[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
	           1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};

	// method names
	const char *names[] = {"cblas"};
	// method functions
	double (*functions[])(unsigned int, unsigned int) = {dgemm_blas};

	for(i = 0; i < ARR_SIZE(N); i++)
	{
		n = N[i];
		gflop = 2.0*n*n*n/1000000000.0;
		iterations = ((unsigned int) 10 / gflop) + 1;
		printf("N = %4i; %4i iters;", n, iterations);

		for(j = 0; j < ARR_SIZE(names); j++)
		{
			time = functions[j](n, iterations);
			gflops = iterations*gflop/time;
			printf(" %s %4.1lf GFLOP/s", names[j], gflops);
		}
		printf("\n");
	}
	return 0;
}

double dgemm_blas(unsigned int n, unsigned int iterations)
{
	double *a, *b, *c;
	double t1;
	unsigned int i, j;

	a = (double *) malloc(n*n*sizeof(double));
	b = (double *) malloc(n*n*sizeof(double));
	c = (double *) malloc(n*n*sizeof(double));

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			a[i + n*j] = i + j;
			b[i + n*j] = i - j;
		}
	}

	t1 = second();
	for(i = 0; i < iterations; i++ )
	{
		cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
		            n, n, n, 1.0, a, n, b, n, 0.0, c, n);
	}
	return second() - t1;
}

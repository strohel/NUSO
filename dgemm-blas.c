#include "second.h"

#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>

int main() {
	double *a, *b, *c;
	unsigned int i, j, k, n;
	double t1, t2, mflops;

	int N[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
	           1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};
	for(k = 0; k < sizeof(N)/sizeof(*N); k++)
	{
		n = N[k];

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
		cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
		            n, n, n, 1.0, a, n, b, n, 0.0, c, n);
		t2 = second();

		mflops = 2*n*n*n/(t2-t1)/1000000.0;

		printf("N = %4i mflops = %12lf\n", n, mflops);
	}
	return 0;
}

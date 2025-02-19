#include "second.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
	#include <cblas.h>
}

#include <Eigen/Dense>

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(*arr))

using namespace Eigen;

double dgemm_cblas(int n, int iterations);
double dgemm_eigen_naive(int n, int iterations);
double dgemm_eigen(int n, int iterations);
double dgemm_eigen_templ(int n, int iterations);
template <int compile_n> double dgemm_eigen_templ_impl(int n, int iterations);

int main() {
	unsigned int i, j, n, iterations;
	double time, gflop, gflops;

	int N[] = {4, 6, 8, 12, 16, 24, 32, 48, 64,
	           100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
	           1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};

	// method names
	const char *names[] = {"cblas", "Eigen-naive", "Eigen", "Eigen+templ"};
	// method functions
	double (*functions[])(int, int) = {dgemm_cblas, dgemm_eigen_naive, dgemm_eigen, dgemm_eigen_templ};

	for(i = 0; i < ARR_SIZE(N); i++)
	{
		n = N[i];
		gflop = 2.0*n*n*n/1000000000.0;
		iterations = ((unsigned int) 5 / gflop) + 1;
		iterations = iterations > 9999999 ? 9999999 : iterations;
		printf("N = %4i; %7i iters;", n, iterations);

		for(j = 0; j < ARR_SIZE(names); j++)
		{
			time = functions[j](n, iterations);
			gflops = iterations*gflop/time;
			printf(" %s: %5.3lf s %5.2lf GFLOP/s;", names[j], time, gflops);
		}
		printf("\n");
	}
	return 0;
}

double dgemm_cblas(int n, int iterations)
{
	double *a, *b, *c;
	double t1;
	int i, j;

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

double dgemm_eigen_naive(int n, int iterations)
{
	MatrixXd a(n, n), b(n, n), c(n, n);
	int i, j;
	double t1;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			a(i, j) = i + j;
			b(i, j) = i - j;
		}
	}

	t1 = second();
	for(i = 0; i < iterations; i++ )
	{
		c = a * b;
	}
	return second() - t1;
}

double dgemm_eigen(int n, int iterations)
{
	MatrixXd a(n, n), b(n, n), c(n, n);
	int i, j;
	double t1;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			a(i, j) = i + j;
			b(i, j) = i - j;
		}
	}

	t1 = second();
	for(i = 0; i < iterations; i++ )
	{
		c.noalias() = a * b;
	}
	return second() - t1;
}

double dgemm_eigen_templ(int n, int iterations)
{
	switch(n)
	{
		case 1: return dgemm_eigen_templ_impl<1>(n, iterations);
		case 2: return dgemm_eigen_templ_impl<2>(n, iterations);
		case 3: return dgemm_eigen_templ_impl<3>(n, iterations);
		case 4: return dgemm_eigen_templ_impl<4>(n, iterations);
		case 6: return dgemm_eigen_templ_impl<6>(n, iterations);
		case 8: return dgemm_eigen_templ_impl<8>(n, iterations);
		case 12: return dgemm_eigen_templ_impl<12>(n, iterations);
		case 16: return dgemm_eigen_templ_impl<16>(n, iterations);
		case 24: return dgemm_eigen_templ_impl<24>(n, iterations);
		case 32: return dgemm_eigen_templ_impl<32>(n, iterations);
		case 48: return dgemm_eigen_templ_impl<48>(n, iterations);
		case 64: return dgemm_eigen_templ_impl<64>(n, iterations);
		default: return dgemm_eigen_templ_impl<Dynamic>(n, iterations);
	}
}

template <int compile_n>
double dgemm_eigen_templ_impl(int n, int iterations)
{
	Matrix<double, compile_n, compile_n> a(n, n), b(n, n), c(n, n);
	int i, j;
	double t1;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			a(i, j) = i + j;
			b(i, j) = i - j;
		}
	}

	t1 = second();
	for(i = 0; i < iterations; i++ )
	{
		c.noalias() = a * b;
	}
	return second() - t1;
}

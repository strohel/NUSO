#include "second.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern "C" {
	#include <cblas.h>

	void dgesv_(int*, int*, double*, int*, int*, double*, int*, int*);
	int dgesvx_(char *fact, char *trans, int *n, int *nrhs,
	            double *a, int *lda, double *af, int *ldaf,
	            int *ipiv, char *equed, double *r__, double *c__,
	            double *b, int *ldb, double *x, int *ldx,
	            double *rcond, double *ferr, double *berr,
	            double *work, int *iwork, int *info);
}

#include <eigen3/Eigen/Dense>

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(*arr))

using namespace std;

struct MyMatrix
{
	MyMatrix(int N_, int M_)
		: N(N_)
		, M(M_)
		, data(new double[N * M])
	{}

	MyMatrix(const MyMatrix &orig)
		: N(orig.N)
		, M(orig.M)
		, data(new double[N * M])
	{
		memcpy(data, orig.data, N * M * sizeof(double));
	}

	~MyMatrix()
	{
		delete[] data;
	}

	MyMatrix &operator=(const MyMatrix &rhs)
	{
		delete[] data;
		N = rhs.N;
		M = rhs.M;
		data = new double[N * M];
		memcpy(data, rhs.data, N * M * sizeof(double));
		return *this;
	}

	double *operator[](int i)
	{
		return &data[i*N];
	}

	int N, M;
	double *data;
};

MyMatrix loadMatrix(const char *filename, int N)
{
	MyMatrix A(N, N);
	int real_width = 1000;
	assert(N <= real_width);

	ifstream f(filename);
	if(!f.good())
		throw "Cannot read filename";
	string line;

	// preskocim prvnich 5 radku
	for (int i=0; i<5; i++)
		getline(f,line);

	A.N = N;
	A.M = N;
	A.data = new double[A.N*A.M];

	double dummy;
	for (int j=0; j < A.M; j++)
		for (int i=0; i < real_width; i++)
			if(i < A.N)
				f >> A[i][j];
			else
				f >> dummy;
	return A;
}

double lapack(MyMatrix A, double *b, double *x);
double lapack_expert(MyMatrix A, double *b, double *x);

int main() {
	MyMatrix A(1000, 1000);
	unsigned int j, iterations;
	double time, gflop, gflops;

	double *x, *b;

	// Nacteni matice
	A = loadMatrix("p1000", 1000);
	x = new double[A.M];
	b = new double[A.N];

	// Vypocet prave strany
	for (int i=0; i<A.N; i++) {
		b[i] = 0;
		for (int j=0; j<A.M; j++)
			b[i] += A[i][j];
	}
	cerr << "b[0]..b[2]: " << b[0] << " " << b[1] << " " << b[2] << endl;

	// method names
	const char *names[] = {"lapack", "lapack-expert"};
	// method functions
	double (*functions[])(MyMatrix, double *, double *) = {lapack, lapack_expert};

// 	gflop = 2.0*n*n*n/1000000000.0;
// 	iterations = ((unsigned int) 5 / gflop) + 1;
// 	iterations = iterations > 9999999 ? 9999999 : iterations;
	gflop = 1; // TODO
	int n = 1000;
	iterations = 1; // TODO
	printf("N = %4i; %7i iters;", n, iterations);

	for(j = 0; j < ARR_SIZE(names); j++)
	{
		time = functions[j](A, b, x);
		gflops = iterations*gflop/time;
		printf(" %s: %5.3lf s %5.2lf GFLOP/s;", names[j], time, gflops);
		cerr << "x[0]..x[2]: " << x[0] << " " << x[1] << " " << x[2] << endl;
	}
	printf("\n");
	return 0;
}

double
lapack(MyMatrix A, double *b, double *x)
{
	double t1;

	int one = 1, info, *ipvt = new int[A.N];

	memcpy(x, b, A.N * sizeof(double));
	t1 = second();
	dgesv_(&A.N, &one, A.data, &A.N, ipvt, x, &A.N, &info);
	return second() - t1;
}

double
lapack_expert(MyMatrix A, double *b, double *x)
{
	double t1, t2;

	char *fact  = (char *) "N";
	char *trans =  (char *) "T";
	int  one = 1;
	int  ipvt[A.N], iwork[A.N], info;
	char equed;
	double r[A.N], c[A.N], rcond, ferr, berr, work[4*A.N], AF[A.N*A.N];

	t1 = second();
	dgesvx_(fact, trans, &A.N, &one, A.data, &A.N, AF, &A.N, 
	        ipvt, &equed, r, c, b, &A.N, x, &A.N, &rcond, &ferr, &berr,
	        work, iwork, &info);
	t2 = second();

	cerr << "Info  = " << info << endl;
	cerr << "rcond = " << rcond << endl;
	cerr << "ferr  = " << ferr << endl;
	return t2 - t1;
}

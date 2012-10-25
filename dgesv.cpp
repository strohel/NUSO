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

using namespace Eigen;
using namespace std;

struct MyMatrix
{
	explicit MyMatrix(int N_ = 0, int M_ = 0)
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

double lapack(MyMatrix A, double *b, double *x, int iters);
double lapack_expert(MyMatrix A, double *b, double *x, int iters);
double EigPartPivLU(MyMatrix A, double *b, double *x, int iters);
double EigFullPivLU(MyMatrix A, double *b, double *x, int iters);
double EigQR(MyMatrix A, double *b, double *x, int iters);
double EigColPivQR(MyMatrix A, double *b, double *x, int iters);
double EigFullPivQR(MyMatrix A, double *b, double *x, int iters);

int main() {
	int N[] = {100, 150, 200, 250, 300, 350, 400, 450, 500, 550,
	           600, 650, 700, 750, 800, 850, 900, 950, 1000};

	// method names
	const char *names[] = {"LAPACK", "LAPACK-Expert", "EigPartPivLU", "EigFullPivLU",
			"EigQR", "EigColPivQR", "EigFullPivQR"};
	// method functions
	double (*functions[])(MyMatrix, double *, double *, int) = {lapack, lapack_expert,
			EigPartPivLU, EigFullPivLU, EigQR, EigColPivQR, EigFullPivQR};

	for(unsigned int i = 0; i < ARR_SIZE(N); i++)
	{
		int n = N[i];

		// Nacteni matice
		MyMatrix A = loadMatrix("p1000", n);
		double *x = new double[A.M];
		double *b = new double[A.N];

		// Vypocet prave strany
		for (int i = 0; i < A.N; i++) {
			b[i] = 0;
			for (int j = 0; j < A.M; j++)
				b[i] += A[i][j];
		}
		// cerr << "b[0]..b[2]: " << b[0] << " " << b[1] << " " << b[2] << endl;

		double gflop = 1.0*n*n*n/1000000000.0; // TODO
		int iterations = 1 / gflop + 1;
		printf("N = %4i; %4i iters;", n, iterations);

		for(unsigned int j = 0; j < ARR_SIZE(names); j++)
		{
			double time = functions[j](A, b, x, iterations);
			double gflops = iterations*gflop/time;
			printf(" %s: %5.3lf s %5.2lf GFLOP/s;", names[j], time, gflops);
			// cerr << "x[0]..x[2]: " << x[0] << " " << x[1] << " " << x[2] << endl;
		}
		printf("\n");
	}
	return 0;
}

double
lapack(MyMatrix A, double *b, double *x, int iters)
{
	double t1;

	int one = 1, info, *ipvt = new int[A.N];

	memcpy(x, b, A.N * sizeof(double));
	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		dgesv_(&A.N, &one, A.data, &A.N, ipvt, x, &A.N, &info);
	}
	return second() - t1;
}

double
lapack_expert(MyMatrix A, double *b, double *x, int iters)
{
	double t1, t2;

	char *fact  = (char *) "N";
	char *trans =  (char *) "T";
	int  one = 1;
	int  ipvt[A.N], iwork[A.N], info;
	char equed;
	double r[A.N], c[A.N], rcond, ferr, berr, work[4*A.N], AF[A.N*A.N];

	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		dgesvx_(fact, trans, &A.N, &one, A.data, &A.N, AF, &A.N, 
		        ipvt, &equed, r, c, b, &A.N, x, &A.N, &rcond, &ferr, &berr,
		        work, iwork, &info);
	}
	t2 = second();

// 	cerr << "Info  = " << info << endl;
// 	cerr << "rcond = " << rcond << endl;
// 	cerr << "ferr  = " << ferr << endl;
	return t2 - t1;
}

double
EigPartPivLU(MyMatrix A, double *b, double *x, int iters)
{
	double t1;

	Map<MatrixXd> A_(A.data, A.N, A.M);
	Map<VectorXd> b_(b, A.N);
	Map<VectorXd> x_(x, A.N);

	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		x_ = A_.partialPivLu().solve(b_);
	}
	return second() - t1;
}

double
EigFullPivLU(MyMatrix A, double *b, double *x, int iters)
{
	double t1;

	Map<MatrixXd> A_(A.data, A.N, A.M);
	Map<VectorXd> b_(b, A.N);
	Map<VectorXd> x_(x, A.N);

	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		x_ = A_.fullPivLu().solve(b_);
	}
	return second() - t1;
}

double
EigQR(MyMatrix A, double *b, double *x, int iters)
{
	double t1;

	Map<MatrixXd> A_(A.data, A.N, A.M);
	Map<VectorXd> b_(b, A.N);
	Map<VectorXd> x_(x, A.N);

	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		x_ = A_.householderQr().solve(b_);
	}
	return second() - t1;
}

double
EigColPivQR(MyMatrix A, double *b, double *x, int iters)
{
	double t1;

	Map<MatrixXd> A_(A.data, A.N, A.M);
	Map<VectorXd> b_(b, A.N);
	Map<VectorXd> x_(x, A.N);

	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		x_ = A_.colPivHouseholderQr().solve(b_);
	}
	return second() - t1;
}

double
EigFullPivQR(MyMatrix A, double *b, double *x, int iters)
{
	double t1;

	Map<MatrixXd> A_(A.data, A.N, A.M);
	Map<VectorXd> b_(b, A.N);
	Map<VectorXd> x_(x, A.N);

	t1 = second();
	for(int i = 0; i < iters; i++)
	{
		x_ = A_.fullPivHouseholderQr().solve(b_);
	}
	return second() - t1;
}

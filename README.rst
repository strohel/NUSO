NUSO Sandbox
============

Requirements
------------

 * CBLAS implementation (reference, ATLAS, MKL, ...) with cblas.h under /usr/include
   and linkable using -lcblas
 * Fortran LAPACK implementation linkable as -llapack
 * Eigen 3 with headers insalled under /usr/include/eigen3

Build
-----

 * ``make``
 * Customize compiler, flags:
   ``make clean && make CXX=g++-4.6.3 CXXFLAGS="-O3 -march=core-avx-i" LDFLAGS="-Wl,--as-needed"``

Run
---

BLAS Matrix * Matrix multiply
*****************************

 * ``./dgemm``
 * Save stats to `out.txt`: ``./dgemm > out.txt``
 * Vistualize stats (needs Python & matplotlib): ``./visualize_dgemm.py out.txt``
 * Short-hand: ``./dgemm | ./visualize_dgemm.py``

LAPACK Linear System Solving
****************************

 * ``./dgesv``
 * Save stats to `out.txt`: ``./dgesv > out.txt``
 * Vistualize stats (needs Python & matplotlib & NumPy): ``./visualize_dgesv.py out.txt``
 * Short-hand: ``./dgesv | ./visualize_dgesv.py``

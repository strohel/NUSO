NUSO Sandbox
============

Requirements
------------

 * CBLAS implementation (reference, ATLAS, MKL, ...) with cblas.h under /usr/include
   and linkable using -lcblas
 * Eigen 3 with headers insalled under /usr/include/eigen3

Build
-----

 * ``make``
 * Customize compiler, flags:
   ``make clean && make CXX=g++-4.6.3 CXXFLAGS="-O3 -march=core-avx-i" LDFLAGS="-Wl,--as-needed"``

Run
---

 * ``./comparison``
 * Save stats to `out.txt`: ``./comparison > out.txt``
 * Vistualize stats (needs Python & matplotlib): ``./visualize.py out.txt``
 * Short-hand: ``./comparison | ./visualize.py``

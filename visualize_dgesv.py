#!/usr/bin/env python
# -*- Coding:utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np

import fileinput


def main():
    Ns = []
    gflops = {}
    errors = {}
    for line in fileinput.input():
        (N, iters, implementations) = line.split(';', 2)
        N = int(N.split()[2])
        Ns.append(N)
        implementations = implementations.strip('\n\r \t;')
        for implementation in implementations.split(';'):
            implementation = implementation.split()
            name = implementation[0].strip(' :')
            eucl_error = float(implementation[1])
            curr_gflops = float(implementation[2])
            if name in gflops:
                gflops[name].append(curr_gflops)
                errors[name].append(eucl_error)
            else:
                gflops[name] = [curr_gflops]
                errors[name] = [eucl_error]

    plt.subplot(1, 3, 1)
    for name in gflops:
        plt.plot(Ns, gflops[name], label=name)
    plt.xlabel('N (Matrix of size N*N)')
    plt.ylabel('Fake GFLOP/s')
    plt.legend(loc='lower right')
    plt.grid(True)

    plt.subplot(1, 3, 2)
    start, end = 0, 18
    for name in errors:
        plt.semilogy(Ns[start:end], errors[name][start:end], label=name)
    plt.xlabel('N (Matrix of size N*N)')
    plt.ylabel('Absolute Eucleidian Distance (Error)')
    plt.legend(loc='upper left')
    plt.grid(True)

    plt.subplot(1, 3, 3)
    lastvals = [errors[name][end] for name in errors]
    x = np.arange(len(lastvals))
    plt.bar(x, lastvals, align='center')
    plt.ylabel('Absolute Eucleidian Distance (Error) for N = 1000')
    plt.xticks(x, errors.keys(), rotation=30)

    plt.show()

if __name__ == "__main__":
    main()

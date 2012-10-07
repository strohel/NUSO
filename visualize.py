#!/usr/bin/env python
# -*- Coding:utf-8 -*-

import matplotlib.pyplot as plt

import fileinput


def main():
    Ns = []
    gflops = {}
    for line in fileinput.input():
        (N, iters, implementations) = line.split(';', 2)
        N = int(N.split()[2])
        Ns.append(N)
        implementations = implementations.strip('\n\r \t;')
        for implementation in implementations.split(';'):
            implementation = implementation.split()
            name = implementation[0].strip(' :')
            curr_gflops = float(implementation[3])
            if name in gflops:
                gflops[name].append(curr_gflops)
            else:
                gflops[name] = [curr_gflops]

    for name in gflops:
        plt.semilogx(Ns, gflops[name], label=name)
    plt.xlabel('N (Matrix of size N*N)')
    plt.ylabel('GFLOP/s')
    plt.legend(loc='lower right')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()

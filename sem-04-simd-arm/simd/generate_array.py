#!/usr/bin/python3

import random
import sys

if (__name__ == "__main__"):
    if (len(sys.argv) != 4):
        print("usage: python3 generate_array.py size mean stddev", file=sys.stderr)
        sys.exit(1)
    size = int(sys.argv[1])
    mean = float(sys.argv[2])
    stddev = float(sys.argv[3])


    random.seed(1)
    array = [random.gauss(mean, stddev) for i in range(size)]
    
    print(size)
    for a in array:
        print(f"{a:.6f} ", end="")
    print()

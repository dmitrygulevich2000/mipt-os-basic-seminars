#!/usr/bin/python3
import sys

lines = [l.strip() for l in sys.stdin if len(l.strip()) > 0]
lines.sort(key=lambda l: int(l.split("-")[0], 16))

for l in lines:
    print(l)

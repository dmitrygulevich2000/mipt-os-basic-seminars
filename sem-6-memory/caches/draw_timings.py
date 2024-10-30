import matplotlib.pyplot as plt
import sys

def volume_label(bytes):
    suffixes = ["B", "KB", "MB", "GB"]
    bytes = float(bytes)

    i = 0
    while bytes > 1000:
        bytes /= 1000
        i += 1
    return "{:g}".format(round(bytes, 1)) + suffixes[i]

def draw_ram(filename="timings.txt"):
    with open(filename, "r") as f:
        entries = list(map(lambda l: l.strip().split(), f.readlines()))
        grid = [int(e[0][:-1]) for e in entries]
        timings = [float(e[1]) for e in entries]
    caches = [32*1024, 512*1024, 4*1024*1024]
    
    fig, ax = plt.subplots(1, 1, figsize=(13, 5))
    ax.plot(grid, timings, "b^-")
    ax.vlines(caches, 0, max(timings), "r")
    
    ax.set_xscale("log")
    labels = [volume_label(bytes) for bytes in grid]
    ax.set_xticks(grid[::2], labels[::2])
    ax.set_yticks([i for i in range(0, 25, 5)] + [i for i in range(30, int(max(timings)), 10)])
    ax.set_ylabel("ns")
    ax.grid()
    fig.savefig("caches.png", bbox_inches="tight")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python3 draw_timings.py filename")
        exit()
    
    draw_ram(sys.argv[1])

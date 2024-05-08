import random
import sys

if len(sys.argv) != 3:
    print(f'usage: {sys.argv[0]} <outfile> <num_vertices>')
    sys.exit(0)

mean = int(sys.argv[2])
dev = mean / 10

num_vertices = int(random.normalvariate(mean, dev))
num_edges = int(num_vertices**2 / 4)
edges = { i: [] for i in range(num_vertices) }

with open(sys.argv[1], 'w') as f:
    f.write(f'{num_vertices} {num_vertices} {num_edges}\n')

    while num_edges > 0:
        u = random.randint(0, num_vertices - 1)
        v = random.randint(0, num_vertices - 1)
        
        if u != v and v not in edges[u]:
            w = random.random() * 100
            f.write(f'{u + 1} {v + 1} {w}\n')

            edges[u].append(v)
            edges[v].append(u)
            num_edges -= 1

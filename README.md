# Graph Spanners

## Brief

Implementation and analysis of the algorithms described in:

- Baswana, Surender & Sen, Sandeep. (2003). A Simple and Linear Time Randomized Algorithm for Computing Sparse Spanners in Weighted Graphs

## Steps to build

```console
mkdir build/
cd build/
cmake ..
make
```

## Steps to run

```console
./build/spanner input/<graph.mtx> <algorithm> [<k>]

# algorithm:
#     0: 3-spanner
#     1: (2k - 1)-spanner (vertex-cluster joining)
#     2: (2k - 1)-spanner (cluster-cluster joining)
```

## Using scripts

### 1. Generating dense graphs

```console
python scripts/generator.py <num_vertices>
```

**Note:** Currently set to write to input/custom.mtx.

### 2. Running tests

```console
./tests.sh <algorithm> [<k>]
```

## File Structure

- ``headers/`` - Class and function definitions.

- ``input/`` - Undirected Weighted Graphs from [SuiteSparse Matrix Collection](https://sparse.tamu.edu/) (Matrix Market).

- ``plots/`` - Data concerning runtime behaviour of algorithm.

- ``report/`` - Project report.

- ``scripts/`` - Helper scripts for input generation and testing.

- ``src/`` - Main C++/CUDA implementation.

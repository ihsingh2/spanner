# Graph Spanners

## Brief

Implementation and analysis of the algorithms described in:

- Baswana, Surender & Sen, Sandeep. (2003). A Simple and Linear Time Randomized Algorithm for Computing Sparse Spanners in Weighted Graphs

## Steps to run

```console
mkdir build/; cd build/
cmake ..; make
cd ..
./build/spanner input/<graph.mtx> <t>
```

## Using scripts

### 1. Generating dense graphs

```console
python scripts/generator.py <num_vertices>
```

**Note:** Currently set to write to input/custom.mtx.

### 2. Running tests

```console
./tests.sh <t>
```

## File Structure

- ``headers/`` - Class and function definitions.

- ``input/`` - Undirected Weighted Graphs from [SuiteSparse Matrix Collection](https://sparse.tamu.edu/) (Matrix Market).

- ``scripts/`` - Helper scripts for input generation and testing.

- ``src/`` - Main C++/CUDA implementation.

# Graph Spanners

## Brief

Implementation and analysis of the algorithms described in:

- Baswana, Surender & Sen, Sandeep. (2003). A Simple and Linear Time Randomized Algorithm for Computing Sparse Spanners in Weighted Graphs

## Instructions

### Steps to build

```shell
mkdir build/
cd build/
cmake ..
make
```

### Steps to run

```shell
./build/spanner input/<graph.mtx> <algorithm> [<k>]

# algorithm:
#     0: 3-spanner
#     1: (2k - 1)-spanner (vertex-cluster joining)
#     2: (2k - 1)-spanner (cluster-cluster joining)
```

### Using scripts

#### 1. Generating dense graphs

```shell
python scripts/generator.py <outfile> <num_vertices>
```

#### 2. Running tests

```shell
./tests.sh <algorithm> [<k>]
```

### Using macros

Change (append/remove) the following line in [CMakeLists.txt](CMakeLists.txt).

```cmake
target_compile_definitions(spanner PRIVATE TIMING)
```

- ``VALIDATION`` - Validate the algorithm's output by running APSP.

- ``TIMING`` - Measure the time taken by every stage of the algorithm.

## File Structure

- ``headers/`` - Class and function definitions.

- ``input/`` - Undirected Weighted Graphs from [SuiteSparse Matrix Collection](https://sparse.tamu.edu/) (Matrix Market).

- ``plots/`` - Data concerning runtime behaviour of algorithm.

- ``report/`` - Project report.

- ``scripts/`` - Helper scripts for input generation and testing.

- ``src/`` - Main C++/CUDA implementation.

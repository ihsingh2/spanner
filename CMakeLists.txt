cmake_minimum_required(VERSION 3.18)
set(CMAKE_BUILD_TYPE Release)

project(spanner
    LANGUAGES CXX CUDA
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories(headers/)

add_executable(spanner
    src/apsp.cu
    src/graph.cpp
    src/main.cpp
    src/minheap.cpp
    src/spanner.cpp
    src/three_spanner.cpp
    src/util.cpp
)

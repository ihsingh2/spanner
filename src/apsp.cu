__global__ void floyd_warshall_kernel(const int num_vertices, double *output, double inf, int k) {
    int i = blockDim.y * blockIdx.y + threadIdx.y;
    int j = blockDim.x * blockIdx.x + threadIdx.x;
    if ((i < num_vertices) && (j < num_vertices)) {
        int ij = i * num_vertices + j;
        int ik = i * num_vertices + k;
        int kj = k * num_vertices + j;
        if (output[ik] != inf && output[kj] != inf) {
            if (output[ij] > output[ik] + output[kj]) {
                output[ij] = output[ik] + output[kj];
            }
        }
    }
}

void floyd_warshall_gpu(const double *graph, int num_vertices, double *output) {
    int THREADS_PER_BLOCK_SIDE = 16;
    int BLOCKS_PER_GRAPH_SIDE = ((num_vertices + THREADS_PER_BLOCK_SIDE - 1) / THREADS_PER_BLOCK_SIDE);
    dim3 blocks(BLOCKS_PER_GRAPH_SIDE, BLOCKS_PER_GRAPH_SIDE, 1);
    dim3 threadsPerBlock(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE, 1);
    int size = sizeof(double) * num_vertices * num_vertices;

    double *dev_output;
    cudaMalloc(&dev_output, size);
    cudaMemcpy(dev_output, graph, size, cudaMemcpyHostToDevice);

    double inf = std::numeric_limits<double>::max();
    for (int k = 0; k < num_vertices; k++) {
        floyd_warshall_kernel<<<blocks, threadsPerBlock>>>(num_vertices, dev_output, inf, k);
    }

    cudaMemcpy(output, dev_output, size, cudaMemcpyDeviceToHost);
    cudaFree(dev_output);
}

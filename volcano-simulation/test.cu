/*

    EJECUTAR SI SE DESEA PROBRAR EL FUNCIONAMIENTO DE CUDA

*/

/*
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include <time.h>

using namespace std;
// Kernel function to add the elements of two arrays
__global__

void add(int n, float* x, float* y)
{
    int index = threadIdx.x;
    int stride = blockDim.x;
    for (int i = index; i < n; i += stride)
        y[i] = x[i] + y[i];
}

int main(void)
{
    long N = 1 << 20;
    float* x, * y;

    // Allocate Unified Memory – accessible from CPU or GPU
    cudaMallocManaged(&x, N * sizeof(float));
    cudaMallocManaged(&y, N * sizeof(float));

    // initialize x and y arrays on the host
    for (long i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    unsigned int time0, time1;
    time0 = clock();

    add << < 1, 1 >> > (N, x, y);

    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++) {
        maxError = fmax(maxError, fabs(y[i] - 3.0f));
        //std::cout << "y[i] = " << y[i] << endl;
    }

    std::cout << "Max error: " << maxError << std::endl;
    time1 = clock();
    double timeCreation = double(time1 - time0) / CLOCKS_PER_SEC;
    printf("time %lf", timeCreation);
    // Free memory
    cudaFree(x);
    cudaFree(y);

    return 0;
}*/
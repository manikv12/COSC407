#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void test(int *thr, int n);

int main()
{
    int n = 100;
    int *resThr;
    int *memThr = (int*)calloc(n*n,sizeof(int));
    memset(memThr, 0, sizeof(int)*(n*n));
    cudaMalloc((void **) &resThr, sizeof(int) * (n*n));
    cudaMemset(resThr, 0, sizeof(int)*(n*n));
    test<<<n,n,1>>>(resThr, n*n);
    cudaThreadSynchronize();
    cudaMemcpy(memThr,resThr,sizeof(int)*(n*n),cudaMemcpyDeviceToHost);
    cudaFree(resThr);
    int i;
    for(i=0;i<(n*n);i++)
    {
        printf("%d, ",memThr[i]);
    }
}
__global__ void test(int *thr, int n)
{
    int idx = (blockDim.x*blockIdx.x)+threadIdx.x;
    
    if(idx <= n) 
        thr[idx] = idx;
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cuda.h>
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
__global__ void edt(int N, int count, int *dev_arr, int *matrix);
int main()
{
    int matrix1[5][5] = {{1,1,1,1,1},{1,1,1,1,1},{1,1,0,1,1},{1,1,1,1,1},{1,1,1,1,1}};
    //int matrix2[5][5] = {{1,1,1,1,1},{1,0,1,1,1},{1,1,0,1,1},{1,1,1,1,1},{1,1,1,1,1}};
    //int matrix3[5][5] = {{1,1,1,1,1},{1,0,1,0,1},{1,1,1,1,1},{1,1,0,1,1},{1,1,1,1,1}};
    int squared_matrix1[5][5];
    //int squared_matrix2[5][5];
    //int squared_matrix3[5][5];
    int a,b, N = 5, count = 0;
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            squared_matrix1[a][b] = 1000;
            //squared_matrix2[a][b] = 1000;
            //squared_matrix3[a][b] = 1000;
        }
    }
    
    printf("\nCUDA Implementation\n");

    /* Count number of 0's in matrix */
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            if(0==matrix1[a][b])
                count++;
        }
    }
    
    int *host_array = (int*)malloc(count*sizeof(int)*(N*N)); 
    int *dev_array, *matrix;
    cudaMalloc(&dev_array, count*sizeof(int)*(N*N));
    cudaMalloc(&matrix, sizeof(matrix1));
    cudaMemcpy((void **)matrix, matrix1, sizeof(matrix1), cudaMemcpyHostToDevice);
    edt<<<N,N>>>(N, count, dev_array, matrix);
    cudaMemcpy(host_array, dev_array, count*sizeof(int)*(N*N), cudaMemcpyDeviceToHost);
   

    /* TODO
        This will return an array containing 'count' matrices. It will then
        iterate through all of them at each index and set the least valued
        integer as the value at each index in squared_matrix
    */

    printf("\n"); 
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            printf("%d ", host_array[a+b]);
        }
        printf("\n");
    }
    printf("\n");

    cudaFree(dev_array);
    cudaFree(matrix);
    free(host_array);
    return 0;
}

__global__ void edt(int N, int count, int *dev_arr, int *matrix)
{
    int idx = blockIdx.x+threadIdx.x;
    if(idx < (N*N))
    {
        int a,b;
        if(matrix[idx] == 0)
        {
            dev_arr[idx] = 10;
            for(a=0;a<N;a++)
            {
                for(b=0;b<N;b++)
                {
                    int tmp = abs(pow((float)(a-idx),2.0f)) + abs(pow((float)(b-idx),2.0f));
                    if((matrix[a+b] > tmp))
                    {
                        matrix[a+b] = 9;
                    }
                }
            }

        }
        else
            dev_arr[idx] = 9;
    }
}












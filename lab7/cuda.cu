#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>
float h(float a, float b, int n);
__device__ float f_dev(float x, float y);

typedef struct{
    float val;
    float x;
    float y;
} point;

__global__ void f2(float hx, float hy, float xa, float ya, point *memBlock);
point *findMin(point *Points, int size);

int main()
{
    int n = 100;
    float eps = 0.000001;
    float hx, hy, xa = -1.0, xb = 1.0, ya = -1.0, yb = 1.0;
     
    while((h(xa, xb, n) >= eps) || (h(ya, yb, n) >= eps))
    {
        point *Points = (point*)malloc(sizeof(point)*(n*n));
        point *devPoints;
        cudaMalloc((void **) &devPoints, sizeof(point)*(n*n));
        hx = h(xa, xb, n);
        hy = h(ya, yb, n);
        f2<<<n,n,1>>>(hx, hy, xa, xb, devPoints);
        cudaThreadSynchronize();
        cudaMemcpy(Points, devPoints, sizeof(point)*(n*n), cudaMemcpyDeviceToHost);

        point *Min = findMin(Points, sizeof(point)*(n*n));

        xa = Min->x-hx;
        xb = Min->x+hx;
        ya = Min->y-hy;
        yb = Min->y+hy;
        printf("stuff\n");
        cudaFree(devPoints);
        free(Points);
        free(Min);
    }
    float minx = (xa + xb)/(float)2;
    float miny = (ya + yb)/(float)2;
    printf("xa: %f\nxb: %f\nya: %f\nyb: %f\nGlobal min: x = %f, y = %f\n", xa, xb, ya, yb, minx, miny);
    return 0;
}

__global__ void f2(float hx, float hy, float xa, float ya, point *memBlock)
{
    int idx = (blockDim.x*blockIdx.x)+threadIdx.x;
    float funct = f_dev(xa+(hx*blockIdx.x), ya+(hy*threadIdx.x));
    memBlock[idx].val = funct;
    memBlock[idx].x = xa + (hx*blockIdx.x);
    memBlock[idx].y = ya + (hy*threadIdx.x);
}

point* findMin(point *Points, int size)
{
    point *Min = (point *)malloc(sizeof(point));
    Min->val = Points[0].val;
    Min->x = Points[0].x;
    Min->y = Points[0].y;
    int i;
    for(i = 1;i < size;i++)
    {
        if(Points[i].val < Min->val)
        {
            Min->val = Points[i].val;
            Min->x = Points[i].x;
            Min->y = Points[i].y;
        }
    }
    return Min;
}

float h(float a, float b, int n)
{
    return (b-a)/(float)(n*n);
}
__device__ float f_dev(float x, float y)
{
    return(x*x*(4-2.1*x*x+(x*x*x*x)/3)+x*y+y*y*(-4+4*y*y));
}


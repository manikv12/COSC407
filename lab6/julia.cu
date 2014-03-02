/*
Attribution to http://rosettacode.org/wiki/Bitmap/Write_a_PPM_file
for help with writing to a .ppm file, and to 
https://github.com/smithbower/julia for help with the algorithm for the julia set.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda.h>
#include <cuda_runtime.h>
#define WIDTH 2000
#define HEIGHT 2000
#define NEWTON_IT 100
#define ZOOM 2.2f
#define EPSILON 0.01f
double f(double z);
void cpu_julia(int *matrix);
void gpu_julia_setup();

__device__ __host__ void complex_add(float a, float b, float c, float d, float *realOut, float *imgOut)
{
    *realOut = a + c;
    *imgOut = b + d;
}

__device__ __host__ void complex_sub(float a, float b, float c, float d, float *realOut, float *imgOut)
{
    *realOut = a - c;
    *imgOut = b - d;
}

__device__ __host__ void complex_mul(float a, float b, float c, float d, float *realOut, float *imgOut)
{
    *realOut = (a * c) - (b * d);
    *imgOut = (b * c) + (a * d);
}

__device__ __host__ void complex_div(float a, float b, float c, float d, float *realOut, float *imgOut)
{
    *realOut = ((a * c) + (b * d)) / (pow(c, 2) + pow(d, 2));
    *imgOut = ((b * c) - (a * d))/ (pow(c, 2) + pow(d, 2));
}

__global__ void gpu_julia(int *matrix, int height, int width, int iterations, float epsilon, float zoom)
{
    int threadID = (blockIdx.x * blockDim.x) + threadIdx.x;
    float newRe, newIm, oldRe, oldIm;
    float z_3_r, z_3_i, z_2_r, z_2_i, inner_r, inner_i;
    if(threadID < (height * width))
    {
        int x = (threadID / height);
        int y = (threadID % height);
        newRe = (((float)x / (float)width) - 0.5f) * 2.0f * zoom;
        newIm = ((float)height / (float)width) * (((float)y / (float)height) - 0.5f) * 2.0f * zoom;
        int i;
        for(i = 0; i < iterations; i++)
        {
            oldRe = newRe;
            oldIm = newIm;
            //Clear everything.
            z_3_r = z_3_i = z_2_r = z_2_i = inner_r = inner_i = 0;
            complex_mul(oldRe, oldIm, oldRe, oldIm, &z_2_r, &z_2_i); // z^2
            complex_mul(z_2_r, z_2_i, oldRe, oldIm, &z_3_r, &z_3_i); // z^3
            z_3_r -= 1.0f; //z^3 - 1
            z_2_r *= 3.0f; // 3z^2
            z_2_i *= 3.0f;
            complex_div(z_3_r, z_3_i, z_2_r, z_2_i, &inner_r, &inner_i); // ((z^3 - 1) / 3z^2)
            complex_sub(oldRe, oldIm, inner_r, inner_i, &newRe, &newIm); //z - ((z^3 - 1) / 3z^2)
            //If we've mostly converged, break out early.
            if (abs(newRe - oldRe) < epsilon && abs(newIm - oldIm) < epsilon)
                break;
        }
            if (abs(1.0f - newRe) < epsilon && abs(0 - newIm) < epsilon)
                matrix[threadID] = 1;
            else
            if (newRe - 0.5f < epsilon && 0.86603f - newIm < epsilon)
                matrix[threadID] = 2;
            else
            if (newRe - 0.5f < epsilon && newIm - 0.86603f < epsilon)
                matrix[threadID] = 3;
            else
                matrix[threadID] = 0;
    }
}

__host__ void gpu_julia_setup()
{
    int *gpu_device_matrix;
    int *gpu_matrix = (int*)malloc(sizeof(int) * WIDTH * HEIGHT);
    cudaMalloc((void **)&gpu_device_matrix, sizeof(int) * HEIGHT * WIDTH);
    gpu_julia<<<(WIDTH*HEIGHT/128),128>>>(gpu_device_matrix, HEIGHT, WIDTH, NEWTON_IT, EPSILON, ZOOM);
    cudaThreadSynchronize();
    cudaMemcpy(gpu_matrix, gpu_device_matrix, (sizeof(int) * HEIGHT * WIDTH), cudaMemcpyDeviceToHost); 
    //cpu_julia(gpu_matrix);

    int i, j;
    FILE *fp = fopen("julia.ppm", "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    for (j = 0; j < HEIGHT; ++j)
    {
        for (i = 0; i < WIDTH; ++i)
        {
            unsigned char color[3];
            int val = gpu_matrix[j*HEIGHT +i];
            switch(val)
            {
                case 1:
                    color[0] = 180;
                    color[1] = 82;
                    color[2] = 205;
                break;
                case 2:
                    color[0] = 255;
                    color[1] = 48;
                    color[2] = 48;
                    break;
                case 3:
                    color[0] = 0; 
                    color[1] = 205;
                    color[2] = 0;
                break;
                default:
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 205;              
                    break;
            }
            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);   
}

double f(double z)
{
    double g = pow(z, 3.0f) - 1.0f;
    double g_p = 3.0f * pow(z,2.0f);
    double f_z = z - ((double)g/(double)g_p);
    return f_z;  
}

 
void cpu_julia(int *matrix)
{
    int x,y,i;
    float newRe, newIm, oldRe, oldIm;
    float z_3_r, z_3_i, z_2_r, z_2_i, inner_r, inner_i;
    float ratio = (float)HEIGHT / (float)WIDTH;
    
    for(x = 0; x < WIDTH; x++)
    {
        for(y = 0; y < HEIGHT; y++)
        {
            newRe = (((float)x / (float)WIDTH) - 0.5f) * 2.0f * ZOOM;
            newIm = ratio * (((float)y / (float)HEIGHT) - 0.5f) * 2.0f * ZOOM;
            for(i = 0; i < NEWTON_IT; i++)
            {
                oldRe = newRe;
                oldIm = newIm;

                //Clear everything.
                z_3_r = z_3_i = z_2_r = z_2_i = inner_r = inner_i = 0;

                complex_mul(oldRe, oldIm, oldRe, oldIm, &z_2_r, &z_2_i); // z^2
                complex_mul(z_2_r, z_2_i, oldRe, oldIm, &z_3_r, &z_3_i); // z^3
                z_3_r -= 1.0f; //z^3 - 1

                z_2_r *= 3.0f; // 3z^2
                z_2_i *= 3.0f;

                complex_div(z_3_r, z_3_i, z_2_r, z_2_i, &inner_r, &inner_i); // ((z^3 - 1) / 3z^2)

                complex_sub(oldRe, oldIm, inner_r, inner_i, &newRe, &newIm); //z - ((z^3 - 1) / 3z^2)

                //If we've mostly converged, break out early.
                if (abs(newRe - oldRe) < EPSILON && abs(newIm - oldIm) < EPSILON)
                    break;
            }
            if (abs(1.0f - newRe) < EPSILON && abs(0 - newIm) < EPSILON)
                matrix[x * HEIGHT + y] = 1;
            else
            if (newRe - 0.5f < EPSILON && 0.86603f - newIm < EPSILON)
                matrix[x * HEIGHT + y] = 2;
            else
            if (newRe - 0.5f < EPSILON && newIm - 0.86603f < EPSILON)
                matrix[x * HEIGHT + y] = 3;
            else
                matrix[x * HEIGHT + y] = 0;
        }
    }
}

int main()
{
    int *matrix = (int*)malloc(sizeof(int) * WIDTH * HEIGHT);
    cpu_julia(matrix);
    gpu_julia_setup();
    return 0;
}  

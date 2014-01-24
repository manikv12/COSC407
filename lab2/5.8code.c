#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int *ser_ans(int n);
int *par_ans(int n);
int main(int argc, char *argv[])
{
    int n,i;
    int *ser_arr;
    int *par_arr;

    n = 10;
    printf("Test 1\n");
    ser_arr = ser_ans(n);
    par_arr = par_ans(n);
    for(i = 0; i < n; i++)
    {
        if(ser_arr[n-1] != par_arr[n-1]) 
            printf("Test failed for n of value: %d\n", n);
    }
    free(ser_arr);
    free(par_arr);

    n = 100;
    printf("Test 2\n");
    ser_arr = ser_ans(n);
    par_arr = par_ans(n);
    for(i = 0; i < n; i++)
    {
        if(ser_arr[n-1] != par_arr[n-1]) 
            printf("Test failed for n of value: %d\n", n);
    }
    free(ser_arr);
    free(par_arr);

    n = 1000;
    printf("Test 3\n");
    ser_arr = ser_ans(n);
    par_arr = par_ans(n);
    for(i = 0; i < n; i++)
    {
        if(ser_arr[n-1] != par_arr[n-1]) 
            printf("Test failed for n of value: %d\n", n);
    }
    free(ser_arr);
    free(par_arr);

    n = 10000;
    printf("Test 4\n");
    ser_arr = ser_ans(n);
    par_arr = par_ans(n);
    for(i = 0; i < n; i++)
    {
        if(ser_arr[n-1] != par_arr[n-1]) 
            printf("Test failed for n of value: %d\n", n);
    }
    free(ser_arr);
    free(par_arr);

    n = 100000;
    printf("Test 5\n");
    ser_arr = ser_ans(n);
    par_arr = par_ans(n);
    for(i = 0; i < n; i++)
    {
        if(ser_arr[n-1] != par_arr[n-1]) 
            printf("Test failed for n of value: %d\n", n);
    }
    free(ser_arr);
    free(par_arr);

    return 0;
}

int *par_ans(int n)
{
    double iTime = omp_get_wtime();
    int *arr = malloc(sizeof(int) * n);
    int i;
    arr[0] = 0;
    #pragma omp parallel for num_threads(2)
    for(i = 0; i < n; i++)
        arr[i] = ((i*(i+1))/2);
    #pragma omp barrier
    double fTime = omp_get_wtime() - iTime;
    printf("Parallel ran in: %fs\n", fTime);    
    return arr;
}
int *ser_ans(int n)
{
    double iTime = omp_get_wtime();
    int *arr = malloc(sizeof(int) * n);
    int i;
    arr[0] = 0;
    for(i = 0; i < n; i++)
        arr[i] = arr[i-1] + i;
    double fTime = omp_get_wtime() - iTime;
    printf("Serial version ran in: %fs\n", fTime);
    return arr;
}

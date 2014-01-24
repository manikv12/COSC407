#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THR 8

#define ARR_LEN(array) (sizeof(array)/sizeof(array[0]))

int main(int argc, char *argv[])
{
    int v1[] = {1,2,3,4,5,6,7,8,9,0};
    int v2[] = {-1,-2,-3,-4,-5,-6,-7,-8,-9,0};
    int vf[10]; 
    int sum = 0;
    int i = 0;
#pragma omp parallel for num_threads(NUM_THR)
    for(i = 0; i < ARR_LEN(v1); i++)
        vf[i] = v1[i] + v2[i];
    
#pragma omp parallel for num_threads(NUM_THR) reduction(+:sum)
    for(i = 0;i < ARR_LEN(v1);i++)
        sum += vf[i];

    if(sum == 0)
    {
        printf("Sum is %d!\n", sum);
        return EXIT_SUCCESS;
    }
    else{
        printf("Sum is %d, FAIL!\n", sum);
        return EXIT_FAILURE;
    }
}

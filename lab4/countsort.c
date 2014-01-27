#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARR_SIZE 10000
#define NUM_THR 32

void countsort();
int main(int argc, char *argv[])
{
    int i;
    int arr[ARR_SIZE];
    srand(getpid());
    #pragma omp parallel for num_threads(NUM_THR)
    for(i = 0;i < ARR_SIZE; i++)
       arr[i] = rand()%ARR_SIZE; 
    
    #pragma omp barrier
    float iTime = omp_get_wtime();
    countsort(&arr, ARR_SIZE);
    float fTime = omp_get_wtime() - iTime;
    //for(i = 0; i<ARR_SIZE; i++) printf("%d, ", arr[i]);
    printf("Time to sort in Parallel: %f\n", fTime);
    
    #pragma omp parallel for num_threads(NUM_THR)
    for(i = 0;i < ARR_SIZE; i++)
       arr[i] = rand()%ARR_SIZE; 
    
    iTime = omp_get_wtime();
    countsort(&arr, ARR_SIZE, 1);
    fTime = omp_get_wtime() - iTime;
    //for(i = 0; i<ARR_SIZE; i++) printf("%d, ", arr[i]);
    printf("Time to sort in Serial: %f\n", fTime);




    return 0;
}

void countsort(int a[], int n, int num_thr)
{
    int i,j,count=0;
    int *tmp = malloc(sizeof(int) * n);
    #pragma omp parallel for num_threads(num_thr) private(j) reduction(+:count)
    for(i = 0; i < n; i++)
    {
        count = 0;
        for(j = 0; j < n; j++)
        {
            if(a[j] < a[i])
                    count++;
            else
            {
                if((a[j] == a[i]) && (j < i)) 
                    count++;
            }
        }
        tmp[count] = a[i];
    }
    #pragma omp barrier

    #pragma omp parallel for num_threads(num_thr)
    for(i = 0; i<n; i++)
        memcpy(&a[i], &tmp[i], sizeof(int));
    free(tmp);
}

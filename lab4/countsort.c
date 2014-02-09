#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARR_SIZE 10000
#define NUM_THR 32

void countsort();
int compare_ints();
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
    printf("Time to sort in Parallel: %f\n", fTime);
    
    #pragma omp parallel for num_threads(NUM_THR)
    for(i = 0;i < ARR_SIZE; i++)
       arr[i] = rand()%ARR_SIZE; 
    
    iTime = omp_get_wtime();
    countsort(&arr, ARR_SIZE, 1);
    fTime = omp_get_wtime() - iTime;
    printf("Time to sort in Serial: %f\n", fTime);
    
    #pragma omp parallel for num_threads(NUM_THR)
    for(i = 0;i < ARR_SIZE; i++)
        arr[i] = rand()%ARR_SIZE;
    
    iTime = omp_get_wtime();
    qsort(&arr, ARR_SIZE, sizeof(int), compare_ints);
    fTime = omp_get_wtime() - iTime;
    printf("Time to sort in qsort: %f\n", fTime);


    printf("\nTest 1:\nUnsorted random array: [");
    for(i = 0;i < 5; i++){
        arr[i] = rand()%5;
        printf("%d, ", arr[i]);
    }
    countsort(&arr, 5, NUM_THR);
    printf("]\nSorted array: [");
    for(i = 0; i < 5; i++)
        printf("%d, ", arr[i]);
    printf("]\n");

    printf("\nTest 2:\nUnsorted random array: [");
    for(i = 0;i < 10; i++){
        arr[i] = rand()%10;
        printf("%d, ", arr[i]);
    }
    countsort(&arr, 10, NUM_THR);
    printf("]\nSorted array: [");
    for(i = 0; i < 10; i++)
        printf("%d, ", arr[i]);
    printf("]\n");

    printf("\nTest 3:\nUnsorted random array: [");
    for(i = 0;i < 15; i++){
        arr[i] = rand()%15;
        printf("%d, ", arr[i]);
    }
    countsort(&arr, 15, NUM_THR);
    printf("]\nSorted array: [");
    for(i = 0; i < 15; i++)
        printf("%d, ", arr[i]);
    printf("]\n");

    printf("\nTest 4:\nUnsorted random array: [");
    for(i = 0;i < 20; i++){
        arr[i] = rand()%20;
        printf("%d, ", arr[i]);
    }
    countsort(&arr, 20, NUM_THR);
    printf("]\nSorted array: [");
    for(i = 0; i < 20; i++)
        printf("%d, ", arr[i]);
    printf("]\n");
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

int compare_ints(const void* a, const void* b)
{
    const int *arg1 = a;
    const int *arg2 = b;            
    return *arg1 - *arg2;
}

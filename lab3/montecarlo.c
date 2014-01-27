#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define DEF_THR_NUM 32

double montecarlo();
int main(int argc, char *argv[])
{
    if(argc<2) 
    {
        printf("Usage: %s <number of tosses>\n", argv[0]);
        exit(1);
    }
     
    int iterations = atoi(argv[1]);

    double pi_est = montecarlo(iterations, DEF_THR_NUM);
    printf("Estimate for Pi: %f\n", pi_est);
    
    return 0;
}

double montecarlo(int iterations, int thread_count)
{
    unsigned long long int num_in_circle = 0;
    srand(time(NULL) * getpid());
    int i;
    #pragma omp parallel for reduction(+:num_in_circle) num_threads(thread_count)
    for(i = 0; i < iterations; i++)
    {
        double x = (double)rand()/(double)RAND_MAX;
        double y = (double)rand()/(double)RAND_MAX;
        double distance_squared = (x*x)+(y*y);
        if(distance_squared <= 1) num_in_circle += 1;
    }
    #pragma omp barrier
    double pi_est = (4*((double)num_in_circle))/((double)iterations);
    return pi_est;
}

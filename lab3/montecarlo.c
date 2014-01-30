#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define DEF_THR_NUM 4

double montecarlo(int iterations, int thread_count);
int main(int argc, char *argv[])
{
    if(argc<2) 
    {
        printf("Usage: %s <number of tosses>\n", argv[0]);
        exit(1);
    }
     
    int iterations = atoi(argv[1]);

    //float iTime_par = omp_get_wtime();
    double pi_est_par = montecarlo(iterations, DEF_THR_NUM);
    //float fTime_par = omp_get_wtime() - iTime_par;
    printf("Parallel estimate for Pi: %f\n\n", pi_est_par);

    //float iTime_ser = omp_get_wtime();
    double pi_est_ser = montecarlo(iterations, 1);
    //float fTime_ser = omp_get_wtime() - iTime_ser;
    printf("Serial estimate for Pi: %f\n\n", pi_est_ser);
    
    double test_1 = montecarlo(1000, DEF_THR_NUM);
    printf("1000 iterations, Pi = %f\n\n", test_1);
    double test_2 = montecarlo(10000, DEF_THR_NUM);
    printf("10000 iterations, Pi = %f\n\n", test_2);
    double test_3 = montecarlo(100000, DEF_THR_NUM);
    printf("100000 iterations, Pi = %f\n\n", test_3);
    double test_4 = montecarlo(1000000, DEF_THR_NUM);
    printf("1000000 iterations, Pi = %f\n\n", test_4);
    double test_5 = montecarlo(10000000, DEF_THR_NUM);
    printf("10000000 iterations, Pi = %f\n\n", test_5);

    return 0;
}
double montecarlo(int iterations, int thread_count)
{
    unsigned long long int num_in_circle = 0;
    srand(time(NULL));
    int i;
    double x,y,dist_sq;

    float iTime = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) reduction(+:num_in_circle) private(x,y,dist_sq)
    for(i = 0; i < iterations; i++)
    {
        x = (double)rand()/(double)RAND_MAX;
        y = (double)rand()/(double)RAND_MAX;
        dist_sq = (x*x)+(y*y);
        if(dist_sq <= 1) num_in_circle += 1;
    }
    double pi_est = (4*((double)num_in_circle))/((double)iterations); 
    float fTime = omp_get_wtime() - iTime;
    printf("Time to run: %f\n", fTime);
    return pi_est;
}

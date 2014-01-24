#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);
double f(double x);   
double Trap(double a, double b, int n, double* global_result_p);
int test();
typedef struct
{
    double value;
    double time;
}data;
void parallel_compute(data *par_data, double a, double b, int n, int thread_count);
void serial_compute(data *ser_data, double a, double b, int n);

int main(int argc, char* argv[]) {
   double  global_result = 0.0;  
   double  a, b;                
   int     n;                  
   int     thread_count;
    
   if(0 != test())\
   {
       printf("TESTS FAILED\n");
       exit(1);
   }
   printf("TESTS PASSED\n");
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);

   if (n % thread_count != 0) Usage(argv[0]);
   
   data *par_data = malloc(sizeof(data));
   data *ser_data = malloc(sizeof(data));

   parallel_compute(par_data, a, b, n, thread_count);
   serial_compute(ser_data, a, b, n);

   printf("Parallel value: %f\nSerial value: %f\n\nParallel time: %f\nSerial time: %f\n",\
        par_data->value, ser_data->value, par_data->time, ser_data->time);
   return 0;
} 

void parallel_compute(data *par_data, double a, double b, int n, int thread_count)
{
    par_data->time = 0.0;
    par_data->value = 0.0;
    double iTime = omp_get_wtime();
    #pragma omp parallel num_threads(thread_count)
    {
        double my_result = 0.0;
        my_result += Trap(a, b, n, NULL);
        #pragma omp critical
        par_data->value += my_result;
    }
   #pragma omp barrier
   par_data->time = omp_get_wtime() - iTime;
}

void serial_compute(data *ser_data, double a, double b, int n)
{
    ser_data->time = 0.0;
    ser_data->value = 0.0;
    double iTime = omp_get_wtime();
    #pragma omp parallel num_threads(1)
    {
        double my_result = 0.0;
        my_result += Trap(a, b, n, NULL);
        #pragma omp critical
        ser_data->value += my_result;
    }
   #pragma omp barrier
   ser_data->time = omp_get_wtime() - iTime;
}

int test()
{
    int success = 0;

    if(4 != f(2)) success = 1;
    if(36 != f(6)) success = 1;
   
    if(52 != (int)Trap(1,5,1,NULL)) success = 1;
    if(106 != (int)Trap(3,7,4,NULL)) success = 1;
    return success;
}
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "   number of trapezoids must be evenly divisible by\n");
   fprintf(stderr, "   number of threads\n");
   exit(0);
}  /* Usage */

double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */

double Trap(double a, double b, int n, double* global_result_p) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   h = (b-a)/n; 
   local_n = n/thread_count;  
   local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; 
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h; 

   return my_result;

}  /* Trap */

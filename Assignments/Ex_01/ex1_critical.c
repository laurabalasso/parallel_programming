#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

double local_sum(double local_a, double local_b, double h ){
    
    double local_result = 0.0;
    
    while(local_a<=local_b){
        double x_i = local_a + h*0.5;
        local_result += 1.0/(1.0+x_i*x_i);
        local_a = local_a+h;
    }
    local_result = h * local_result;
    return local_result;
}

int main(int argc, char* argv[]){
  double global_result = 0.0;
  int a = 0;
  int b = 1;
  double n = 100000000;

  double start = omp_get_wtime();

#pragma omp parallel
  {
    double h = 1.0/n;
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();

    int local_n = n / nthreads;
    double local_a = a + tid * local_n * h;
    double local_b = local_a + local_n * h;

    double local_result = local_sum(local_a, local_b, h );
#pragma omp critical
    global_result += local_result;

  }

  double end = omp_get_wtime();
  double time = end -start;

  global_result = 4*global_result;
  printf("pi approximation: %lf\n", global_result);
  printf("time to solution: %lf\n", time);
  return 0;

}



#include <stdio.h>
#include <omp.h>

double calculate_pi(void) {
  const long nb_pas = 1000000000;
  double pas = 1.0 / (double) nb_pas;

  double sum = 0.0;

  #pragma omp parallel
  {
    double x;
    double psum = 0.0;

    #pragma omp for
    for (long i = 1; i <= nb_pas; i++) {
      x = (i - 0.5) * pas;
      psum += 4.0 / (1.0 + x * x);
    }

    #pragma omp atomic
    sum += psum;
  }
  double pi = pas * sum;
  return pi;
}

int main(void) {
  // Ex1
  /*
  #pragma omp parallel
  {
    printf("Total threads : %d\n", omp_get_num_threads());
    printf("Thread number : %d\n", omp_get_thread_num());
  }
  */

  // Ex2
  /*
  #pragma omp parallel
  {
    int start = omp_get_thread_num() * 10;
    for (int i = start; i < start+10; i++) {
      printf("Thread number : %d, i = %d\n", omp_get_thread_num(), i);
    }
  }
  */
  /*
  #pragma omp parallel for
  for (int i = 0; i < 100; i++) {
    printf("Thread number : %d, i = %d\n", omp_get_thread_num(), i);
  }
  */
  // Ex3
  double start_time = omp_get_wtime();
  double pi = calculate_pi();
  double end_time = omp_get_wtime();
  double time_parallel = end_time - start_time;
  printf("PI = %f\n", pi);
  printf("Time = %f\n", time_parallel);

  omp_set_num_threads(1);
  start_time = omp_get_wtime();
  pi = calculate_pi();
  end_time = omp_get_wtime();
  printf("PI = %f\n", pi);
  printf("Time = %f\n", end_time - start_time);

  printf("Speedup = %f\n", (end_time - start_time) / time_parallel);
  return 0;
}

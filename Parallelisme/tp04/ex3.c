#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define N 1000

int main(void) {
  double A[N][N];
  double B[N];
  double x[N];

  // Random source
  srand(time(NULL));

  double start = omp_get_wtime();

  // Init
  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    double sum = 0;
    for (int j = 0; j < N; j++) {
      A[i][j] = ((float)rand()/RAND_MAX) * 100;
      sum += A[i][j];
    }
    A[i][i] = sum;
  }

  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    B[i] = ((float)rand()/RAND_MAX) * 100;
    x[i] = 0;
  }

  // Solve

  int converged = 0;
  double epsilon = 0.0001;
  while (!converged) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
      double sum = 0;
      for (int j = 0; j < N; j++) {
        if (i != j) {
          sum += A[i][j] * x[j];
        }
      }
      x[i] = (B[i] - sum) / A[i][i];
    }

    // Check convergence
    converged = 1;
    // Can't really parallelize this as you can't break out of a parallel loop
    for (int i = 0; i < N && converged; i++) {
      double sum = 0;
      for (int j = 0; j < N; j++) {
        sum += A[i][j] * x[j];
      }
      if (fabs(sum - B[i]) > epsilon) {
        converged = 0;
      }
    }
  }

  double end = omp_get_wtime();

  // Print
  for (int i = 0; i < N; i++) {
    printf("%f ", x[i]);
  }

  printf("\nTime: %f\n", end - start);
}

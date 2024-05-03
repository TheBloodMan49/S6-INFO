#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#define MAX 100
#define N 150
#define M 100
#define THRESHOLD 0.00001

int main(void) {
  double T[N+2][M+2];
  double T1[N+2][M+2];

  struct timeval start, stop;

  // Init
  #pragma omp parallel for
  for (int i = 0; i < N+2; i++) {
    for (int j = 0; j < M+2; j++) {
      if (i == 0 || i == N+1 || j == 0 || j == M+1)
        T[i][j] = MAX;
      else
        T[i][j] = 0;
    }
  }

  gettimeofday(&start, NULL);

  // Iteration
  double delta;
  do {
    delta = 0;
    #pragma omp parallel for reduction(+:delta)
    for (int i = 1; i < N+1; i++) {
      for (int j = 1; j < M+1; j++) {
        T1[i][j] = (T[i][j] + T[i-1][j] + T[i+1][j] + T[i][j-1] + T[i][j+1]) / 5;
        delta += fabs(T1[i][j] - T[i][j]);
      }
    }

    // Swap matrices
    #pragma omp parallel for
    for (int i = 1; i < N+1; i++) {
      for (int j = 1; j < M+1; j++) {
        T[i][j] = T1[i][j];
      }
    }
  } while (delta > THRESHOLD);

  gettimeofday(&stop, NULL);
  double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

  for (int i = 0; i < N+2; i++) {
    for (int j = 0; j < M+2; j++) {
      printf("%f ", T[i][j]);
    }
    printf("\n");
  }

  printf("Time: %lf\n", secs);

  return 0;
}

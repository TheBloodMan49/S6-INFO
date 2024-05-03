#include <stdio.h>
#include <math.h>
#include <omp.h>

#define MAX 2000000

int main(void) {
  int a[MAX];

  double start = omp_get_wtime();

  // Init
  #pragma omp parallel for
  for (int i = 0; i < MAX; i++) {
    a[i] = i;
  }

  // Sieve
  for (int i = 2; i < sqrt(MAX); i++) {
    if (a[i] > 0) {
      #pragma omp parallel for
      for (int j = i*i; j < MAX; j += i) {
        a[j] = 0;
      }
    }
  }

  // Remove non-primes
  int p = 0;
  for (int i = 0; i < MAX; i++) {
    if (a[i] > 0) {
      a[p] = a[i];
      p++;
    }
  }

  double end = omp_get_wtime();

  // Print
  for (int i = 0; i < p; i++) {
    printf("%d ", a[i]);
  }

  printf("\nTime: %f\n", end - start);

  return 0;
}
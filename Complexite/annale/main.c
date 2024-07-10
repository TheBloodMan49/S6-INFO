#include <stdio.h>
#include <string.h>
#include <limits.h>

#define M 17

int n = 7;
char** words;

int cout(int i) {
  int sum = 0;
  for (int k = i; k <= n; k++) {
    sum += strlen(words[k]);
  }
  if (n-i+sum <= M) {
    return 0;
  } else {
    int curr_min = INT_MAX;
    for (int j = i; j <= n; j++) {
      sum = 0;
      for (int k = i; k <= j; k++) {
        sum += strlen(words[k]);
      }
      if (j-i+sum <= M) {

      }
    }
  }
}

int main(void) {
  printf("Hello, World!\n");
  return 0;
}

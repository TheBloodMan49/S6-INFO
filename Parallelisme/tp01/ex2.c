#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 1000
int V[N];

void* somme(void* data) {
    long int tid = (long int) data;
    long int start = tid*100;
    long int end = start + 100;
    long int i;
    long int sum = 0;
    for (i = start; i < end; i++) {
        sum += V[i];
    }

    pthread_exit((void*) sum);
}

int main(int argc, char* argv[]){
    long int i;
    int t_id;
    pthread_t p_thread[10];
    pthread_attr_t attr[10];
    long int partial_sum[10];
    long int sum = 0;

    for (i = 0; i < 1000; i++) {
        V[i] = i;
    }

    for (i = 0; i < 10; i++) {
        pthread_attr_init(&attr[i]);
        pthread_create(&p_thread[i], &attr[i], somme, (void*) i);
    }

    for (i = 0; i < 10; i++) {
        pthread_join(p_thread[i], (void*) &partial_sum[i]);
    }

    for (i = 0; i < 10; i++) {
        sum += partial_sum[i];
    }

    printf("Somme: %ld\n", sum);
    printf("Attendu: %d\n", N*(N-1)/2);

    return 0;
}
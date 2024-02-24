#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 10

typedef struct {
  int buffer[N];
  int head;
  int tail;
} circ_buffer;

typedef struct {
  circ_buffer *cb;
  int value[2*N];
} producer_args;

typedef struct {
  int num;
  circ_buffer *cb;
} consumer_args;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

int thread_id = 0;

void put(circ_buffer *cb, int value) {
  pthread_mutex_lock(&mutex);
  while ((cb->head + 1) % N == cb->tail) {
    pthread_cond_wait(&not_full, &mutex);
  }
  cb->buffer[cb->head] = value;
  cb->head = (cb->head + 1) % N;
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&not_empty);
}

int get(circ_buffer *cb) {
  pthread_mutex_lock(&mutex);
  while (cb->head == cb->tail) {
    pthread_cond_wait(&not_empty, &mutex);
  }
  int value = cb->buffer[cb->tail];
  cb->tail = (cb->tail + 1) % N;
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&not_full);
  return value;
}

void *producer(void *args) {
  producer_args *pargs = (producer_args *)args;
  circ_buffer *cb = pargs->cb;
  for (int i = 0; i < 2*N; i++) {
    put(cb, pargs->value[i]);
  }
  free(pargs);
  pthread_exit(NULL);
}

void *consumer(void *args) {
  consumer_args *cargs = (consumer_args *)args;
  circ_buffer *cb = cargs->cb;
  for (int i = 0; i < N/2; i++) {
    int value = get(cb);
    printf("Consumer %d: %d\n", cargs->num, value);
  }
  free(cargs);
  pthread_exit(NULL);
}

void spawn_producer(circ_buffer *cb, const int *value, int n){
  pthread_t thread;
  producer_args *pargs = malloc(sizeof(producer_args));
  pargs->cb = cb;
  for (int i = 0; i < n; i++) {
    pargs->value[i] = value[i];
  }
  pthread_create(&thread, NULL, producer, (void *)pargs);
}

void spawn_consumer(circ_buffer *cb) {
  pthread_t thread;
  consumer_args *cargs = malloc(sizeof(consumer_args));
  cargs->num = thread_id++;
  cargs->cb = cb;
  pthread_create(&thread, NULL, consumer, (void *)cargs);
}


int main() {
  circ_buffer cb;
  cb.head = 0;
  cb.tail = 0;

  int current = 0;

  while (1) {
    char input;
    scanf("%c", &input);
    if (input == 'p') {
      int value[2*N];
      for (int i = 0; i < 2*N; i++) {
        value[i] = current++;
      }
      spawn_producer(&cb, value, 2*N);
    } else if (input == 'c') {
      spawn_consumer(&cb);
    } else if (input == 'q') {
      break;
    }
  }

  return 0;
}
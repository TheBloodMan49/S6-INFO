#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 2

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
  circ_buffer *cb;
} consumer_args;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void put(circ_buffer *cb, int value) {
  pthread_mutex_lock(&mutex);
  if ((cb->head + 1) % N == cb->tail) {
    // Erase most recent value
    cb->buffer[cb->head-1] = value;
  }
  else {
    cb->buffer[cb->head] = value;
    cb->head = (cb->head + 1) % N;
  }
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
  return value;
}

void *producer(void *args) {
  producer_args *pargs = (producer_args *)args;
  circ_buffer *cb = pargs->cb;
  for (int i = 0; i < 2*N; i++) {
    put(cb, pargs->value[i]);
    usleep(100000);
  }
  free(pargs);
  pthread_exit(NULL);
}

void *consumer(void *args) {
  consumer_args *cargs = (consumer_args *)args;
  circ_buffer *cb = cargs->cb;
  for (int i = 0; i < 1.5*N; i++) {
    int value = get(cb);
    printf("Consumer: %d\n", value);
    usleep(200000);
  }
  free(cargs);
  pthread_exit(NULL);
}

int main() {
  circ_buffer cb;
  cb.head = 0;
  cb.tail = 0;

  producer_args *pargs = (producer_args *)malloc(sizeof(producer_args));
  pargs->cb = &cb;
  for (int i = 0; i < 2*N; i++) {
    pargs->value[i] = i;
  }

  pthread_t producer_thread;

  consumer_args *cargs = (consumer_args *)malloc(sizeof(consumer_args));
  cargs->cb = &cb;
  pthread_t consumer_thread;

  pthread_create(&producer_thread, NULL, producer, (void *)pargs);
  pthread_create(&consumer_thread, NULL, consumer, (void *)cargs);

  pthread_join(producer_thread, NULL);
  pthread_join(consumer_thread, NULL);

  free(pargs);
  free(cargs);
  return 0;
}
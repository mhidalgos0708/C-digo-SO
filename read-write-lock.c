#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define THREAD_NUM 4

pthread_rwlock_t rwlock;
int critical_region = 0;

void *rdlockThread(void *args) {
  int index = *(int *)args;

  pthread_rwlock_rdlock(&rwlock);

  printf("Thread %d - read %d\n", index, critical_region);

  pthread_rwlock_unlock(&rwlock);
  return NULL;
}

void *wrlockThread(void *args) {
  int index = *(int *)args;

  pthread_rwlock_wrlock(&rwlock);

  critical_region++;
  printf("Thread %d - wrote %d\n", index, critical_region);

  pthread_rwlock_unlock(&rwlock);
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t thread_writer[THREAD_NUM], thread_reader[THREAD_NUM];

  pthread_rwlock_init(&rwlock, NULL);

  for (int i = 0; i < THREAD_NUM; i++) {
    int *a = malloc(sizeof(int));
    *a = i;
    pthread_create(&thread_reader[i], NULL, &rdlockThread, a);
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    int *b = malloc(sizeof(int));
    *b = i;
    pthread_create(&thread_writer[i], NULL, &wrlockThread, b);
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(thread_reader[i], NULL);
    pthread_join(thread_writer[i], NULL);
  }

  pthread_rwlock_destroy(&rwlock);

  return 0;
}
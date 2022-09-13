#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 4

sem_t semaphore;

void *routine(void *args) { 
  int index = *(int*) args;
  sem_wait(&semaphore);
  sleep(1);
  printf("Hello from thread %d\n", index);
  sem_post(&semaphore);
  free(args);
  return NULL; 
}

int main() {
  pthread_t thread_array[THREAD_NUM];
  sem_init(&semaphore, 0, 1);

  for (int i = 0; i < THREAD_NUM; i++) {
    int *a = malloc(sizeof(int));
    *a = i;
    if (pthread_create(&thread_array[i], NULL, &routine, a) != 0) {
      perror("Failed to create threads");
    }
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    if (pthread_join(thread_array[i], NULL) != 0) {
      perror("Failed to join threads");
    }
  }

  sem_destroy(&semaphore);

  return 0;
}
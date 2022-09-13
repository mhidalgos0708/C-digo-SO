#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
int critical_region = 0;

void *routine(void *args) {
  int index = *(int*) args;
  pthread_mutex_lock(&mutex);
  critical_region++;
  sleep(1);
  printf("thread %d - counter %d\n", index, critical_region);
  pthread_mutex_unlock(&mutex);
  free(args);
  return NULL;
}

int main() {
  pthread_t thread1;
  pthread_t thread2;

  pthread_mutex_init(&mutex, NULL);

  int *a = malloc(sizeof(int));
  *a = 1;
  if (pthread_create(&thread1, NULL, &routine, a) != 0) {
    perror("Error creating thread1");
  }

  int *b = malloc(sizeof(int));
  *b = 2;
  if (pthread_create(&thread2, NULL, &routine, b) != 0) {
    perror("Error creating thread2");
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("Error joining thread1");
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("Error joining thread2");
  }

  pthread_mutex_destroy(&mutex);
  printf("El resultado es: %d\n", critical_region);

  return 0;
}
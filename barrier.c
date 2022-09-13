#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUM 8

int diceValues[THREAD_NUM];
int status[THREAD_NUM] = {0};

pthread_barrier_t barrierRolledDice;
pthread_barrier_t barrierCalculate;

void *rollDice(void *args) {
  int index = *(int *)args;
  diceValues[index] = rand() % 6 + 1;

  pthread_barrier_wait(&barrierRolledDice);
  pthread_barrier_wait(&barrierCalculate);

  if (status[index] == 1) {
    printf("(%d rolled %d) I won\n", index, diceValues[index]);
  } else {
    printf("(%d rolled %d) I lost\n", index, diceValues[index]);
  }
  free(args);
  return NULL;
}

int main() {
  srand(time(NULL));
  pthread_t thread_array[THREAD_NUM];

  pthread_barrier_init(&barrierRolledDice, NULL, THREAD_NUM + 1);
  pthread_barrier_init(&barrierCalculate, NULL, THREAD_NUM + 1);

  for (int i = 0; i < THREAD_NUM; i++) {
    int *a = malloc(sizeof(int));
    *a = i;
    if (pthread_create(&thread_array[i], NULL, &rollDice, a) != 0) {
      perror("Failed to create thread.");
    }
  }

  pthread_barrier_wait(&barrierRolledDice);

  int max = 0;
  for (int i = 0; i < THREAD_NUM; i++) {
    if (diceValues[i] > max) {
      max = diceValues[i];
    }
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    if (diceValues[i] == max) {
      status[i] = 1;
    } else {
      status[i] = 0;
    }
  }

  pthread_barrier_wait(&barrierCalculate);

  for (int i = 0; i < THREAD_NUM; i++) {
    if (pthread_join(thread_array[i], NULL) != 0) {
      perror("Failed to join thread.");
    }
  }

  pthread_barrier_destroy(&barrierRolledDice);
  pthread_barrier_destroy(&barrierCalculate);

  return 0;
}
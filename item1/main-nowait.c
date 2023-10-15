#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREAD 5
#define USLEEP 0 // Fix for running on Replit
#define MAX_NUM 10000

long request;
long respond;
ushort quit;

int SOMA = 0;

void *server(void *args) {
  while (!quit) {
    while (request == 0 && quit == 0) {
      usleep(USLEEP);
    }
    // printf("\nResolvendo request: %ld", request);
    // fflush(stdout);
    if (quit == 1) {
      printf("\nSaindo...\n");
      fflush(stdout);
      pthread_exit(NULL);
    }
    respond = request;
    while (respond != 0) {
      usleep(USLEEP);
    }
    request = 0;
  }

  printf("\nSaindo...\n");
  fflush(stdout);

  pthread_exit(NULL);
}

void *client(void *args) {
  long id = (long)args;

  for (int i = 0; i < MAX_NUM; i++) {

    /*while (respond != id) {
      if (request != id) {
        request = id;
      }
      usleep(USLEEP);
    }*/

    usleep(USLEEP);
    int local = SOMA;
    usleep((rand() % 2));
    SOMA = local + 1;

    respond = 0;
  }

  pthread_exit(NULL);
}

int main(void) {
  request = 0;
  respond = 0;
  quit = 0;

  pthread_t threads[NUM_THREAD] = {};

  printf("\nCriando 0");
  fflush(stdout);
  pthread_create(&threads[0], NULL, server, NULL);

  for (long i = 1; i < NUM_THREAD; i++) {
    printf("\nCriando %ld", i);
    fflush(stdout);
    long aux = i;
    pthread_create(&threads[i], NULL, client, (void *)aux);
  }

  for (long i = 1; i < NUM_THREAD; i++) {
    printf("\nEsperando %ld", i);
    fflush(stdout);
    pthread_join(threads[i], NULL);
  }

  printf("\n\n");

  quit = 1;

  printf("\nSoma final: %d\n\n", SOMA);

  pthread_exit(NULL);
  return 0;
}
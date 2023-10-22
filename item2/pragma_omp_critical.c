/**
 * Integrantes:
 *
 * Eduardo de Oliveira Motta
 * Gabriel Henrique Silva
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ITERNUM 10
#define N 2048
#define USEC 1000000
#define SLEEP_TIME 0.001
#define NUM_THREADS 4

int totalCells = 0; // Variável global para armazenar o total de células vivas

float getCell(float **grid, int i, int j) {
  return grid[(i + N) % N][(j + N) % N];
}

int getNeighbors(float **grid, int i, int j) {
  int neighbors = 0;
  if (getCell(grid, i - 1, j - 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i - 1, j) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i - 1, j + 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i, j - 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i, j + 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i + 1, j - 1) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i + 1, j) > 0.0f) {
    neighbors++;
  }
  if (getCell(grid, i + 1, j + 1) > 0.0f) {
    neighbors++;
  }
  return neighbors;
}

void swap(float ***grid, float ***newgrid) {
  float **temp = *grid;
  *grid = *newgrid;
  *newgrid = temp;
}

float average(float **grid, int i, int j) {
  float sum = 0.0f;
  sum += getCell(grid, i - 1, j - 1);
  sum += getCell(grid, i - 1, j);
  sum += getCell(grid, i - 1, j + 1);
  sum += getCell(grid, i, j - 1);
  sum += getCell(grid, i, j + 1);
  sum += getCell(grid, i + 1, j - 1);
  sum += getCell(grid, i + 1, j);
  sum += getCell(grid, i + 1, j + 1);
  return sum / 8.0f;
}

void iterate(float **grid, float **newgrid, int i, int j) {
  int neighbors = getNeighbors(grid, i, j);
  float value = grid[i][j];
  if (value > 0.0f) {
    if (neighbors < 2) {
      newgrid[i][j] = 0.0f;
    } else if (neighbors == 2 || neighbors == 3) {
      newgrid[i][j] = value;
    } else if (neighbors > 3) {
      newgrid[i][j] = 0.0f;
    }
  } else {
    if (neighbors == 3) {
      float newvalue = average(grid, i, j);
      if (newvalue > 0.0f)
        newgrid[i][j] = newvalue;
      else {
        newgrid[i][j] = 0.1f;
      }
    } else {
      newgrid[i][j] = 0.0f;
    }
  }
}

int numberOfCells(float **grid) {
  totalCells =
      0; // Inicializa a variável global para armazenar o total de células vivas

  struct timeval start_time, end_time;

  gettimeofday(&start_time, NULL);

#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (grid[i][j] >
          0.0f) { // Se o valor da célula for maior que zero, a célula está viva
#pragma omp critical
        {
          totalCells++; // Incrementa o total de células vivas usando uma seção
                        // crítica para garantir exclusão mútua
        }
      }
    }
  }

  gettimeofday(&end_time, NULL);

  long execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                        (end_time.tv_usec - start_time.tv_usec);
  printf("Total de Threads: %d \n", NUM_THREADS);
  printf("Tempo gasto em numberOfCells: %ld microssegundos\n", execution_time);

  return totalCells; // Retorna o total de células vivas na grade
}

void clearStdout() { printf("\033[H\033[J"); }

void printGrid(float **grid, int i, int stop) {
  printf("Número de Células: %d \n", numberOfCells(grid));
  for (int i = 0; i < stop; i++) {
    printf("|");
    for (int j = 0; j < stop; j++) {
      if (grid[i][j] == 0.0f) {
        printf("\033[1;30m██\033[0m");
      } else if (grid[i][j] < 0.2f) {
        printf("\033[1;31m██\033[0m");
      } else if (grid[i][j] < 0.4f) {
        printf("\033[1;33m██\033[0m");
      } else if (grid[i][j] < 0.6f) {
        printf("\033[1;32m██\033[0m");
      } else if (grid[i][j] < 0.8f) {
        printf("\033[1;36m██\033[0m");
      } else {
        printf("\033[1;34m██\033[0m");
      }
    }
    printf("|\n");
  }
}

void addGlider(float **grid, int x, int y) {
  grid[x][y + 1] = 1.0f;
  grid[x + 1][y + 2] = 1.0f;
  grid[x + 2][y] = 1.0f;
  grid[x + 2][y + 1] = 1.0f;
  grid[x + 2][y + 2] = 1.0f;
}

void addRPentomino(float **grid, int x, int y) {
  grid[x][y + 1] = 1.0f;
  grid[x][y + 2] = 1.0f;
  grid[x + 1][y] = 1.0f;
  grid[x + 1][y + 1] = 1.0f;
  grid[x + 2][y + 1] = 1.0f;
}

int getResult(void (*addPatterns)(float **grid)) {

  float **grid = (float **)malloc(sizeof(float *) * N);
  float **newgrid = (float **)malloc(sizeof(float *) * N);

  for (int i = 0; i < N; i++) {
    grid[i] = (float *)malloc(sizeof(float) * N);
    newgrid[i] = (float *)malloc(sizeof(float) * N);

    for (int j = 0; j < N; j++) {
      grid[i][j] = 0.0f;
      newgrid[i][j] = 0.0f;
    }
  }

  addPatterns(grid);

  // printGrid(grid, 0);

  omp_set_num_threads(NUM_THREADS);

  for (int i = 0; i < ITERNUM; i++) {
#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        iterate(grid, newgrid, i, j);
      }
    }

    swap(&grid, &newgrid);
    // usleep(USEC * SLEEP_TIME);
  }

  int cells = numberOfCells(grid);

  printf("Número de Células: %d \n", cells);

  for (int i = 0; i < N; i++) {
    free(grid[i]);
    free(newgrid[i]);
  }
  free(grid);
  free(newgrid);

  return cells;
}

void testOne(float **grid) {
  addGlider(grid, 1, 1);
  addRPentomino(grid, 10, 30);
}

int main(int argc, char *argv[]) {

  int result1 = getResult(testOne);

  printf("Número final de células: %d\n", totalCells); // Use a variável global

  return 0;
}

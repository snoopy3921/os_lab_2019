#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <getopt.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
};

void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (int i = 0; i < array_size; i++) {
    array[i] = rand();
  }
}

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  for(int i = args->begin; i < args->end - 1; i++) {
    sum += args->array[i];
    }
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args); /* We can instead calling pthread_exit(Sum(sum_args))*/
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;

  static struct option options[] = {{"seed", required_argument, 0, 's'},
    {"array_size", required_argument, 0, 'a'},
    {"threads_num", required_argument, 0, 't'},
    {0, 0, 0, 0}};
  
  int option_idx = 0;
  int c;
  while((c = getopt_long(argc, argv, "s:a:t:", options, &option_idx)) != -1)
  {
    switch (c)
    {
    case 's':
      /* code */
      printf("Seed: %s\n", optarg);
      seed = atoi(optarg);
      break;
    case 'a':
      /* code */
      printf("Array Size: %s\n", optarg);
      array_size = atoi(optarg);
      break;
    case 't':
      /* code */
      printf("Thread num: %s\n", optarg);
      threads_num = atoi(optarg);
      break;
    
    default: /* Case '?' */
      break;
    }
  }

  pthread_t threads[threads_num];
  /*
   * TODO:
   * your code here
   * Generate array here
   */

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
    args->array = array;
    args->begin = i * array_size / threads_num;
    args->end = (i + 1) * array_size / threads_num;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  free(array);
  printf("Total: %d\n", total_sum);
  return 0;
}

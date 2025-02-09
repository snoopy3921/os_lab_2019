#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

static int timeout_flag;
void handle_alarm()
{
  timeout_flag = 1;
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  timeout_flag = 0;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "s:a:p:f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (seed <= 0) {
              printf("seed is a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0) {
              printf("array_size is a positive number\n");
              return 1;
            } 
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0) {
              printf("pnum must be a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg);
            printf("Timeout: %d\n", timeout);
            if (timeout > 0) {
              signal(SIGALRM, handle_alarm); // Устанавливаем обработчик сигнала
              alarm(timeout); // Устанавливаем таймер
            }
            break;

          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        printf("Execute -f option\n");
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
  int** pipe_fd;
  pipe_fd=(int**)malloc(sizeof(int*) * pnum);
 

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (int i = 0; i < pnum; i++) {
    pipe_fd[i] = (int*)malloc(sizeof(int) * 2);
      // Create a pipe
    if (pipe(pipe_fd[i]) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        // parallel somehow
        int start_index = (i * array_size) / pnum;
        int end_index = ((i + 1) * array_size) / pnum;
        struct MinMax min_max_private;
        if (with_files) {
          // use files here
          char filename[20];
          sprintf(filename, "file%d.txt", i);
          FILE *file;
          file = fopen(filename, "w");
          min_max_private = GetMinMax(array, start_index, end_index);
          fprintf(file, "%d %d\n", min_max_private.min, min_max_private.max);
          fclose(file);
        } else {
          // use pipe here
          close(pipe_fd[i][0]); // Close the read end of the pipe

          min_max_private = GetMinMax(array, start_index, end_index);
          write(pipe_fd[i][1], &min_max_private, sizeof(min_max_private));
          close(pipe_fd[i][1]); // Close the write end of the pipe
        }
        /* wait the parent to be killed */
        if(timeout > 0) pause();
        
        return 0;
      }
      else{
          // while (timeout_flag == 0) {
          //   sleep(1);
          // }

          while(waitpid(child_pid, NULL, WNOHANG) == 0)
          {
            if(timeout_flag == 1)
            {
              printf("Timeout reached. Sending SIGKILL to child process (PID: %d)\n", child_pid);
              kill(child_pid, SIGKILL); // Send SIGKILL to the child process
              break;
            }
            sleep(1);
          }
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    // your code here
    wait(NULL);
    active_child_processes -= 1;
    printf("active_child_processes: %d\n", active_child_processes);
  }

  struct MinMax min_max;
  struct MinMax min_max_of_child;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;
    if (with_files) {
      // read from files
      char filename[20];
      sprintf(filename, "file%d.txt", i);
      FILE *file;
      file = fopen(filename, "r");
      fscanf(file, "%d %d", &min_max_of_child.min, &min_max_of_child.max);
      fclose(file);
      //remove(filename);
    } else {
      // read from pipes
      close(pipe_fd[i][1]); // Close the write end of the pipe
      read(pipe_fd[i][0], &min_max_of_child, sizeof(min_max_of_child));
      close(pipe_fd[i][0]); // Close the read end of the pipe
    }

    if (min_max_of_child.min < min_max.min) min_max.min = min_max_of_child.min;
    if (min_max_of_child.max > min_max.max) min_max.max = min_max_of_child.max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  // Free each row of pipe
  for (int i = 0; i < pnum; i++) {
      free(pipe_fd[i]);
  }
  free(pipe_fd);


  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}

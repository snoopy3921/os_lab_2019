#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <getopt.h>



static struct option options[] = {{"knum", required_argument, 0, 'k'},
{"pnum", required_argument, 0, 'p'},
{"mod", required_argument, 0, 'm'},
{0, 0, 0, 0}};

uint32_t num_k = 0;
uint32_t thread_num = 0;
uint32_t modulo = 1;

typedef struct 
{
      uint32_t num_start;
      uint32_t num_end;
}cal_multiply;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static uint64_t result = 1;


void thread_func(void *p_arg)
{
      cal_multiply *convert_arg = (cal_multiply *)p_arg;
      int res = 1;
      for(int i = convert_arg->num_start; i <= convert_arg->num_end; i++)
      {
            printf("i = %d\n", i);
            res *= i;
      }
      pthread_mutex_lock(&mut);
      result *= res;
      pthread_mutex_unlock(&mut);
}

int mod(int a, int m) {
      if (m == 0) {
          fprintf(stderr, "Error: Division by zero.\n");
          return -1; // Return an error code
      }
      return a % m; // Calculate and return the modulus
  }
  

int main(int argc, char **argv) 
{
      int c;
      while((c = getopt_long(argc, argv, "k:p:m:", options, NULL)) != -1)
      {
        switch (c)
        {
        case 'k':
          /* code */
          printf("number_k: %s\n", optarg);
          num_k = atoi(optarg);
          break;
        case 'm':
          /* code */
          printf("modulo: %s\n", optarg);
          modulo = atoi(optarg);
          break;
        case 'p':
          /* code */
          printf("Thread num: %s\n", optarg);
          thread_num = atoi(optarg);
          break;
        
        default: /* Case '?' */
          break;
        }
      }

      pthread_t p_thread_arrp[thread_num];
      cal_multiply pass_arg[thread_num];
      
      

      for(int i = 0; i < thread_num; i++)
      {     
            pass_arg[i].num_start = i * num_k / thread_num + 1;
            pass_arg[i].num_end = (i + 1) * num_k / thread_num;
            printf("pthread number %d start %d\n", i, pass_arg[i].num_start);
            printf("pthread number %d end %d\n", i, pass_arg[i].num_end);
            if(pthread_create(&p_thread_arrp[i], NULL, (void *)thread_func, (void *)&pass_arg[i]) != 0)
            {
                  printf("pthread number %d create error\n", i);
                  exit(1);
            }
      }
      for(int i = 0; i < thread_num; i++)
      {
            if(pthread_join(p_thread_arrp[i], NULL) != 0)
            {
                  printf("pthread number %d join error\n", i);
                  exit(1);
            }
      }

      printf("k!: %ld, result:%d \n", result, mod(result, modulo));
    
      return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    printf("Child ps: %d\n", getpid());
    exit(42); // Exit the child process immediately
  } else if (pid > 0) {
    printf("Parent ps: %d\n", getpid());
    sleep(10); //Sleep 10s to observe zombie
  } else {
    perror("fork");
    exit(1);
  }

  return 0;
}
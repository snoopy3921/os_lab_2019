#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
      char* arg[] = {"./main_sequent", "10", "100", NULL};
      execvp(arg[0], arg);
}
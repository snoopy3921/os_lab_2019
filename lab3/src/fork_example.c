#include <stdio.h>
#include <unistd.h> // Lib for using fork

int main() {
    pid_t pid = fork(); // Create new process

    if (pid < 0) {
        // Error, return -1
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child
        printf("This is child process with PID: %d\n", getpid());
    } else {
        // Parent
        printf("This is parent process with PID: %d and PID of child: %d\n", getpid(), pid);
    }

    // Both processes are running this
    printf("Both parent and child process are running...\n");

    return 0;
}
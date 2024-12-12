#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <semaphore.h>

#define NUM_THREADS 5
#define NUM_RESOURCES 3

sem_t resource_semaphore;

int main() 
{
    int pipefd[2];
    pid_t pid;
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(1);
    }

    int priority = nice(0);
    srand(time(NULL));   // Initalize random number generator
    if (pid == 0) {
        srand(time(NULL)); // Seed our random number generation using the current time
        // Child process
        priority = nice(10);
        printf("Child PID = %d\n", getpid());
        // Do thing
        close(pipefd[0]); // Close unused read end
        int* r = malloc(sizeof(int));
        *r = (rand() % 100);
        printf("Number: %d\n", *r);
        printf("Child: Sending random number...\n");
        write(pipefd[1], r, sizeof(*r));
        close(pipefd[1]);
        free(r);
        return 1;
    }
    else if (pid > 0) 
    {
    // Parent process
        int* r = malloc(sizeof(int));
        printf("Initial priority: %d\n", priority);
        printf("Parent PID = %d, Child PID = %d\n", getpid(), pid);
        wait(NULL); // Wait for child to finish
        close(pipefd[1]); // Close unused write end
        read(pipefd[0], r, sizeof(*r));
        printf("Parent: Received message: %d\n", *r);
        close(pipefd[0]);
        free(r);
    }

    else
    {
        fprintf(stderr, "Failed to fork process\n"); // Error: process could not be forked
        return 1;
    }
    return 0;
}
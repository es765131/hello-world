#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <linux/random.h>

#define ISVERBOSE 0 // Change this to 0 to see the randomly generated numbers.
#define USELINUXRAND 1 // Change this to 1 to use linux/random.h, otherwise uses the rand function.


#define NUM_THREADS 180
#define NUM_RESOURCES 3

sem_t resource_semaphore;

void* worker(void* arg)
{
    int id = *(int*)arg;
    
    if (ISVERBOSE == 1)
    {
        printf("Thread %d: Semaphore: Acquiring resource...\n", id);
    }
 
    sem_wait(&resource_semaphore);

    if (ISVERBOSE == 1)
    {
        printf("Thread %d: Semaphore: Resource acquired.\n", id);
    }
 
    // Set up pipes
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
    if (pid == 0) { // Child process
        srand(time(NULL)); // Seed our random number generation using the current time
        
        priority = nice(10);
        if (ISVERBOSE == 1)
        {
            printf("Child PID = %d\n", getpid());
        }

        close(pipefd[0]); // Close unused read end
        int* r = malloc(sizeof(int));
        if (USELINUXRAND == 0)
        {
            *r = (rand() % 99);
        }
        else
        {
            *r = (getrandom(*r, sizeof(int), 0));
        }
        if (ISVERBOSE == 1)
        {
            printf("Number: %d\n", *r);
            printf("Child: Sending random number...\n");
        }
        write(pipefd[1], r, sizeof(*r));
        close(pipefd[1]);
        free(r);
        return NULL;
    }
    else if (pid > 0) // Parent process
    {
        int* r = malloc(sizeof(int));
        if (ISVERBOSE == 1)
        {
            printf("Initial priority: %d\n", priority);
            printf("Parent PID = %d, Child PID = %d\n", getpid(), pid);
        }
        wait(NULL); // Wait for child to finish
        close(pipefd[1]); // Close unused write end
        read(pipefd[0], r, sizeof(*r));
        if (ISVERBOSE == 1)
        {
            printf("Parent: Received message: %d\n", *r);
        }
        if (ISVERBOSE == 0)
        {
            printf("%d\n", *r);
        }
        close(pipefd[0]);
        free(r);
    }

    else
    {
        fprintf(stderr, "Failed to fork process\n"); // Error: process could not be forked
        return NULL;
    }
    if (ISVERBOSE == 1)
    {
        printf("Thread %d: Releasing resource\n", id);
    }
    sem_post(&resource_semaphore);
 
    free(arg);
    return NULL;
}
int main() 
{
    pthread_t threads[NUM_THREADS];

    // Initialize semaphore with NUM_RESOURCES
    sem_init(&resource_semaphore, 0, NUM_RESOURCES);
    int j = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, worker, id);
        if (j == 2)
        {
            sleep(1);
            j = 0;
        }
        else
        {
            j++;
        }
        
    }
 
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
 
    sem_destroy(&resource_semaphore);
    return 0;
}

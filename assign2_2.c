#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

#define ISVERBOSE 1 // Change this to 0 to see the randomly generated numbers.


#define NUM_THREADS 30 // We generate three numbers every second, so this program will give us 60 seconds of random numbers.
#define NUM_RESOURCE 3 // This uses 3 resources to make sure that the three numbers are generated quickly,
                        // to show that generating numbers close together generates the same 'random' number.

sem_t resource_semaphore; // Define our semaphore

void* worker(void* arg) // Define our 'worker' thread code
{
    int id = *(int*)arg; // The thread id equals the arg (also called id) passed into worker by pthreadcreate. This is used to print to console which thread this is.
    
    if (ISVERBOSE == 1)
    {
        printf("Thread %d: Semaphore: Acquiring resource...\n", id); // If ISVERBOSE = 1 then print this message in the console.
    }
 
    sem_wait(&resource_semaphore); // Wait for the semaphore to aquire the resource.

    if (ISVERBOSE == 1) // If ISVERBOSE = 1 then print this message in the console.
    {
        printf("Thread %d: Semaphore: Resource acquired.\n", id);
    }
 
    // Set up pipes
    int pipefd[2]; // We will be using this pipe to send information from the child process to the parent process.
    if (pipe(pipefd) == -1) // -1 is the error code for the pipe function.
    {
        perror("pipe"); 
        printf("Thread %d: Failed to create pipe!\n", id);
        exit(1);    // If the pipe fails creation, print this message and terminate the program.
    }
    pid_t pid; // Setup process identification system
    pid = fork(); // Attempt to fork our process into a child and parent process
    if (pid == -1)  // Same with pid_t, -1 is the error code.
    {
        perror("fork");
        printf("Thread %d: Failed to fork process!\n", id);
        exit(1);    // If pid fails, print this message and terminate the program.
    }

    int priority = nice(0); // set process to zero (default)
    srand(time(NULL));   // Initalize random number generator using time as a seed. (Time in seconds)
    if (pid == 0) { // Child process
        srand(time(NULL)); // Seed our random number generation using the current time
        
        priority = nice(10); // set priority for the child process (10). This is a lower priority then 0.
        if (ISVERBOSE == 1)
        {
            printf("Child PID = %d\n", getpid()); // Print the process identifier of the child process
        }

        close(pipefd[0]); // Close unused read end of this pipe. We will only be writing a number, not reading.

        int* r = malloc(sizeof(int)); // Use malloc to dynamically allocate space for int r.

        *r = (rand() % 99); // Generate a random number from 1 to 99.

        if (ISVERBOSE == 1)
        {
            printf("Number: %d\n", *r); // Prints the number, used to compare with the number when the pipe is read from by the parent process.
            printf("Child: Sending random number...\n");
        }
        write(pipefd[1], r, sizeof(*r)); // Write the random number to pipe, using the sizeof() the integer as the buffer size.
        close(pipefd[1]); // Close the pipe, the data we want to send has been written.
        free(r); // Free the address space that contains r, so that it may be used elsewhere.
        return NULL; // End this process
    }
    else if (pid > 0) // Parent process
    {
        int* r = malloc(sizeof(int)); // Use malloc to dynamically allocate space for int r.
        if (ISVERBOSE == 1)
        {
            printf("Initial priority: %d\n", priority); // Print the priority of this process. (Will be 0).
            printf("Parent PID = %d, Child PID = %d\n", getpid(), pid); // Gets and prints the parent and child PID using
                                                                        // getpid() for the parent and simply the pid variable for the child process.
        }
        wait(NULL); // Wait for child process to finish running
        close(pipefd[1]); // Close unused write end of pipe
        read(pipefd[0], r, sizeof(*r)); // Read from the pipe we put the random number in via the child process
        if (ISVERBOSE == 1)
        {
            printf("Parent: Received message: %d\n", *r); // Print the recieved random number to compare with the input random number. If verbose is enabled.
        }
        if (ISVERBOSE == 0)
        {
            printf("%d\n", *r); // Otherwise, just print the number.
        }
        close(pipefd[0]); // We are done reading from the 
        free(r); // Clear up our r variable
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
    sem_post(&resource_semaphore); // Frees the semaphore for acquisition elsewhere.
 
    free(arg); // Free the memory storing the process id.
    return NULL;
}
int main() 
{
    pthread_t threads[NUM_THREADS]; // Initalise threads with the number of threads we want

    // Initialize semaphore with our NUM_RESOURCE variable
    sem_init(&resource_semaphore, 0, NUM_RESOURCE); // Initalise our semaphore
    int j = 0; // Used to count to three
    for (int i = 0; i < NUM_THREADS; i++) { // This for loop will create our threads. 
        int* id = malloc(sizeof(int));
        *id = i; // Thread id is equal to the iteration of the for loop.
        pthread_create(&threads[i], NULL, worker, id); // Create a thread that runs the worker function and has the id of the id variable.
        if (j == 2)
        {
            sleep(1); // Wait one second, this is how long it takes for the random numbers to change
            j = 0; // set j back to zero
        }
        else
        {
            j++; // otherwise, add 1 to our j variable.
        }
        
    }
 
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // Waits for each thread to be terminated.
    }
 
    sem_destroy(&resource_semaphore); // Terminate the semaphore, releasing the system resources it was using.
    return 0;
}

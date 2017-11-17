/*
This program creates a semaphore and passes it to a number of threads.
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t *tids;
int n;
void *threadout (void *args);

//TO START DECLARE GLOBALS OF
//BOOK TITLE
//CHAPTER TITLE
//SO THREADOUT CAN GRAB THEM AND OPEN THE FILES
#define BUFFER_SIZE 1024
char book [BUFFER_SIZE];
char chapter [BUFFER_SIZE];
int counter;

int main (int argc, char *argv [])
{
    int error;
    int i;
    sem_t sem;

    //NEXT CHECK THAT 4 PARAMETERS WERE GIVEN
    if (argc != 4)
    {
        fprintf (stderr, "Usage: %s number_of_threads\n", argv [0]);
        return 1;
    }
    //Initalize Counter to 1
    counter =1;
    //ASSIGN CHAPTER TITLE, AND BOOK TITLE TO GLOBAL VARIABLES
    strcpy(chapter , argv [1]);
    strcpy(book, argv [3]);
/*
****************** Allocate space for thread ids ******************
*/
    //GRAB NUMBER OF THREADS TO CREATE FROM PASSED IN PARAMETERS
    n = atoi (argv [2]);
    tids = (pthread_t *) calloc (n, sizeof (pthread_t));
    if (tids == NULL)
    {
        perror ("Failed to allocate memory for thread IDs");
        return 1;
    }
    if (sem_init (&sem, 0, 1) == -1)
    {
        perror ("Failed to initialize semaphore");
        return 1;
    }

/*
****************** Create threads *********************************
*/
    for (i = 0; i < n; i++)
    {
        if (error = pthread_create (tids + i, NULL, threadout, &sem))
        {
            fprintf (stderr, "Failed to create thread:%s\n", strerror (error));
            return 1;
        }
    }

/*
****************** Wait for threads to terminate ******************
*/
    for (i = 0; i < n; i++)
    {
        if (error = pthread_join (tids [i], NULL))
        {
            fprintf (stderr, "Failed to join thread:%s\n", strerror (error));
            return 1;
        }
    }

/*
****************** Destroy thread *********************************
*/
    if (sem_destroy (&sem) == -1)
    {
        perror ("Failed to destroy semaphore");
        return 1;
    }

    fprintf (stdout, "The book has been created \n");

    return 0;
}

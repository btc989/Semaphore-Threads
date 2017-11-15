/*
This program creates a semaphore and passes it to a number of threads.
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
pthread_t *tids;
int n;
void *threadout (void *args);

int main (int argc, char *argv [])
{
    char buffer [BUFFER_SIZE];
    char *c;
    char *d;
    char *e;
    int error;
    int i;
    sem_t sem;
    char istring[32];






    if (argc != 4)
    {
        fprintf (stderr, "Usage: %s number_of_threads\n", argv [0]);
        return 1;
    }

/*
****************** Allocate space for thread ids ******************
*/
    n = atoi (argv [2]);
    d = argv [3];
    
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
    for (i = 1; i <= n; i++)
    {
        strcpy(buffer, argv[1]);
        sprintf(istring, "%d", i);
        strcat(buffer,istring);
        c= buffer;
        e=i;
        if (error = pthread_create (tids + i, NULL, threadout, &sem, c,d,e ))
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

    fprintf (stdout, "This is tid 1 from pid %ld\n", (long) getpid ());

    return 0;
}
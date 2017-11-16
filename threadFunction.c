/*
A thread function with a critical section protected by a semaphore
passed as its parameter.
*/

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define TEN_MILLION 10000000L
#define BUFFER_SIZE 1024

extern pthread_t *tids;
extern int n;

void *threadout (void *args)
{
    char buffer [BUFFER_SIZE];
    char *c;
    int i;
    sem_t *semp;
    pthread_t tid = pthread_self ();
    struct timespec sleeptime;

    semp = (sem_t *) args;
    sleeptime.tv_sec = 0;
    sleeptime.tv_nsec = TEN_MILLION;

//ADDED
//Create the title of the chapter ex. mychapter2
    strcpy(chap, chapter);
    sprintf(istring, "%d", tid);
    strcat(chap,istring);

//OPEN FILE
    inFile = open(chap, O_RDONLY); //open the file to send to server
    if (inFile == -1) {
        printf("Error: Could Not Open File %s /n",chap);
        return NULL;
    }
//END

/*
This for loop converts a thread id into an integer. It works
on both Hercules and the Linux machines in CL115. It solves
the problem of printing thread ids when they are implemented
as structs (such on the Linux machines in CL115). In this way,
thread ids are handled consistently when printed from the
different POSIX implementations.
*/
    for (i = 0; i < n; i ++)
    {
        if (pthread_equal (tid, tids [i]))
        {
            break;
        }
    }
    snprintf (buffer, BUFFER_SIZE, "This is tid %d from pid %ld\n",
              (int) i + 2, (long) getpid ());
    c = buffer;

/*
****************** Lock ****************************************
*/

//SHOULD THIS BE A CHECK IF CHAPTER ID == SHARED VARIABLE?????
    while (sem_wait (semp) == -1)
    {
        if(errno != EINTR)
        {
            fprintf (stderr, "Thread failed to lock semaphore\n");
            return NULL;
        }
    }

/*
****************** Critical Section ****************************
*/
    //ADDED
    /*

        //Check shared counter is value equals same as chapter number
    int x = getShared(semp);
    if(x== cnum){
        //IF TRUE THEN OPEN BOOK
        outFile=open(book,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if(outFile == -1){
            printf("Error could not open book \n");
            return NULL;
        }
        //THEN COPY ALL OF CHAPTER OVER TO BOOK
       while( (n_char=read(inFile, buffer, 10))!=0)
        {
                //Display the characters read
                n_char=write(outFile,buffer,n_char);
        }
        //INCREMENT SHARED COUNTER
        x= incrementShared();
    }
    */
    //END OF ADDED
    
    
    
    
    while (*c != '\0')
    {
        fputc (*c, stderr);
        c++;
        nanosleep (&sleeptime, NULL);
    }

/*
****************** Unlock **************************************
*/
    if (sem_post (semp) == -1)
    {
        fprintf (stderr, "Thread failed to unlock semaphore\n");
    }

    return NULL;
}

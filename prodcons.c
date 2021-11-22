#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/*
This program provides a possible solution for producer-consumer problem using mutex and semaphore.
I have used 1 producers and 1 consumers to demonstrate the solution.
*/
#define MaxItems 5   // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
char buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{
    char array[BufferSize] = {'a', 'b', 'c', 'd', 'e'};
    for (int i = 0; i < MaxItems; i++)
   // while (1)
    {//this function blocked untill buffer is full
        sem_wait(&empty); //wait whenever buffer is full and its decreamented from 5 to 0
        //means no space into buffer to write just wait or blocked untilled consume
        //wont execute untill 0
        pthread_mutex_lock(&mutex);
            buffer[in] = array[i];
    //    buffer[in] = array[in];
        //void pointer cannot be directly deferenced
        printf("Producer %d: Insert Item %c at %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
//switching between thread can happened at any given time might happen between any iteration but using semaphore it gurantee
//that must be atomic operation(uniterruptable)
// gives gurantee that removing from buffer happened after the inserting (writing )
void *consumer(void *cno)
{
    char con_array[BufferSize];
    for (int i = 0; i < MaxItems; i++)
    //while (1)
    {//this function will block when buffer is empty
        sem_wait(&full);//0 lock 
        pthread_mutex_lock(&mutex);
        con_array[i] = buffer[out];
       // con_array[out] = buffer[out];
        //void pointer cannot be directly deferenced
        printf("Consumer %d: Remove Item %c from %d\n", *((int *)cno), con_array[out], out);
        out = (out + 1) % BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
//without mutual exclusion output
// Producer 1: Insert Item a at 0
// Consumer 2: Remove Item a from 0
// Producer 1: Insert Item b at 1
// Producer 1: Insert Item c at 2
// Consumer 2: Remove Item b from 1
// Consumer 2: Remove Item c from 2
// Producer 1: Insert Item d at 3
// Consumer 2: Remove Item d from 3
// Consumer 2: Remove Item e from 4
// Producer 1: Insert Item e at 4
int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BufferSize);//initially it will have 5
    sem_init(&full, 0, 0);
    int pid = 1;
    int cid = 2;
    //suppose producer id =1
    //suppose consumer id =2
    pthread_create(&t1, NULL, (void *)producer, (void *)&pid);

    pthread_create(&t2, NULL, (void *)consumer, (void *)&cid);

    pthread_join(t1, NULL);

    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    //using semaphore and mutex
    // Producer 1: Insert Item a at 0
    // Consumer 2: Remove Item a from 0
    // Producer 1: Insert Item b at 1
    // Consumer 2: Remove Item b from 1
    // Producer 1: Insert Item c at 2
    // Consumer 2: Remove Item c from 2
    // Producer 1: Insert Item d at 3
    // Consumer 2: Remove Item d from 3
    // Producer 1: Insert Item e at 4
    // Consumer 2: Remove Item e from 4
    return 0;
}
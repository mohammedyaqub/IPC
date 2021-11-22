#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
int x = 10;//this is the var shared between threads
sem_t s1;

const int max = 1000000;
//to synchronize the threads producer then consumer so need two semaphore
void *fun1(void *arg)
{

    sem_wait(&s1);//initial time decrement the value which will become zero not availble
    for (int i = 0; i < max; i++)
    {
        x++;
    }
    sem_post(&s1);
    printf("after incrementing inside fun1 the x val 10000 + 10 = %d\n", x);

    //sem_post(&s1);
    pthread_exit(NULL);
}
void *fun2(void *arg)
{

    sem_wait(&s1);
    for (int i = 0; i < max; i++)
    {
        x--;
    }
    printf(" after decrement in  fun2 it must be 10 == %d\n", x);

    sem_post(&s1);
    pthread_exit(NULL);
}
int main()
{
    pthread_t pt1, pt2;  // thread handle
    sem_init(&s1, 0, 1); //initially available
    pthread_create(&pt1, NULL, fun1, NULL);
    pthread_create(&pt2, NULL, fun2, NULL);
    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);
    sem_destroy(&s1);
    printf("x is %d\n", x);
    printf("main--thank you\n");
    return 0; // exit(0);
}

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

const int max = 10;
sem_t s1;
sem_t s2;
//to synchronize the threads producer then consumer so need two semaphore 
void *fun1(void *pv) // consumer
{
  int i;
  sem_wait(&s2);//initially blocked here as of s2 == 0
  sem_wait(&s1);
  for (i = 1; i <= max; i++) {
    printf("consumer %d\n", i);
    sleep(1);
  }
  sem_post(&s1);
  // pthread_exit(NULL);
}
void *fun2(void *pv) // producer
{
  int i;
  sem_wait(&s1);//initially available to execute then decrement thus making it not available to other threads
  for (i = 1; i <= max; i++) {
    printf("producer --%d\n", i);
    sleep(1);
  }
  sem_post(&s1);//increament s1 can make it available to other threads after successfully done 
  sem_post(&s2);//giving chance to consumer to consume
  // pthread_exit(NULL);
}
int main() {
  pthread_t pt1, pt2; // thread handle
  sem_init(&s1, 0, 1);//initially available 
  sem_init(&s2, 0, 0);//initially not available
  pthread_create(&pt1, NULL, fun1, NULL);
  pthread_create(&pt2, NULL, fun2, NULL);
  pthread_join(pt1, NULL);
  pthread_join(pt2, NULL);
  sem_destroy(&s1);
  sem_destroy(&s2);
  printf("main\n");
  return 0; // exit(0);
}

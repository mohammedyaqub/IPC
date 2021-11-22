#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//synchronization in threads using locks
int MAX = 10;
int count = 1;
pthread_mutex_t thr;
pthread_cond_t cond;
void *even(void *arg){
   while(count < MAX) {
      pthread_mutex_lock(&thr);
      while(count % 2 != 0) {
         //checking if the number is not even so just waiting or sleeping 
         pthread_cond_wait(&cond, &thr);
         //behind the scene above func calls
         //pthread_mutex_unlock(&thr)
         //wait for signal to receive then 
         //pthread_mutex_lock(&thr)
        // pthread_cond_broadcast() function shall unblock all threads currently blocked on the specified condition variable cond.         
   
      }
      printf("evennumber %d\n ", count++);
      pthread_mutex_unlock(&thr);
      pthread_cond_signal(&cond);
   }
   pthread_exit(0);
}
void *odd(void *arg){
   while(count < MAX) {
      pthread_mutex_lock(&thr);
      while(count % 2 != 1) {
         //checking if the number is not odd so just waiting or sleeping then it must be even  
         pthread_cond_wait(&cond, &thr);
      }
      printf("oddnumber %d\n ", count++);
      pthread_mutex_unlock(&thr);
      pthread_cond_signal(&cond);
   }
   pthread_exit(0);
}
int main(){
   pthread_t thread1;
   pthread_t thread2;
   pthread_mutex_init(&thr, 0);
   pthread_cond_init(&cond, 0);
   pthread_create(&thread1, 0, even, NULL);
   pthread_create(&thread2, 0, odd, NULL);
   pthread_join(thread1, 0);
   pthread_join(thread2, 0);
   pthread_mutex_destroy(&thr);
   pthread_cond_destroy(&cond);
   return 0;
}
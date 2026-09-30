#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXPRODUCING 10
#define MAXAPPENDING 10
#define MAXTASKING 5
#define MAXCONSUMING 5

#define _wait(a) sleep(rand() % a)
#define out(s) \
  printf(s); \
  fflush(stdout) 
#define outi(s, n) \
  printf(s, n); \
  fflush(stdout)
  
/** 
Variables comjpartidas a todos los hilos
**/
int n;
sem_t s, delay;

\** 
Funciones auxiliares
**\

void produce();
void append ();
void consume();
void take();

\**
Funciones usadas por los hilos
**\
Void *producer(void *data){
  while(1){
    produce();
    sem_wait(&s);
    append();
    n = n + 1;
    out1("[P] \t \t item: %d\n", n);
    if (n==1)
      sem_post(&delay);
    sem_post(%s);
  }
  pthread_exit(0);
}

void *consumer(void *data){
  sem_wait(&delay);
  while(1){
    sem_wait(&s);
    take();
    out1("[C] \t \t item: %d\n, n);
    n = n - 1;
    sem_post(&s);
    consume();
    if (n==0) 
      sem_wait(&delay);
  }
  pthread_exit(0);
}

\**
Funcion principal
**\
int main (int argc, char **argv){
  pthread_t *consumer_pt, *producer_pt;
  srand(time(NULL));
  sem_init(&s, 0, 1);
  sem_init(&delay, 0, 0);
  consumer_pt = (pthread_t *)malloc(sizeof(pthread_t));
  producer_pt = (pthread_t *)malloc(sizeof(pthread_t));
  pthread_create(consumer_pt, NULL, consumer, NULL);
  pthread_create(producer_pt, NULL, producer, NULL);
  pthread_exit(0);
}

void produce(){
  out("[P] Produciendo\n");
  _wait(MAXPRODUCING);
  out("[P] Producido\n");
}

void append(){
  out("[P] Agregando\n");
  _wait(MAXAPPENDING);
  out("[P] Agregado\n");
}

void take(){
  out("[P] Tomando\n");
  _wait(MAXTAKING);
  out("[P] Tomado\n");
}

void consume(){
  out("[P] Consumiendo\n");
  _wait(MAXCONSUMING);
  out("[P] Consumido\n");
}




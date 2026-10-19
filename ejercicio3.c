#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 5
#define NP 3
#define NC 3
#define NITERS 4

typedef struct {
    int buf[BUFF_SIZE];
    int in;
    int out;
    sem_t full;
    sem_t empty;
    sem_t mutex;
} sbuf_t;

sbuf_t shared;

/* Funcion del productor */
void *Producer(void *arg)
{
    int i, item;
    int index = (int)(intptr_t)arg;

    for (i = 0; i < NITERS; i++) {

        /* Producir un elemento */
        item = i;

        /* Esperar una posicion libre */
        sem_wait(&shared.empty);

        /* Acceso exclusivo al buffer */
        sem_wait(&shared.mutex);

        shared.buf[shared.in] = item;
        shared.in = (shared.in + 1) % BUFF_SIZE;

        printf("[P%d] Producing %d ...\n", index, item);
        fflush(stdout);

        /* Liberar el buffer */
        sem_post(&shared.mutex);

        /* Indicar que hay un elemento disponible */
        sem_post(&shared.full);

        /* Intercalar la ejecucion de los hilos */
        if (i % 2 == 1)
            sleep(1);
    }

    return NULL;
}

/* Funcion del consumidor */
void *Consumer(void *arg)
{
    int i, item;
    int index = (int)(intptr_t)arg;

    for (i = 0; i < NITERS; i++) {

        /* Esperar hasta que exista un elemento */
        sem_wait(&shared.full);

        /* Acceso exclusivo al buffer */
        sem_wait(&shared.mutex);

        /* Retirar el siguiente elemento */
        item = shared.buf[shared.out];
        shared.out = (shared.out + 1) % BUFF_SIZE;

        printf("------> [C%d] consumed %d\n", index, item);
        fflush(stdout);

        /* Liberar el buffer */
        sem_post(&shared.mutex);

        /* Indicar que hay una posicion libre */
        sem_post(&shared.empty);
    }

    return NULL;
}

int main(void)
{
    pthread_t producers[NP];
    pthread_t consumers[NC];
    int index;

    /* Inicializar los indices del buffer */
    shared.in = 0;
    shared.out = 0;

    /* Inicializar los semaforos */
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    sem_init(&shared.mutex, 0, 1);

    /* Crear los productores */
    for (index = 0; index < NP; index++) {
        pthread_create(
            &producers[index],
            NULL,
            Producer,
            (void *)(intptr_t)index
        );
    }

    /* Crear los consumidores */
    for (index = 0; index < NC; index++) {
        pthread_create(
            &consumers[index],
            NULL,
            Consumer,
            (void *)(intptr_t)index
        );
    }

    /* Esperar a que terminen los productores */
    for (index = 0; index < NP; index++) {
        pthread_join(producers[index], NULL);
    }

    /* Esperar a que terminen los consumidores */
    for (index = 0; index < NC; index++) {
        pthread_join(consumers[index], NULL);
    }

    /* Liberar los semaforos */
    sem_destroy(&shared.full);
    sem_destroy(&shared.empty);
    sem_destroy(&shared.mutex);

    return 0;
}


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 1000

pthread_t workers[NUM_THREADS];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct {
    int count;
} shared_data;

void *run(void *arg)
{
    pthread_mutex_lock(&lock);
    shared_data.count++;
    pthread_mutex_unlock(&lock);
    printf("Thread #%ld checked in\n", (long)arg);
    return NULL;
}

int main(void)
{
    int error;
    for (int i=0;i<NUM_THREADS;i++)
    {
        error = pthread_create(&workers[i], NULL, &run, (void *)i);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }

    for (int i=0;i<NUM_THREADS;i++) {
        pthread_join(workers[i], NULL);
    }
    printf("shared_data.count = %d\n", shared_data.count);
    return 0;
}
